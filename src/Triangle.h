#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include <cstdlib>

#include "Shader.h"
#include <Image.h>
#include <ImageFactory.h>

namespace NAMESPACE_RENDERING
{
	struct ShaderAttributes {
		sp_float position[9];
		sp_float color[12];
	};

	class Triangle
	{
	private:
		GLuint programShader;

		GLuint triangleVBO;
		GLint positionAttribute;
		GLint colorAttribute;

		ShaderAttributes attributes;

		void initVBO();
		void setUpPositionAttribute();
		void setUpColorAttribute();

	public:
		Triangle();
		~Triangle();

		ShaderAttributes getAttributes();

		void setColor(GLfloat newColor[12]);

		void init(ShaderAttributes attributes);
		void init(ShaderAttributes attributes, const char * vertexShaderSource, const char * fragmentShaderSource);

		void render();
	};
}

#endif // TRIANGLE_HEADER