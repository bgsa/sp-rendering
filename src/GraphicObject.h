#ifndef GRAPHIC_OBJECT
#define GRAPHIC_OBJECT

#include "SpectrumRendering.h"
#include <Object.h>
#include "RenderData.h"

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
	private:
		sp_char* tag;

	protected:

		GLuint programShader;
		GLuint vertexBufferObject;

		GLint projectionMatrixLocation;
		GLint viewMatrixLocation;
		GLint modelViewLocation;

		GLint positionAttribute = -1;
		GLint colorLocation = -1;

	public:

		Mat4f modelView = Mat4f::identity();

		API_INTERFACE virtual void init() = 0;

		API_INTERFACE void setTag(sp_char* tag) {
			this->tag = tag;
		}
		API_INTERFACE sp_char* getTag() {
			return tag;
		}

		API_INTERFACE virtual void update(sp_longlong elapsedTime) { };

		API_INTERFACE virtual void render(const RenderData& renderData) = 0;

		API_INTERFACE virtual GraphicObjectType type() = 0;

		API_INTERFACE virtual ~GraphicObject()
		{
			if (programShader != 0)
				glDeleteProgram(programShader);

			if (vertexBufferObject != 0)
				glDeleteBuffers(1, &vertexBufferObject);
		}

	};
}

#endif // GRAPHIC_OBJECT
