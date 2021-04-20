#ifndef GRAPHIC_OBJECT_3D_LIST_HEADER
#define GRAPHIC_OBJECT_3D_LIST_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject.h"
#include "SpIGraphicObjectList3D.h"
#include "IGraphicObjectRenderer.h"
#include "SpArray.h"
#include "OpenGLBuffer.h"
#include "OpenGLTextureBuffer.h"
#include "SpTransform.h"

namespace NAMESPACE_RENDERING
{
	class GraphicObject3DList 
		: public SpIGraphicObjectList3D
		, public GraphicObject
	{
	protected:
		IGraphicObjectRenderer* renderer;
		OpenGLBuffer* _indexesBuffer;
		OpenGLBuffer* _buffer;

	public:
		
		API_INTERFACE GraphicObject3DList()
		{
			renderer = nullptr;
			_indexesBuffer = nullptr;
			_buffer = nullptr;
		}

		API_INTERFACE virtual OpenGLBuffer* indexesBuffer()
		{
			return _indexesBuffer;
		}

		API_INTERFACE virtual OpenGLBuffer* buffer()
		{
			return _buffer;
		}

		API_INTERFACE virtual void translate(const sp_uint index, const Vec3& translation) = 0;

		API_INTERFACE virtual void scale(const sp_uint index, const Vec3& factors) = 0;

		API_INTERFACE void setRenderer(IGraphicObjectRenderer* renderer)
		{
			this->renderer = renderer;
		}

		API_INTERFACE void render(const SpRenderData& renderData) override
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