#ifndef GRAPHIC_OBJECT_3D_LIST_HEADER
#define GRAPHIC_OBJECT_3D_LIST_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject.h"
#include "IGraphicObjectRenderer.h"
#include "SpArray.h"
#include "OpenGLBuffer.h"
#include "OpenGLTextureBuffer.h"
#include "SpTransform.h"

namespace NAMESPACE_RENDERING
{
	class GraphicObject3DList : 
		public GraphicObject
	{
	protected:
		IGraphicObjectRenderer* renderer;
		SpArray<SpTransform>* _transforms;
		SpArray<sp_uint>* _indexes;
		OpenGLBuffer* _buffer;
		OpenGLTextureBuffer* _transformsAsTexture;

	public:
		
		API_INTERFACE GraphicObject3DList()
		{
			renderer = nullptr;
			_transforms = nullptr;
		}

		API_INTERFACE sp_uint length()
		{
			return _transforms->length();
		}

		API_INTERFACE virtual SpArray<sp_uint>* indexes()
		{
			return _indexes;
		}

		API_INTERFACE virtual OpenGLBuffer* buffer()
		{
			return _buffer;
		}

		API_INTERFACE virtual OpenGLTextureBuffer* trasnformAsTexture()
		{
			return _transformsAsTexture;
		}

		API_INTERFACE virtual SpTransform& transforms(const sp_uint index)
		{
			return _transforms->data()[index];
		}

		API_INTERFACE void setLength(sp_uint length)
		{
			_transforms = sp_mem_new(SpArray<SpTransform>)(length, length);
		}

		API_INTERFACE void setRenderer(IGraphicObjectRenderer* renderer)
		{
			this->renderer = renderer;
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			renderer->render(renderData);
		}

		API_INTERFACE GraphicObjectType type() override
		{
			return GraphicObjectType::Type3D;
		}

		API_INTERFACE virtual void dispose() override
		{
			if (_transforms != nullptr)
			{
				sp_mem_delete(_transforms, SpArray<SpTransform>);
				_transforms = nullptr;
			}

			GraphicObject::dispose();
		}

	};
}

#endif // GRAPHIC_OBJECT_3D_LIST_HEADER