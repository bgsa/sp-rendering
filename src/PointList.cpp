#include <cstdlib>

#include "Shader.h"
#include "PointList.h"

namespace NAMESPACE_RENDERING
{
	GLfloat* PointListStruct::toArray()
	{
		GLfloat * floats = new GLfloat[points.size() + colors.size()];

		for (size_t i = 0; i < points.size(); i++)
			floats[i] = points[i];

		for (size_t i = 0; i < colors.size(); i++)
			floats[i + points.size()] = colors[i];

		return floats;
	}

	size_t PointListStruct::getArraySize()
	{
		return (points.size() * sizeof(GLfloat)) + (colors.size() * sizeof(GLfloat));
	}

	size_t PointListStruct::getPointCount()
	{
		return points.size() / 3;
	}

	void PointListStruct::setDefaultColor(GLfloat* color4f)
	{
		defaultColor[0] = color4f[0];
		defaultColor[1] = color4f[1];
		defaultColor[2] = color4f[2];
		defaultColor[3] = color4f[3];
	}

	void PointListStruct::addPoint(float x, float y, float z, float redColorFactor, float greenColorFactor, float blueColorFactor, float alphaFactor)
	{
		points.push_back(x);
		points.push_back(y);
		points.push_back(z);

		colors.push_back(redColorFactor);
		colors.push_back(greenColorFactor);
		colors.push_back(blueColorFactor);
		colors.push_back(alphaFactor);
	}

	void PointListStruct::addPoint(float x, float y, float z)
	{
		points.push_back(x);
		points.push_back(y);
		points.push_back(z);

		colors.push_back(defaultColor[0]);
		colors.push_back(defaultColor[1]);
		colors.push_back(defaultColor[2]);
		colors.push_back(defaultColor[3]);
	}


	void PointList::setPointSize(float newPointSize)
	{
		pointSize = newPointSize;
	}

	void PointList::init(PointListStruct attributes)
	{
		this->attributes = attributes;

		const char* vertexShaderSource = "#version 300 es \n"
			"uniform mat4 modelView; \n"
			"uniform mat4 projectionView; \n"
			"uniform float pointSize; \n"

			"in  vec3 Position; \n"
			"in  vec4 Color; \n"

			"out vec4 fragmentColor; \n"

			"void main() \n"
			"{											\n"
			"	gl_Position = projectionView * modelView * vec4(Position, 1.0); \n"
			"   gl_PointSize = pointSize;   \n"
			"	fragmentColor = Color; \n"
			"}											\n";

		const char* fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"
			"in  vec4 fragmentColor; \n"
			"out vec4 FragColor; \n"

			"void main() \n"
			"{\n"
			"	FragColor = fragmentColor; \n"
			"}";


		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		initVBO();
	}

	void PointList::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0); //Specify that our coordinate data is going into attribute index 0(shaderAttribute), and contains three floats per vertex
		glEnableVertexAttribArray(positionAttribute); //habilita atributo de posi��o
	}

	void PointList::setUpColorAttribute()
	{
		glVertexAttribPointer(colorAttribute,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(3 * attributes.getPointCount() * sizeof(GLfloat)) // deslocamento do primeiro elemento
		);
		glEnableVertexAttribArray(colorAttribute); //habilita atributo de cor
	}

	void PointList::initVBO()
	{
		GLfloat * buffer = attributes.toArray();

		glGenBuffers(1, &pointVBO);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, pointVBO); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, attributes.getArraySize(), buffer, GL_STATIC_DRAW);  //insere os dados no buffer para usar glDraw*

		modelViewLocation = glGetUniformLocation(programShader, "modelView");
		projectionViewLocation = glGetUniformLocation(programShader, "projectionView");
		positionAttribute = glGetAttribLocation(programShader, "Position");
		colorAttribute = glGetAttribLocation(programShader, "Color");
		pointSizeLocation = glGetUniformLocation(programShader, "pointSize");

		setUpPositionAttribute();
		setUpColorAttribute();

		delete[] buffer;
	}

	void PointList::render(Mat4f projectionViewMatrix)
	{
		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, pointVBO);

		glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelView);
		glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, projectionViewMatrix);

		glUniform1fv(pointSizeLocation, 1, &pointSize);

		setUpPositionAttribute();
		setUpColorAttribute();

		glDrawArrays(GL_POINTS, 0, attributes.getPointCount());
	}

	PointList::~PointList()
	{
		if (programShader != 0)
			glDeleteProgram(programShader);

		if (pointVBO != 0)
			glDeleteBuffers(1, &pointVBO);
	}
}