#ifndef DEFAULT_RENDERER_MANAGER_HEADER
#define DEFAULT_RENDERER_MANAGER_HEADER

#include "SpectrumRendering.h"
#include "RendererEditor.h"

#include <Timer.h>
#include "Camera.h"
#include "GraphicObject2D.h"
#include "GraphicObject3D.h"
#include <CollisionResponse.h>

#include "SphereX.h"
#include "Cube.h"
#include <OBB.h>
#include "CubeList.h"
#include "RockRenderer.h"

namespace NAMESPACE_RENDERING
{
	class DefaultRendererManager
		: public IRendererManager
	{
	private:
		std::vector<GraphicObject*> graphicObjects;

		Timer timer;

		bool isRunning = true;

		Rock* rock = NULL;
		RockRenderer* rockRenderer = NULL;

	protected:
		SpWindow* window;
		RendererEditor* editor = NULL;

		void render2D(RenderData renderData);
		void render3D(RenderData renderData);

	public:
		API_INTERFACE virtual void init(SpWindow* window) override;
		API_INTERFACE virtual void start() override;
		API_INTERFACE void stop() override;
		API_INTERFACE void resize(sp_float width, sp_float height) override;

		API_INTERFACE void update() override;
		API_INTERFACE void preRender() override;
		API_INTERFACE void render() override;
		API_INTERFACE void postRender() override;

		API_INTERFACE void addGraphicObject(GraphicObject* graphicObject) override;
		API_INTERFACE sp_bool hasGraphicObject(GraphicObject* graphicObject) override;
		API_INTERFACE void removeGraphicObject(GraphicObject* graphicObject) override;

		API_INTERFACE inline void setRendererEditor(RendererEditor* editor)
		{
			this->editor = editor;
		}

		API_INTERFACE ~DefaultRendererManager();
	};
}

#endif // DEFAULT_RENDERER_MANAGER_HEADER