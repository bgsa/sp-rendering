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
		char* tag;

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

		void setTag(char* tag) {
			this->tag = tag;
		}
		char* getTag() {
			return tag;
		}

		virtual void update(long long elapsedTime) { };
		virtual void render(const RenderData& renderData) = 0;

		virtual GraphicObjectType type() = 0;

		virtual ~GraphicObject()
		{
			if (programShader != 0)
				glDeleteProgram(programShader);

			if (vertexBufferObject != 0)
				glDeleteBuffers(1, &vertexBufferObject);
		}

	};
}

#endif GRAPHIC_OBJECT