#ifndef GRAPHIC_OBJECT_3D
#define GRAPHIC_OBJECT_3D

#include "SpectrumRendering.h"
#include "GLConfig.h"
#include "GraphicObject.h"
#include "RendererSettings.h"
#include <vector>
#include "ContentAlignment.h"
#include <BoundingVolume.h>
#include "IGraphicObjectRenderer.h"

namespace NAMESPACE_RENDERING
{
	class GraphicObject3D : 
		public GraphicObject
	{
	protected:
		IGraphicObjectRenderer* renderer = NULL;
		ColorRGBAf* color = new ColorRGBAf{ 1.0f, 1.0f, 1.0f, 1.0f };

	public:

		API_INTERFACE GraphicObject3D()
			: GraphicObject()
		{
		}

		API_INTERFACE virtual void setRenderer(IGraphicObjectRenderer* renderer)
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

		API_INTERFACE virtual void dispose() override
		{
			GraphicObject::dispose();
		}

	};
}

#endif // GRAPHIC_OBJECT_3D
