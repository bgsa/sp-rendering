#ifndef GRAPHIC_OBJECT_3D
#define GRAPHIC_OBJECT_3D

#include "SpectrumRendering.h"
#include "GLConfig.h"
#include "GraphicObject.h"
#include "RendererSettings.h"
#include <vector>
#include "ContentAlignment.h"
#include <BoundingVolume.h>

namespace NAMESPACE_RENDERING
{
	template <class BoundingVolumeType>
	class GraphicObject3D : public GraphicObject
	{
	protected:
		ColorRGBAf * color = new ColorRGBAf{ 1.0f, 1.0f, 1.0f, 1.0f };

	public:
		NAMESPACE_PHYSICS::BoundingVolume<BoundingVolumeType>* boundingVolume = NULL;

		virtual ColorRGBAf* getColor() {
			return color;
		}
		virtual void setColor(ColorRGBAf color) {
			this->color->Red = color.Red;
			this->color->Green = color.Green;
			this->color->Blue = color.Blue;
			this->color->Alpha = color.Alpha;
		}

		GraphicObjectType type() override
		{
			return GraphicObjectType::Type3D;
		}

		GraphicObject3D* scale(float xScale, float yScale, float zScale)
		{
			modelView *= Mat4f::createScale(xScale, yScale, zScale);
			boundingVolume->scale(xScale, yScale, zScale);

			return this;
		}
		GraphicObject3D* scaleUniform(float scaleValue)
		{
			modelView *= Mat4f::createScale(scaleValue, scaleValue, scaleValue);
			boundingVolume->scale(scaleValue, scaleValue, scaleValue);

			return this;
		}

		GraphicObject3D* translate(float xAxis, float yAxis, float zAxis)
		{
			modelView *= Mat4f::createTranslate(xAxis, yAxis, zAxis);
			boundingVolume->translate(xAxis, yAxis, zAxis);

			return this;
		}

		GraphicObject3D* rotate(float angleInRadians, float xAxis, float yAxis, float zAxis)
		{
			modelView *= Mat4f::createRotate(angleInRadians, xAxis, yAxis, zAxis);
			boundingVolume->rotate(angleInRadians, xAxis, yAxis, zAxis);

			return this;
		}

	};
}

#endif GRAPHIC_OBJECT_3D