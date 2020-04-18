#ifndef SPRITE_RENDERER_HEADER
#define SPRITE_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "Shader.h"
#include <Factory.h>
#include <IFileManager.h>
#include <Image.h>
#include <ImageFactory.h>
#include <vector>
#include <map>
#include <pugixml/pugixml.hpp>

namespace NAMESPACE_RENDERING
{
	struct SpriteShaderAttributes {
		GLfloat position[8];
		GLfloat color[16];
		GLfloat texture[8];
	};

	struct SpriteFrame
	{
		float posisionX;
		float positionY;
		float width;
		float height;
	};

	struct SpriteAnimation
	{
		SpriteFrame frames[9];
		std::map<std::string, std::vector<int>> animations;
	};

	class Sprite
	{
	private:
		Mat4f model = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};

		GLuint programShader;
		SpriteShaderAttributes attributes;

		GLuint squareVBO;

		GLint modelLocation;
		GLint projectionViewLocation;
		GLint textureCoordinateLocation;
		GLint positionAttribute;
		GLint colorAttribute;
		GLint textureAttribute;
		GLuint texture;

		SpriteAnimation spriteData;

		void initVBO();
		void setUpPositionAttribute();
		void setUpColorAttribute();
		void setUpTextureAttribute();

		void loadSpriteAnimation();

	public:
		~Sprite();

		void init();
		void init(SpriteShaderAttributes attributes);
		void init(SpriteShaderAttributes attributes, const char * vertexShaderSource, const char * fragmentShaderSource);

		void update(long long elapsedTime);

		inline void translate(float x, float y, float z);
		inline void scale(float xScale, float yScale, float zScale);
		inline void rotate(float angle, float x, float y, float z);
		inline void rotateByCenter(float angle, float x, float y, float z);
		void render(Mat4f projectionViewMatrix);
	};
}

#endif // !SPRITE_RENDERER_HEADER