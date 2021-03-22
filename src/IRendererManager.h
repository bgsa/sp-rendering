#ifndef I_RENDERER_MANAGER_HEADER
#define I_RENDERER_MANAGER_HEADER

#include "SpectrumRendering.h"
#include "SpWindow.h"
#include "GraphicObject2D.h"
#include "GraphicObject3D.h"
#include "GraphicObject3DList.h"
#include "SpCamera.h"
#include "SpViewportData.h"
#include "SpPhysicObject.h"
#include "SpPhysicObjectList.h"
#include "DOP18Renderer.h"
#include "DOP18ListRenderer.h"
#include "LogGL.hpp"

namespace NAMESPACE_RENDERING
{
	class IRendererManager
	{
	protected:
		SpCamera* _camera = NULL;
		SpViewportData* _viewport;

		SpVector<GraphicObject*> graphicObjects2D;
		SpVector<GraphicObject*> graphicObjects3D;

		//DOP18Renderer dop18Renderer;
		//DOP18ListRenderer dop18ListRenderer;
		
		virtual void render3D(const RenderData& renderData)
		{
			for (SpVectorItem<GraphicObject*>* item = graphicObjects3D.begin(); item != nullptr; item = item->next())
			{
				item->value()->render(renderData);

				/*
				SpPhysicObject* physicObject = dynamic_cast<SpPhysicObject*>(item->value());
				if (physicObject != nullptr)
					dop18Renderer.render(renderData, physicObject->boundingVolume());
				*/

				//SpPhysicObjectList* physicObjectList = dynamic_castdate<SpPhysicObjectList*>(item->value());

				/*
				if (physicObjectList != nullptr)
					dop18ListRenderer.render(renderData, (DOP18*) physicObjectList->boundingVolumes(0u), physicObjectList->length());
				*/

				LogGL::glErrors(__FILE__, __LINE__);
			}
		}

		virtual void render2D(const RenderData& renderData)
		{
			for (SpVectorItem<GraphicObject*>* item = graphicObjects2D.begin(); item != nullptr; item = item->next())
				item->value()->render(renderData);
		}

	public:

		API_INTERFACE IRendererManager()
		{
			_viewport = sp_mem_new(SpViewportData)();
		}
		
		API_INTERFACE inline SpCamera* camera()
		{
			return _camera;
		}

		API_INTERFACE inline SpViewportData* viewport()
		{
			return _viewport;
		}

		API_INTERFACE virtual void init(SpCamera* camera)
		{
			//dop18Renderer.init();
			//dop18ListRenderer.init();
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

		API_INTERFACE virtual void addGraphicObject(GraphicObject3DList* graphicObjectList)
		{
			graphicObjects3D.add(graphicObjectList);
		}

		API_INTERFACE virtual sp_bool hasGraphicObject(GraphicObject2D* graphicObject)
		{
			SpVectorItem<GraphicObject*>* item = graphicObjects2D.find(graphicObject);
			return item != nullptr;
		}

		API_INTERFACE virtual sp_bool hasGraphicObject(GraphicObject3D* graphicObject)
		{
			SpVectorItem<GraphicObject*>* item = graphicObjects3D.find(graphicObject);
			return item != nullptr;
		}

		API_INTERFACE virtual void removeGraphicObject(GraphicObject3D* graphicObject)
		{
			SpVectorItem<GraphicObject*>* item = graphicObjects3D.find(graphicObject);

			if (item != nullptr)
				graphicObjects3D.remove(item);
		}

		API_INTERFACE virtual void removeGraphicObject(GraphicObject2D* graphicObject)
		{
			SpVectorItem<GraphicObject*>* item = graphicObjects2D.find(graphicObject);

			if (item != nullptr)
				graphicObjects3D.remove(item);
		}

	};
}

#endif // !I_RENDERER_MANAGER_HEADER