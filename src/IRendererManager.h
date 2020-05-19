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

		SpVector<GraphicObject2D*> graphicObjects2D;
		SpVector<GraphicObject3D*> graphicObjects3D;

		DOP18Renderer dop18Renderer;
		
		virtual void render3D(const RenderData& renderData)
		{
			for (SpVectorItem<GraphicObject3D*>* item = graphicObjects3D.begin(); item != nullptr; item = item->next())
			{
				item->value()->render(renderData);

				SpPhysicObject* physicObject = dynamic_cast<SpPhysicObject*>(item->value());
				if (physicObject != nullptr)
					dop18Renderer.render(renderData, physicObject->boundingVolume());
			}
		}

		virtual void render2D(const RenderData& renderData)
		{
			for (SpVectorItem<GraphicObject2D*>* item = graphicObjects2D.begin(); item != nullptr; item = item->next())
				item->value()->render(renderData);
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
			for (SpVectorItem<GraphicObject3D*>* item = graphicObjects3D.begin(); item != nullptr; item = item->next())
				item->value()->update(elapsedTime);

			for (SpVectorItem<GraphicObject2D*>* item = graphicObjects2D.begin(); item != nullptr; item = item->next())
				item->value()->update(elapsedTime);
		}

		API_INTERFACE virtual void preRender() = 0;

		API_INTERFACE virtual void render() = 0;

		API_INTERFACE virtual void postRender() = 0;

		API_INTERFACE virtual void resize(sp_float width, sp_float height) = 0;

		API_INTERFACE virtual void addGraphicObject(GraphicObject2D* graphicObject)
		{
			graphicObjects2D.add(graphicObject);
		}

		API_INTERFACE virtual void addGraphicObject(GraphicObject3D* graphicObject)
		{
			graphicObjects3D.add(graphicObject);
		}

		API_INTERFACE virtual sp_bool hasGraphicObject(GraphicObject3D* graphicObject)
		{
			SpVectorItem<GraphicObject3D*>* item = graphicObjects3D.find(graphicObject);
			return item != nullptr;
		}

		API_INTERFACE virtual void removeGraphicObject(GraphicObject3D* graphicObject)
		{
			SpVectorItem<GraphicObject3D*>* item = graphicObjects3D.find(graphicObject);

			if (item != nullptr)
				graphicObjects3D.remove(item);
		}

	};
}

#endif // !I_RENDERER_MANAGER_HEADER