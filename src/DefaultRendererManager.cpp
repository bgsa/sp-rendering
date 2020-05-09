#include "DefaultRendererManager.h"
#include <algorithm>
#include "GLConfig.h"
#include "RendererSettings.h"
#include "LogGL.hpp"
#include "SpEventDispatcher.h"

namespace NAMESPACE_RENDERING
{
	void DefaultRendererManager::start()
	{
#if defined(WINDOWS) || defined(LINUX) || defined(MAC)
		sp_float nextTick = ZERO_FLOAT;
		sp_bool fpsLowerThanFrameLimit = false;
		sp_float elapsedTime = ZERO_FLOAT;

		while (isRunning)
		{
			elapsedTime = sp_float(timer.getElapsedTime());
			nextTick = timer.getSkipTick();

			do
			{
				SpEventDispatcher::instance()->processAllEvents();
				update();

				fpsLowerThanFrameLimit = elapsedTime > nextTick;
				nextTick += timer.getSkipTick();
			} while (fpsLowerThanFrameLimit);

			//timeInterpolated = timer.getFramesPerSecond() + SKIP_TICKS - FRAMES_PER_SECOND_LIMIT / SKIP_TICKS;
			//render(timeInterpolated);
			preRender();
			render();
			postRender();

			//cout << "FPS: " << timer.getFramesPerSecond() << END_OF_LINE;
			//cout << "Elapsed Time: " << timer.getElapsedTime() << END_OF_LINE;

			timer.update();

			if (!isRunning)
				break;
			else
				LogGL::glErrors(__FILE__, __LINE__);
		}
#endif
	}

	void DefaultRendererManager::addGraphicObject(GraphicObject* graphicObject)
	{
		graphicObjects.push_back(graphicObject);
	}

	sp_bool DefaultRendererManager::hasGraphicObject(GraphicObject* graphicObject)
	{
		std::vector<GraphicObject*>::iterator item = std::find(graphicObjects.begin(), graphicObjects.end(), graphicObject);
		return item != graphicObjects.end();
	}

	void DefaultRendererManager::removeGraphicObject(GraphicObject* graphicObject)
	{
		std::vector<GraphicObject*>::iterator item = std::find(graphicObjects.begin(), graphicObjects.end(), graphicObject);

		if (item != graphicObjects.end())
			graphicObjects.erase(item);
	}

	void DefaultRendererManager::resize(sp_float width, sp_float height)
	{
		if (width == 0 || height == 0)
			return;

		RendererSettings* settings = RendererSettings::getInstance();
		Vec2f currentSize = settings->getSize();

		if (currentSize.x == width && currentSize.y == height)
			return;

		settings->setSize(width, height);

		glViewport(0, 0, (GLsizei) width, (GLsizei)height);
		glScissor(0, 0, (GLsizei)width, (GLsizei)height);

		_camera->updateProjectionPerspectiveAspect(settings->getAspectRatio());
	}

	void DefaultRendererManager::init(SpWindow* window)
	{
		this->window = window;

		Log::info("OpenGL Vendor: " + GLConfig::getGLVendor());
		Log::info("OpenGL Version: " + GLConfig::getGLVersion());
		Log::info("OpenGLSL Version: " + GLConfig::getGLShadingLanguageVersion());
		Log::info("OpenGL Renderer: " + GLConfig::getGLRenderer());
		Log::info("OpenGL Extensions:");

		for (std::string extension : GLConfig::getGLExtensions())
			Log::info(extension);

		timer.start();

		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST); //elimina os v�rtices que sobrepoem outros v�rtices quando est�o no mesmo eixo Z.
		glEnable(GL_BLEND);									  //enable alpha color
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    //enable alpha color
		glEnable(GL_LINE_SMOOTH);

		Vec3f cameraPosition = { 0.0f, 12.0f, -17.0f };
		Vec3f cameraTarget = { 0.0f, 10.0f, 0.0f };
		_camera = sp_mem_new(Camera)();
		_camera->initProjectionPerspective(cameraPosition, cameraTarget, window->state()->aspectRatio());

		glViewport(0, 0, window->state()->width, window->state()->height);

		if (editor != NULL)
			editor->init(this);

		rockRenderer = sp_mem_new(RockRenderer)();
		rock = sp_mem_new(Rock)();
		rock->init();
		rock->setRenderer(rockRenderer);
		rockRenderer->setObjects(rock, ONE_UINT);
		this->addGraphicObject(rock);
	}

	void DefaultRendererManager::update()
	{
		timer.update();

		sp_longlong elapsedTime = timer.getElapsedTime();

		/*
		// CHECK COLLISIONS !!!!    CUBE (OBB) x CUBE (OBB)
		GraphicObject* a = this->graphicObjects[0];

		CubeList* cubeList = dynamic_cast<CubeList*>(a);
		Cube cube1 = cubeList->cubes[0];
		Cube cube2 = cubeList->cubes[1];

		OBB* obb1 = (OBB*) cube1.boundingVolume;
		OBB* obb2 = (OBB*) cube2.boundingVolume;
		OpenML::CollisionStatus status = obb1->collisionStatus(*obb2);

		if (status == OpenML::CollisionStatus::INSIDE) {
			Log::info("colidindo!");
		}
		else {
			Log::info("not colidindo!");
		}
		*/

		/*
		// CHECK COLLISIONS !!!!
		GraphicObject* a1 = this->graphicObjects[0];
		GraphicObject* a2 = this->graphicObjects[1];

		SphereX* ss1 = dynamic_cast<SphereX*>(a1);
		SphereX* ss2 = (SphereX*)a2;
		OpenML::CollisionStatus status = ss1->geometry.colisionStatus(ss2->geometry);

		if (status != OpenML::CollisionStatus::OUTSIDE) {
			CollisionResponse* response = CollisionResponse::handle(&ss1->geometry, &ss2->geometry);

			if (response != NULL)
			{
				ss1->geometry.particleSystem->particles[0].velocity = response->object1Impulse;
				ss2->geometry.particleSystem->particles[0].velocity = response->object2Impulse;
			}
		}
		// END COLLISION !
		*/
		
		for (GraphicObject* graph : graphicObjects)
			graph->update(elapsedTime);
	}

	void DefaultRendererManager::render3D(RenderData renderData)
	{
		for (GraphicObject* graph : graphicObjects) {

			if (graph->type() == GraphicObjectType::Type3D)
				graph->render(renderData);
		}
	}

	void DefaultRendererManager::render2D(RenderData renderData)
	{
		for (GraphicObject* graph : graphicObjects) {

			if (graph->type() == GraphicObjectType::Type2D)
				graph->render(renderData);
		}
	}

	void DefaultRendererManager::preRender()
	{
		if (editor != NULL)
			editor->preRender();
	}

	void DefaultRendererManager::render()
	{
		RendererSettings * settings = RendererSettings::getInstance();
		Vec2f size = settings->getSize();
		sp_float aspectRatio = settings->getAspectRatio();
		ColorRGBAf backgroundColor = settings->getBackgroudColor().normalizeColor();
		
		_camera->updateProjectionPerspectiveAspect(aspectRatio);

		RenderData renderData;
		renderData.projectionMatrix = _camera->getProjectionMatrix();
		renderData.viewMatrix = _camera->getViewMatrix() * _camera->transform;

		glViewport(0, 0, (GLsizei) size.x, (GLsizei)size.y);
		glScissor(0, 0, (GLsizei)size.x, (GLsizei)size.y);
		glClearColor(backgroundColor.Red, backgroundColor.Green, backgroundColor.Blue, backgroundColor.Alpha);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//camera->rotate(0.05f, 0.0f, 1.0f, 0.0f);
		//camera->rotate(-0.5f, 1.0f, 0.0f, 0.0f);
		//camera->rotate(0.05f, 1.0f, 1.0f, 0.0f);

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);

		render3D(renderData);

		renderData.projectionMatrix = _camera->getHUDProjectionMatrix(size.x, size.y);

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		render2D(renderData);

		if (editor != NULL)
			editor->render(renderData);

		window->refresh();
	}

	void DefaultRendererManager::postRender()
	{
		if (editor != NULL)
			editor->postRender();
	}

	void DefaultRendererManager::stop()
	{
		isRunning = false;
	}

	DefaultRendererManager::~DefaultRendererManager()
	{
		if (_camera != NULL)
		{
			sp_mem_delete(_camera, Camera);
			_camera = NULL;
		}

		if (editor != NULL)
		{
			editor->dispose();
			editor = NULL;
		}
	}
}