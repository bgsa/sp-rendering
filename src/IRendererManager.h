#ifndef I_RENDERER_MANAGER_HEADER
#define I_RENDERER_MANAGER_HEADER

#include "SpectrumRendering.h"
#include "DisplayDevice.h"
#include "RenderData.h"
#include "GraphicObject.h"

namespace NAMESPACE_RENDERING
{
	class IRendererManager
	{
	public:

		API_INTERFACE virtual void init(DisplayDevice* displayDevice) = 0;

		API_INTERFACE virtual void start() = 0;

		API_INTERFACE virtual void stop() = 0;

		API_INTERFACE virtual void update() = 0;

		API_INTERFACE virtual void preRender() = 0;

		API_INTERFACE virtual void render() = 0;

		API_INTERFACE virtual void postRender() = 0;

		API_INTERFACE virtual void resize(sp_float width, sp_float height) = 0;

		API_INTERFACE virtual void addGraphicObject(GraphicObject* graphicObject) = 0;

	};
}

#endif // !I_RENDERER_MANAGER_HEADER