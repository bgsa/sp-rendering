#include "Square.h"

namespace NAMESPACE_RENDERING
{
	struct SquareShaderAttributes {
		GLfloat position[8];
		GLfloat color[16];
		GLfloat texture[8];
	};

	SquareShaderAttributes squareAttributes = SquareShaderAttributes{
		{ //position  (space coordinates)
			0.0f, 0.0f, //bottom-left
			0.0f, 100.0f, //top-left
			100.0f, 0.0f, //bottom-right
			100.0f, 100.0f, //top-right	
		},
		{ //color 
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		},
		{ //texture (u,v)
			0.0f, 0.0f,  // Top-left
			0.0f, 1.0f,  // Bottom-left
			1.0f, 0.0f,  // Top-right
			1.0f, 1.0f   // Bottom-right
		}
	};

	void Square::init()
	{
		const char* vertexShaderSource = "#version 300 es \n"
			"uniform mat4 projectionView; \n"
			"uniform mat4 model; \n"

			"in  vec2 Position; \n"
			"in  vec4 Color; \n"
			"in  vec2 Texture; \n"

			"out vec4 fragmentColor; \n"
			"out vec2 fragmentTexture; \n"

			"void main() \n"
			"{											\n"
			"	gl_Position = projectionView * model * vec4(Position, 0.0, 1.0); \n"

			"	fragmentTexture = Texture; \n"
			"	fragmentColor = Color; \n"
			"}											\n";

		const char* fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"

			"in  vec4 fragmentColor; \n"
			"in  vec2 fragmentTexture; \n"

			"out vec4 FragColor; \n"
			"uniform sampler2D tex; \n"

			"void main() \n"
			"{\n"
			"	FragColor = texture(tex, fragmentTexture) * fragmentColor; \n"
			"}";

		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		initVBO();
	}

	void Square::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			0); //Specify that our coordinate data is going into attribute index 0(shaderAttribute), and contains three floats per vertex
		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	void Square::setUpColorAttribute()
	{
		glVertexAttribPointer(colorAttribute,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(sizeof(squareAttributes.position)) // deslocamento do primeiro elemento
		);
		glEnableVertexAttribArray(colorAttribute); //habilita atributo de coordenadas
	}

	void Square::setUpTextureAttribute()
	{
		glVertexAttribPointer(textureAttribute,
			2, //quantidad de valores por vertices da textura
			GL_FLOAT,		   // o tipo de cada elementos
			GL_FALSE,          // estado dos nossos valores.
			0,					// sem dados intercalados em cada posição da textura, ou seja, os valores da texturas estão contíguos no array de vértices
			(void*)(sizeof(squareAttributes.position) + (sizeof(squareAttributes.color))) // deslocamento do primeiro elemento
		);
		glEnableVertexAttribArray(textureAttribute);
	}

	void Square::setUpImage(std::string filename)
	{
		Image *image = ImageFactory::load(filename.c_str());
		unsigned char* pixels = image->getData();
		GLsizei width = image->getWidth();
		GLsizei height = image->getHeight();
		GLenum colorFormat = image->getColorFormat();

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//caso utilize mipmap ...
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//set texture border color
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

		glTexImage2D(GL_TEXTURE_2D, //target
			0, //level
			colorFormat, //GL_RGB, //internalFormat
			width, //width,
			height, //height,
			0, //border
			colorFormat, //format GL_RGB  or GL_RGBA (format of data read from textel file) (GL_RGB for *.bmp files and GL_RGBA for *.rgb files) GL_ALPHA GL_RGB GL_RGBA GL_LUMINANCE GL_LUMINANCE_ALPHA
			GL_UNSIGNED_BYTE, //type , GL_FLOAT
			pixels); //const void *pixels

		delete image;
	}

	void Square::initVBO()
	{
		glGenBuffers(1, &squareVBO);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, squareVBO); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, sizeof(SquareShaderAttributes), &squareAttributes, GL_STATIC_DRAW);  //insere os dados no buffer para usar glDraw*

		projectionViewLocation = glGetUniformLocation(programShader, "projectionView");
		modelLocation = glGetUniformLocation(programShader, "model");

		positionAttribute = glGetAttribLocation(programShader, "Position");
		colorAttribute = glGetAttribLocation(programShader, "Color");
		textureAttribute = glGetAttribLocation(programShader, "Texture");

		setUpPositionAttribute();
		setUpColorAttribute();
		setUpTextureAttribute();

		setUpImage("resources/images/sprite.png");
	}

	void Square::render(Mat4f projectionViewMatrix)
	{
		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, projectionViewMatrix);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model);

		setUpPositionAttribute();
		setUpColorAttribute();
		setUpTextureAttribute();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	Square::~Square()
	{
		if (programShader != 0)
			glDeleteProgram(programShader);

		if (squareVBO != 0)
			glDeleteBuffers(1, &squareVBO);

		if (texture != 0 && glIsTexture(texture))
			glDeleteTextures(1, &texture);
	}
}