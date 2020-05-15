#ifndef GRAPHIC_OBJECT_3D_LIST_HEADER
#define GRAPHIC_OBJECT_3D_LIST_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject.h"
#include "IGraphicObjectRenderer.h"
#include "SpArray.h"
#include "OpenGLBuffer.h"
#include "OpenGLTextureBuffer.h"

namespace NAMESPACE_RENDERING
{
	template <class GameObjectType>
	class GraphicObject3DList : 
		public GraphicObject
	{
	protected:
		IGraphicObjectRenderer<GameObjectType>* renderer;
		Mat4f* _transforms;
		sp_uint _length;
		SpArray<sp_uint>* _indexes;
		OpenGLBuffer* _buffer;
		OpenGLTextureBuffer* _transformsAsTexture;

	public:
		
		API_INTERFACE GraphicObject3DList()
		{
			renderer = nullptr;
			_transforms = nullptr;
			_length = ZERO_UINT;
		}

		API_INTERFACE sp_uint length()
		{
			return _length;
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

		API_INTERFACE Mat4f* transforms()
		{
			return _transforms;
		}

		API_INTERFACE void setLength(sp_uint length)
		{
			_length = length;
			_transforms = sp_mem_new_array(Mat4f, length);
		}

		API_INTERFACE void setRenderer(IGraphicObjectRenderer<GameObjectType>* renderer)
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
				sp_mem_release(_transforms);
				_transforms = nullptr;
			}

			GraphicObject::dispose();
		}

	};
}

#endif // GRAPHIC_OBJECT_3D_LIST_HEADER