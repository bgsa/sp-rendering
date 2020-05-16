#ifndef GRAPHIC_OBJECT_HEADER
#define GRAPHIC_OBJECT_HEADER

#include "SpectrumRendering.h"
#include <Object.h>
#include "RenderData.h"
#include "SpTransform.h"

namespace NAMESPACE_RENDERING
{
	enum GraphicObjectType
	{
		Type2D = 2,
		Type3D = 3
	};

	class GraphicObject
		: public Object
	{
	protected:
		sp_uint programShader;
		sp_uint vertexBufferObject;

		sp_int projectionMatrixLocation;
		sp_int viewMatrixLocation;
		sp_int transformMatrixLocation;

		sp_int positionAttribute = -1;
		sp_int colorLocation = -1;

	public:
		SpTransform transform;

		API_INTERFACE GraphicObject()
		{
		}

		API_INTERFACE virtual void init() = 0;

		API_INTERFACE virtual void update(sp_longlong elapsedTime) { };

		API_INTERFACE virtual void render(const RenderData& renderData) = 0;

		API_INTERFACE virtual GraphicObjectType type() = 0;

		API_INTERFACE virtual void dispose() override
		{
			if (programShader != 0)
			{
				glDeleteProgram(programShader);
			}

			if (vertexBufferObject != 0)
			{
				glDeleteBuffers(1, &vertexBufferObject);
			}
		}

		API_INTERFACE virtual ~GraphicObject()
		{
			dispose();
		}

	};
}

#endif // GRAPHIC_OBJECT_HEADER