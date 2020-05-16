#include "Line.h"

#include <cstdlib>
#include "Shader.h"
#include "GLConfig.h"

namespace NAMESPACE_RENDERING
{
	Line::Line() {
	}
	Line::Line(Vec3f point1, Vec3f point2) {
		this->point1 = point1;
		this->point2 = point2;
	}

	void Line::setLineWidth(float newLineWidth)
	{
		lineWidth = newLineWidth;
	}

	void Line::init()
	{
		const char* vertexShaderSource = "#version 300 es \n"
			"uniform mat4 projection; \n"
			"uniform mat4 view; \n"
			"uniform mat4 modelView; \n"
			"uniform vec4 color; \n"

			"in  vec3 Position; \n"

			"out vec4 fragmentColor; \n"

			"void main() \n"
			"{											\n"
			"	gl_Position = projection * view * modelView * vec4(Position, 1.0); \n"
			"	fragmentColor = color; \n"
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

	void Line::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0); //Specify that our coordinate data is going into attribute index 0(shaderAttribute), and contains three floats per vertex
		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	void Line::initVBO()
	{
		GLfloat* buffer = new GLfloat[6];
		buffer[0] = point1[0];
		buffer[1] = point1[1];
		buffer[2] = point1[2];
		buffer[3] = point2[0];
		buffer[4] = point2[1];
		buffer[5] = point2[2];

		glGenBuffers(1, &vertexBufferObject);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, buffer, GL_STATIC_DRAW);  //insere os dados no buffer para usar glDraw*
																									// Get the sampler location
		transformMatrixLocation = glGetUniformLocation(programShader, "modelView");
		projectionMatrixLocation = glGetUniformLocation(programShader, "projection");
		viewMatrixLocation = glGetUniformLocation(programShader, "view");
		positionAttribute = glGetAttribLocation(programShader, "Position");
		colorLocation = glGetUniformLocation(programShader, "color");

		setUpPositionAttribute();
	}

	void Line::render(RenderData renderData)
	{
		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, renderData.projectionMatrix);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, renderData.viewMatrix);
		glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, transform.toMat4());
		glUniform4f(colorLocation, color->Red, color->Green, color->Blue, color->Alpha);

		setUpPositionAttribute();

		float currentLineWidth = GLConfig::getGLfloat(GL_LINE_WIDTH);
		glLineWidth(lineWidth);

		glDrawArrays(GL_LINES, 0, 2);

		glLineWidth(currentLineWidth);
	}
}