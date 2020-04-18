#include <cstdlib>

#include "Shader.h"
#include "GridSystem.h"
#include "GLConfig.h"

namespace NAMESPACE_RENDERING
{
	GridSystemShaderAttributes gridSystemAttributes = GridSystemShaderAttributes{
		{ 0.f, 0.f, -10.0f },
		{ 0.f, 0.f, 10.0f }
	};

	void GridSystem::setLineWidth(float newLineWidth)
	{
		lineWidth = newLineWidth;
	}

	void GridSystem::init()
	{
		const char* vertexShaderSource = "#version 300 es \n"
			"uniform mat4 projection; \n"
			"uniform mat4 view; \n"
			"uniform mat4 modelView; \n"
			"uniform vec4 Color; \n"

			"in  vec3 Position; \n"

			"void main() \n"
			"{											\n"
			"	gl_Position = projection * view * modelView * vec4(Position, 1.0); \n"
			"}											\n";

		const char* geometryShaderSource = "#version 330 \n"
			"#extension GL_EXT_geometry_shader : enable \n"

			"layout(lines) in; \n"
			"layout(line_strip, max_vertices = 2) out; \n"

			"void main() { \n"
			"	gl_Position = gl_in[0].gl_Position;   \n"
			"	EmitVertex();   \n"

			"	gl_Position = gl_in[1].gl_Position;   \n"
			"	EmitVertex();   \n"

			"	EndPrimitive();   \n"
			"}";

		const char* fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"

			"uniform vec4 Color; \n"

			"out vec4 FragColor; \n"

			"void main() \n"
			"{\n"
			"	FragColor = Color; \n"
			"}";

		this->attributes = gridSystemAttributes;

		//programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);
		programShader = Shader::loadShaderProgramWithGeometry(vertexShaderSource, geometryShaderSource, fragmentShaderSource);

		initVBO();
	}

	void GridSystem::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0); //Specify that our coordinate data is going into attribute index 0(shaderAttribute), and contains three floats per vertex
		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	void GridSystem::initVBO()
	{
		glGenBuffers(1, &vertexBufferObject);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, sizeof(GridSystemShaderAttributes), &attributes, GL_STATIC_DRAW);  //insere os dados no buffer para usar glDraw*

		projectionMatrixLocation = glGetUniformLocation(programShader, "projection");
		viewMatrixLocation = glGetUniformLocation(programShader, "view");
		modelViewLocation = glGetUniformLocation(programShader, "modelView");

		colorLocation = glGetUniformLocation(programShader, "Color");
		positionAttribute = glGetAttribLocation(programShader, "Position");

		setUpPositionAttribute();
	}

	void GridSystem::render(const RenderData& renderData)
	{
		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, renderData.projectionMatrix);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, renderData.viewMatrix);
		glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, modelView);

		glUniform4f(colorLocation, color->Red, color->Green, color->Blue, color->Alpha);

		setUpPositionAttribute();

		float currentLineWidth = GLConfig::getGLfloat(GL_LINE_WIDTH);
		glLineWidth(lineWidth);

		float quantidade = 10.0f;
		Mat4f matrixRotate = Mat4f::createRotate((float)degreesToRadians(90), 0.0f, 1.0f, 0.0f);
		//float spacing = 1.0f;
		//Mat4f scaleMatrix = Mat4f::createScale(spacing, 0.0f, spacing);

		for (float i = -quantidade; i < quantidade + 1; i++)
		{
			//draw lines on Z axis
			Mat4f matrixTranslatedX = Mat4f::createTranslate(i, 0.0f, 0.0f);

			Mat4f result = modelView * matrixTranslatedX;

			glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, result);
			glDrawArrays(GL_LINES, 0, 2);

			//draw lines on X axis		
			Mat4f matrixTranslatedZ = Mat4f::createTranslate(0.0f, 0.0f, i);

			result = matrixRotate * modelView;

			Mat4f result2 = matrixTranslatedZ * result;

			glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, result2);
			glDrawArrays(GL_LINES, 0, 2);
		}


		glLineWidth(currentLineWidth);
	}
}