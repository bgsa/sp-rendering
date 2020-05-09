#ifndef I_RENDERER_MANAGER_HEADER
#define I_RENDERER_MANAGER_HEADER

#include "SpectrumRendering.h"
#include "SpWindow.h"
#include "RenderData.h"
#include "GraphicObject.h"
#include "Camera.h"

namespace NAMESPACE_RENDERING
{
	class IRendererManager
	{
	protected:
		Camera* _camera = NULL;

	public:
		
		API_INTERFACE inline Camera* camera()
		{
			return _camera;
		}

		API_INTERFACE virtual void init(SpWindow* window) = 0;

		API_INTERFACE virtual void start() = 0;

		API_INTERFACE virtual void stop() = 0;

		API_INTERFACE virtual void update() = 0;

		API_INTERFACE virtual void preRender() = 0;

		API_INTERFACE virtual void render() = 0;

		API_INTERFACE virtual void postRender() = 0;

		API_INTERFACE virtual void resize(sp_float width, sp_float height) = 0;

		API_INTERFACE virtual void addGraphicObject(GraphicObject* graphicObject) = 0;
		API_INTERFACE virtual sp_bool hasGraphicObject(GraphicObject* graphicObject) = 0;
		API_INTERFACE virtual void removeGraphicObject(GraphicObject* graphicObject) = 0;

	};
}

#endif // !I_RENDERER_MANAGER_HEADER