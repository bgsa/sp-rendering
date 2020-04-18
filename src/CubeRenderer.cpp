#include <cstdlib>
#include <vector>

#include "Shader.h"
#include "CubeRenderer.h"
#include "Cube.h"

namespace NAMESPACE_RENDERING
{
	void CubeRenderer::init()
	{
		const char* vertexShaderSource = "#version 300 es \n"
			"in  vec3 Position; \n"
			"in  vec3 Normal; \n"

			"uniform mat4 projectionMatrix; \n"
			"uniform mat4 viewMatrix; \n"
			"uniform samplerBuffer modelViewAsTexture;  \n"

			"out vec3 normalCoord, eyeCoord; \n"

			"void main() \n"
			"{											\n"
			"	vec4 col1 = texelFetch(modelViewAsTexture, gl_InstanceID * 4);      \n"
			"	vec4 col2 = texelFetch(modelViewAsTexture, gl_InstanceID * 4 + 1);  \n"
			"	vec4 col3 = texelFetch(modelViewAsTexture, gl_InstanceID * 4 + 2);  \n"
			"	vec4 col4 = texelFetch(modelViewAsTexture, gl_InstanceID * 4 + 3);  \n"
			"	mat4 modelMatrix = mat4(col1, col2, col3, col4);                    \n"

			"	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(Position, 1.0); \n"

			"	normalCoord = mat3(modelMatrix) * Normal;  \n"
			"	eyeCoord = vec3(modelMatrix * vec4(Position, 1));  \n"
			"}											\n";


		//*   *********** PHONG SHADING (ILUMINATION) ****************
		const char* fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"

			"in vec3 normalCoord; \n"
			"in vec3 eyeCoord; \n"

			"uniform vec3 MaterialAmbient, MaterialSpecular, MaterialDiffuse; \n"
			"uniform vec3 LightAmbient, LightSpecular, LightDiffuse; \n"
			"uniform float ShininessFactor; \n"

			"uniform vec3 LightPosition; \n"

			"out vec4 FragColor; \n"

			"vec4 PhongShading() \n"
			"{ \n"
			"	vec3 normalizeNormal = normalize(normalCoord); \n"
			"	vec3 normalizeLightVec = normalize(LightPosition - eyeCoord); \n" //vector viewer

			"	float cosAngle = max(0.0, dot(normalizeNormal, normalizeLightVec)); \n" // Diffuse Intensity

			"	vec3 R = normalize(-normalizeLightVec + normalize(-eyeCoord));       \n" //faster

			"	float specularIntensity = pow(max(0.0, dot(R, -normalizeLightVec)), ShininessFactor); \n"  // Specular Intensity

			"	vec3 ambient = MaterialAmbient * LightAmbient; \n"
			"	vec3 diffuse = cosAngle * (MaterialDiffuse * LightDiffuse); \n"
			"	vec3 specular = specularIntensity * (MaterialSpecular * LightSpecular); \n"

			"	return vec4(ambient + diffuse + specular, 1.0); \n"
			"} \n"

			"void main() \n"
			"{\n"
			"	FragColor = PhongShading(); \n"
			"}";

		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		initVBO();
	}

	void CubeRenderer::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0);

		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	void CubeRenderer::setUpNormalAttribute()
	{
		glVertexAttribPointer(normalAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(sizeof(cubeAttributes.position)) // deslocamento do primeiro elemento
		);

		glEnableVertexAttribArray(normalAttribute); //habilita atributo de coordenadas
	}

	void CubeRenderer::initVBO()
	{
		glGenBuffers(1, &vertexBufferObject);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, sizeof(CubeShaderAttributes), &cubeAttributes, GL_STATIC_DRAW);  //insere os dados no buffer para usar glDraw*

		projectionMatrixLocation = glGetUniformLocation(programShader, "projectionMatrix");
		viewMatrixLocation = glGetUniformLocation(programShader, "viewMatrix");

		materialAmbientLocation = glGetUniformLocation(programShader, "MaterialAmbient");
		materialDiffuseLocation = glGetUniformLocation(programShader, "MaterialDiffuse");
		materialSpecularLocation = glGetUniformLocation(programShader, "MaterialSpecular");

		lightPositionLocation = glGetUniformLocation(programShader, "LightPosition");
		lightAmbientLocation = glGetUniformLocation(programShader, "LightAmbient");
		lightDiffuseLocation = glGetUniformLocation(programShader, "LightDiffuse");
		lightSpecularLocation = glGetUniformLocation(programShader, "LightSpecular");
		lightShininessFactorLocation = glGetUniformLocation(programShader, "ShininessFactor");

		positionAttribute = glGetAttribLocation(programShader, "Position");
		normalAttribute = glGetAttribLocation(programShader, "Normal");

		setUpPositionAttribute();
		setUpNormalAttribute();

		glGenBuffers(1, &modelBufferObject);
		glBindBuffer(GL_TEXTURE_BUFFER, modelBufferObject);

		glGenTextures(1, &modelMatrixTBO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER, modelMatrixTBO);
	}

	void CubeRenderer::beforeRender(const RenderData& renderData, Cube* cubes, size_t cubesCount)
	{
		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		glUniform3f(materialAmbientLocation, materialAmbient.x, materialAmbient.y, materialAmbient.z);
		glUniform3f(materialDiffuseLocation, materialDiffuse.x, materialDiffuse.y, materialDiffuse.z);
		glUniform3f(materialSpecularLocation, materialSpecular.x, materialSpecular.y, materialSpecular.z);

		glUniform3f(lightPositionLocation, lightPosition.x, lightPosition.y, lightPosition.z);
		glUniform3f(lightAmbientLocation, lightAmbient.x, lightAmbient.y, lightAmbient.z);
		glUniform3f(lightDiffuseLocation, lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
		glUniform3f(lightSpecularLocation, lightSpecular.x, lightSpecular.y, lightSpecular.z);
		glUniform1f(lightShininessFactorLocation, shininessFactor);

		setUpPositionAttribute();
		setUpNormalAttribute();

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, renderData.projectionMatrix);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, renderData.viewMatrix);

		cubes[1].rotate(degreesToRadians(3), 0.0f, 0.0f, 1.0f);

		float* modelMatrixes = ALLOC_ARRAY(float, cubesCount * MAT4_SIZE);
		for (size_t index = 0; index < cubesCount; index++)
			for (size_t i = 0; i < MAT4_SIZE; i++)
				modelMatrixes[(index * MAT4_SIZE) + i] = cubes[index].modelView[i];

		glBindBuffer(GL_TEXTURE_BUFFER, modelBufferObject);
		glBufferData(GL_TEXTURE_BUFFER, sizeof(Mat4f) * cubesCount, modelMatrixes, GL_DYNAMIC_DRAW);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_BUFFER, modelMatrixTBO);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, modelBufferObject);

		ALLOC_RELEASE(modelMatrixes);
	}

	void CubeRenderer::render(size_t cubesCount)
	{
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices, cubesCount);
	}
}