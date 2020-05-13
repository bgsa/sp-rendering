#include "SphereRenderer.h"

namespace NAMESPACE_RENDERING
{
	void SphereRenderer::init()
	{
		const char* vertexShaderSource = "#version 300 es \n"
			"uniform mat4 projection; \n"
			"uniform mat4 view; \n"
			"uniform mat4 modelView; \n"
			"uniform float Radius; \n"

			"in vec3 Position; \n"

			"void main() \n"
			"{	\n"
			"	gl_PointSize = Radius * 30.0;"
			"	gl_Position = projection * view * modelView * vec4(Position, 1.0); \n"
			"}											\n";

		const char* fragmentShaderSource = "#version 320 es \n"
			"precision mediump float; \n"

			"uniform vec4 Color; \n"

			"out vec4 FragColor; \n"

			"void main() \n"
			"{ \n"
			"	float distanceByPoints = distance(vec2(0.5, 0.5), gl_PointCoord.xy); \n"

			"	if (distanceByPoints > 0.5)  \n"
			"		discard; \n"
			"	else \n"
			"		FragColor = Color; \n"
			"}";

		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		initVBO();
	}

	void SphereRenderer::setupAttributeLocations()
	{
		glVertexAttribPointer(
			positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
		);
		glEnableVertexAttribArray(positionAttribute);
	}

	void SphereRenderer::initVBO()
	{
		float initialPosition[3] = { 0.0f, 0.0f, 0.0f };

		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, SIZEOF_FLOAT * 3, initialPosition, GL_STATIC_DRAW);

		projectionMatrixLocation = glGetUniformLocation(programShader, "projection");
		viewMatrixLocation = glGetUniformLocation(programShader, "view");
		transformMatrixLocation = glGetUniformLocation(programShader, "modelView");

		positionAttribute = glGetAttribLocation(programShader, "Position");
		radiusLocation = glGetUniformLocation(programShader, "Radius");
		colorLocation = glGetUniformLocation(programShader, "Color");

		setupAttributeLocations();
	}

	void SphereRenderer::render(const RenderData& renderData)
	{
		glUseProgram(programShader);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_POINT_SPRITE);

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, renderData.projectionMatrix);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, renderData.viewMatrix);
		glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, transform);

		glUniform4fv(colorLocation, 1, color->toVec4());
		glUniform1f(radiusLocation, geometry->ray);

		setupAttributeLocations();

		glDrawArrays(GL_POINTS, 0, 1);
	}
}