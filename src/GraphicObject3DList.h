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
		OpenGLTextureBuffer* _transformsBuffer;
		OpenGLBuffer* _indexesBuffer;
		OpenGLBuffer* _buffer;

		sp_uint _length;

	public:
		
		API_INTERFACE GraphicObject3DList()
		{
			renderer = nullptr;
			_transforms = nullptr;
		}

		API_INTERFACE sp_uint length()
		{
			return _length;
		}

		API_INTERFACE virtual void setLength(sp_uint length)
		{
			if (_transforms != nullptr)
			{
				sp_mem_delete(_transforms, SpArray<SpTransform>);
			}

			_transforms = sp_mem_new(SpArray<SpTransform>)(length, length);
			this->_length = length;
		}

		API_INTERFACE virtual OpenGLBuffer* indexesBuffer()
		{
			return _indexesBuffer;
		}

		API_INTERFACE virtual OpenGLBuffer* buffer()
		{
			return _buffer;
		}

		API_INTERFACE virtual void translate(const sp_uint index, Vec3 translation) = 0;

		API_INTERFACE virtual void scale(const sp_uint index, Vec3 factors) = 0;

		API_INTERFACE virtual OpenGLTextureBuffer* transformsBuffer()
		{
			return _transformsBuffer;
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

	};
}

#endif // GRAPHIC_OBJECT_3D_LIST_HEADER