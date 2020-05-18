#ifndef I_RENDERER_MANAGER_HEADER
#define I_RENDERER_MANAGER_HEADER

#include "SpectrumRendering.h"
#include "SpWindow.h"
#include "GraphicObject2D.h"
#include "GraphicObject3D.h"
#include "Camera.h"
#include "SpViewportData.h"
#include "SpPhysicObject.h"
#include "DOP18Renderer.h"

namespace NAMESPACE_RENDERING
{
	class IRendererManager
	{
	protected:
		Camera* _camera = NULL;
		SpViewportData* _viewport;

		std::vector<GraphicObject*> graphicObjects;

		DOP18Renderer dop18Renderer;
		
		virtual void render3D(const RenderData& renderData)
		{
			for (GraphicObject* graph : graphicObjects) 
			{
				if (graph->type() == GraphicObjectType::Type3D)
					graph->render(renderData);

				SpPhysicObject* physicObject = dynamic_cast<SpPhysicObject*>(graph);
				if (physicObject != nullptr)
					dop18Renderer.render(renderData, physicObject->boundingVolume());
			}
		}

		virtual void render2D(const RenderData& renderData)
		{
			for (GraphicObject* graph : graphicObjects) {

				if (graph->type() == GraphicObjectType::Type2D)
					graph->render(renderData);
			}
		}

	public:

		API_INTERFACE IRendererManager()
		{
			_viewport = sp_mem_new(SpViewportData)();
		}
		
		API_INTERFACE inline Camera* camera()
		{
			return _camera;
		}

		API_INTERFACE inline SpViewportData* viewport()
		{
			return _viewport;
		}

		API_INTERFACE virtual void init(Camera* camera)
		{
			dop18Renderer.init();
		}

		API_INTERFACE virtual void update(sp_longlong elapsedTime)
		{
			for (GraphicObject* graph : graphicObjects)
				graph->update(elapsedTime);
		}

		API_INTERFACE virtual void preRender() = 0;

		API_INTERFACE virtual void render() = 0;

		API_INTERFACE virtual void postRender() = 0;

		API_INTERFACE virtual void resize(sp_float width, sp_float height) = 0;

		API_INTERFACE virtual void addGraphicObject(GraphicObject* graphicObject)
		{
			graphicObjects.push_back(graphicObject);
		}

		API_INTERFACE virtual sp_bool hasGraphicObject(GraphicObject* graphicObject)
		{
			std::vector<GraphicObject*>::iterator item = std::find(graphicObjects.begin(), graphicObjects.end(), graphicObject);
			return item != graphicObjects.end();
		}

		API_INTERFACE virtual void removeGraphicObject(GraphicObject* graphicObject)
		{
			std::vector<GraphicObject*>::iterator item = std::find(graphicObjects.begin(), graphicObjects.end(), graphicObject);

			if (item != graphicObjects.end())
				graphicObjects.erase(item);
		}

	};
}

#endif // !I_RENDERER_MANAGER_HEADER