#ifndef GRAPHIC_OBJECT_3D
#define GRAPHIC_OBJECT_3D

#include "SpectrumRendering.h"
#include "GLConfig.h"
#include "GraphicObject.h"
#include "RendererSettings.h"
#include <vector>
#include "ContentAlignment.h"
#include <BoundingVolume.h>
#include "IRenderer.h"

namespace NAMESPACE_RENDERING
{
	template <class GameObjectType, class BoundingVolumeType>
	class GraphicObject3D : 
		public GraphicObject
	{
	protected:
		IRenderer<GameObjectType>* renderer = NULL;
		ColorRGBAf * color = new ColorRGBAf{ 1.0f, 1.0f, 1.0f, 1.0f };

	public:
		BoundingVolume<BoundingVolumeType>* boundingVolume = NULL;

		API_INTERFACE GraphicObject3D()
			: GraphicObject()
		{
			boundingVolume = sp_mem_new(BoundingVolumeType)();
		}

		API_INTERFACE virtual void setRenderer(IRenderer<GameObjectType>* renderer)
		{
			this->renderer = renderer;
		}

		API_INTERFACE virtual ColorRGBAf* getColor() {
			return color;
		}
		API_INTERFACE virtual void setColor(ColorRGBAf color) {
			this->color->Red = color.Red;
			this->color->Green = color.Green;
			this->color->Blue = color.Blue;
			this->color->Alpha = color.Alpha;
		}

		API_INTERFACE GraphicObjectType type() override
		{
			return GraphicObjectType::Type3D;
		}

		API_INTERFACE GraphicObject3D* scale(sp_float xScale, sp_float yScale, sp_float zScale)
		{
			transform *= Mat4f::createScale(xScale, yScale, zScale);
			boundingVolume->scale(xScale, yScale, zScale);

			return this;
		}
		API_INTERFACE GraphicObject3D* scaleUniform(sp_float scaleValue)
		{
			transform *= Mat4f::createScale(scaleValue, scaleValue, scaleValue);
			boundingVolume->scale(scaleValue, scaleValue, scaleValue);

			return this;
		}

		API_INTERFACE GraphicObject3D* translate(sp_float xAxis, sp_float yAxis, sp_float zAxis)
		{
			transform *= Mat4f::createTranslate(xAxis, yAxis, zAxis);
			boundingVolume->translate(xAxis, yAxis, zAxis);

			return this;
		}

		API_INTERFACE GraphicObject3D* rotate(sp_float angleInRadians, sp_float xAxis, sp_float yAxis, sp_float zAxis)
		{
			transform *= Mat4f::createRotate(angleInRadians, xAxis, yAxis, zAxis);
			boundingVolume->rotate(angleInRadians, xAxis, yAxis, zAxis);

			return this;
		}

		API_INTERFACE virtual void dispose() override
		{
			if (boundingVolume != nullptr)
			{
				boundingVolume->dispose();
				sp_mem_release(boundingVolume);
			}

			GraphicObject::dispose();
		}

	};
}

#endif // GRAPHIC_OBJECT_3D
