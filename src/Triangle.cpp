#include "Triangle.h"

namespace NAMESPACE_RENDERING
{
	ShaderAttributes Triangle::getAttributes() {
		return attributes;
	}

	void Triangle::setColor(GLfloat newColor[12])
	{
		void *buffer = glMapBufferRange(GL_ARRAY_BUFFER,
			sizeof(attributes.position),  //início do buffer que deseja mapear
			sizeof(attributes.color),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		//atualiza o buffer
		GLfloat *mapped = reinterpret_cast<GLfloat *>(buffer);
		for (size_t i = 0; i < 12; i++)
		{
			*(mapped + i) = newColor[i];
			attributes.color[i] = newColor[i];
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void Triangle::init(ShaderAttributes attributes)
	{
		/*
		IFileManager * fileManager = Factory::getFileManagerInstance();

		char * filename = "resources/shaders/default.vert";
		const char * vertexShaderSource = fileManager->readTextFile(filename);

		filename = "resources/shaders/default.frag";
		const char * fragmentShaderSource = fileManager->readTextFile(filename);

		delete fileManager;
		*/

		const char* vertexShaderSource = "#version 300 es \n"
			"in  vec3 Position; \n"
			"in  vec4 Color; \n"
			"out vec4 f_Color; \n"
			"void main() \n"
			"{ \n"
			"gl_Position = vec4(Position, 1.0); \n"
			"f_Color = Color; \n"
			"} \n";

		const char* fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"
			"in  vec4 f_Color; \n"
			"out vec4 fragmentColor; \n"
			"void main() \n"
			"{ \n"
			"	fragmentColor = f_Color; \n"
			"}";


		init(attributes, vertexShaderSource, fragmentShaderSource);
	}

	void Triangle::init(ShaderAttributes attributes, const char * vertexShaderSource, const char * fragmentShaderSource)
	{
		this->attributes = attributes;

		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		initVBO();
	}

	void Triangle::setUpPositionAttribute()
	{
		glVertexAttribPointer(
			positionAttribute,
			//sizeof(attributes.position) / sizeof(GLfloat) / 3,  
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0); //Specify that our coordinate data is going into attribute index 0(shaderAttribute), and contains three floats per vertex
		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	void Triangle::setUpColorAttribute()
	{
		glVertexAttribPointer(
			colorAttribute,    // atributos
			//sizeof(attributes.color) / sizeof(GLfloat) / 3,		//valor=4 - numero de elemenos por vértice, aqui é (r,g,b,a)
			4,
			GL_FLOAT,		   // o tipo de cada elementos
			GL_FALSE,          // estado dos nossos valores.
			0,		           // sem dados extras em cada posição.
			(void*) sizeof(attributes.position) // deslocamento do primeiro elemento
		);
		glEnableVertexAttribArray(colorAttribute);
	}

	void Triangle::initVBO()
	{
		glGenBuffers(1, &triangleVBO);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, triangleVBO); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, sizeof(ShaderAttributes), &attributes, GL_STATIC_DRAW);  //insere os dados no buffer para usar glDraw*

		positionAttribute = glGetAttribLocation(programShader, "Position");
		colorAttribute = glGetAttribLocation(programShader, "Color");
	}

	/*
	void Triangle::renderWithoutVBO() {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
		glEnableVertexAttribArray(positionAttribute);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	*/

	void Triangle::render()
	{
		glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

		setUpPositionAttribute();
		setUpColorAttribute();

		glUseProgram(programShader);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	Triangle::Triangle()
	{
	}

	Triangle::~Triangle()
	{
		if (programShader != 0)
		{
			glDeleteProgram(programShader);
			programShader = 0;
		}

		if (triangleVBO != 0) {
			glDeleteBuffers(1, &triangleVBO);
		}
	}
}