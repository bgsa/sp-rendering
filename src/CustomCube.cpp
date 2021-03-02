#include "CustomCube.h"

namespace NAMESPACE_RENDERING
{
	void CustomCube::init()
	{
		//*   *********** PHONG SHADING (ILUMINATION) ****************
		const char* vertexShaderSource = "#version 300 es \n"
			"in  vec3 Position; \n"
			"in  vec3 Normal; \n"
			"in  vec2 Texture; \n"

			"uniform mat4 projectionView; \n"
			"uniform mat4 model;          \n"
			"uniform mat3 normalMatrix;          \n"

			"out vec3 normalCoord, eyeCoord; \n"
			"out vec2 fragmentTexture; \n"

			"void main() \n"
			"{											\n"
			//"   mat4 normalMatrix = transpose(inverse(model));"
			"	gl_Position = projectionView * model * vec4(Position, 1.0); \n"

			"	fragmentTexture = Texture; \n"

			"	normalCoord = normalMatrix * Normal;  \n"
			"	eyeCoord = vec3(model * vec4(Position, 1));  \n"
			"}											\n";

		const char* fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"

			"in vec3 normalCoord; \n"
			"in vec3 eyeCoord; \n"
			"in vec2 fragmentTexture; \n"

			"uniform vec3 MaterialAmbient, MaterialSpecular, MaterialDiffuse; \n"
			"uniform vec3 LightAmbient, LightSpecular, LightDiffuse; \n"
			"uniform float ShininessFactor; \n"

			"uniform vec3 LightPosition; \n"

			"uniform sampler2D tex; \n"

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
			"{ \n"
			"	FragColor = texture(tex, fragmentTexture) * PhongShading(); \n"
			"}";

		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		initVBO();
	}

	void CustomCube::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0);

		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	void CustomCube::setUpNormalAttribute()
	{
		glVertexAttribPointer(normalAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(sizeof(attributes.position)) // deslocamento do primeiro elemento
		);

		glEnableVertexAttribArray(normalAttribute); //habilita atributo de coordenadas
	}

	void CustomCube::setUpTextureAttribute()
	{
		glVertexAttribPointer(textureAttribute,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(sizeof(attributes.position) + sizeof(attributes.normal)) // deslocamento do primeiro elemento
		);

		glEnableVertexAttribArray(textureAttribute); //habilita atributo de coordenadas
	}

	void CustomCube::initVBO()
	{
		glGenBuffers(1, &cubeVBO);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, sizeof(CustomCubeAttributes), &attributes, GL_STATIC_DRAW);  //insere os dados no buffer para usar glDraw*

		projectionViewLocation = glGetUniformLocation(programShader, "projectionView");
		modelLocation = glGetUniformLocation(programShader, "model");
		normalMatrixLocation = glGetUniformLocation(programShader, "normalMatrix");

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
		textureAttribute = glGetAttribLocation(programShader, "Texture");

		setUpPositionAttribute();
		setUpNormalAttribute();
		setUpTextureAttribute();

		Image *image = ImageFactory::load((std::string("resources/images/") + textureFilename).c_str());
		sp_uchar* pixels = image->data();
		GLsizei width = image->width();
		GLsizei height = image->height();
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

	void CustomCube::render(Mat4 projectionViewMatrix)
	{
		Mat3 normalMatrix;
		model.toMat3(normalMatrix);

		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBindTexture(GL_TEXTURE_2D, texture);

		glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, projectionViewMatrix);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model);
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, normalMatrix);

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
		setUpTextureAttribute();

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
	}

	CustomCube::~CustomCube()
	{
		if (programShader != 0)
			glDeleteProgram(programShader);

		if (cubeVBO != 0)
			glDeleteBuffers(1, &cubeVBO);
	}
}