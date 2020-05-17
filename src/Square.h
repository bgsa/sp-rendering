#ifndef SQUARE_HEADER
#define SQUARE_HEADER

#include <cstdlib>
#include <vector>

#include "Shader.h"
#include <Image.h>
#include <ImageFactory.h>

namespace NAMESPACE_RENDERING
{
	class Square
	{
	private:
		Mat4 model = {
			//model matrix (translate, scale and rotate) here
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};

		GLuint programShader;

		GLuint squareVBO;

		GLint modelLocation;
		GLint projectionViewLocation;
		GLint positionAttribute;
		GLint colorAttribute;
		GLint textureAttribute;
		GLuint texture;

		void initVBO();
		void setUpPositionAttribute();
		void setUpColorAttribute();
		void setUpTextureAttribute();

		void setUpImage(std::string filename);

	public:
		~Square();

		void init();

		void render(Mat4 projectionViewMatrix);
	};
}

#endif // SQUARE_HEADER