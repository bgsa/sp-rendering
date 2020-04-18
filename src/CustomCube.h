#pragma once

#include "SpectrumRendering.h"
#include "Shader.h"
#include <cstdlib>
#include <vector>
#include <Factory.h>
#include <IFileManager.h>
#include <Image.h>
#include <ImageFactory.h>

namespace NAMESPACE_RENDERING
{
	struct CustomCubeAttributes {
		GLfloat position[72];
		GLfloat normal[72];
		GLfloat texture[48];
	};

	class CustomCube
	{
	private:
		CustomCubeAttributes attributes = CustomCubeAttributes{
			{   -0.812913f, -0.034892f, 1.67276f, -0.812913f, 1.95399f, -0.327244f, -0.812913f, -0.046006f, -0.327244f, -0.812913f, -0.046006f, -0.327244f, 1.15029f, 1.90484f, -0.292759f, 1.15029f, -0.095163f, -0.292759f, 1.15029f, 1.90484f, -0.292759f, 1.15029f, -0.084049f, 1.70724f, 1.15029f, -0.095163f, -0.292759f, 1.15029f, 1.91595f, 1.70724f, -0.812913f, -0.034892f, 1.67276f, 1.15029f, -0.084049f, 1.70724f, 1.15029f, -0.095163f, -0.292759f, -0.812913f, -0.034892f, 1.67276f, -0.812913f, -0.046006f, -0.327244f, 1.15029f, 1.90484f, -0.292759f, -0.812913f, 1.96511f, 1.67276f, 1.15029f, 1.91595f, 1.70724f, -0.812913f, 1.96511f, 1.67276f, -0.812913f, 1.95399f, -0.327244f, 1.15029f, 1.91595f, 1.70724f, -0.812913f, 1.96511f, 1.67276f, 1.15029f, -0.084049f, 1.70724f, -0.812913f, 1.95399f, -0.327244f  },
			{   -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0175629f, 0.0f, -0.999846f, 0.0175629f, 0.0f, -0.999846f, 0.0175629f, 0.0f, -0.999846f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -0.017563f, 0.0f, 0.999846f, -0.017563f, 0.0f, 0.999846f, -0.017563f, 0.0f, 0.999846f, -0.0251284f, -0.999669f, 0.00555516f, -0.0251284f, -0.999669f, 0.00555516f, -0.0251284f, -0.999669f, 0.00555516f, 0.0251284f, 0.999669f, -0.00555513f, 0.0251284f, 0.999669f, -0.00555513f, 0.0251284f, 0.999669f, -0.00555516f, -1.0f, 0.0f, 0.0f, 0.0175629f, 0.0f, -0.999846f, 1.0f, 0.0f, 0.0f, -0.017563f, 0.0f, 0.999846f, -0.0251284f, -0.999669f, 0.00555516f, 0.0251284f, 0.999669f, -0.0055551f   },
			{   0.994345f, 0.994369f,
	0.0001f, 0.005631f,
	0.994338f, 0.0001f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.994376f, 0.994345f,
	0.005624f, 0.0001f,
	0.9999f, 0.0001f,
	0.958117f, 0.975846f,
	9.8e-05f, 0.024154f,
	0.958041f, 9.8e-05f,
	0.983088f, 0.965294f,
	0.016912f, 9.8e-05f,
	0.999902f, 9.8e-05f,
	9.8e-05f, 0.016912f,
	0.965294f, 0.983088f,
	9.8e-05f, 0.999902f,
	0.000106f, 0.9999f,
	1.0f, 0.0f,
	0.0001f, 0.994346f,
	0.000173f, 0.999902f,
	9.8e-05f, 0.965294f,
	0.965294f, 9.8e-05f   }
		};

		GLubyte indices[36] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 0, 18, 1, 3, 19, 4, 6, 20, 7, 9, 21, 10, 12, 22, 13, 15, 23, 16 };

		Mat4f model = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};

		GLuint programShader;
		GLuint cubeVBO;

		GLint modelLocation;
		GLint projectionViewLocation;
		GLint normalMatrixLocation;

		GLint positionAttribute;
		GLint normalAttribute;
		GLint textureAttribute;
		GLuint texture;

		GLint materialAmbientLocation;
		GLint materialDiffuseLocation;
		GLint materialSpecularLocation;

		GLint lightPositionLocation;
		GLint lightAmbientLocation;
		GLint lightDiffuseLocation;
		GLint lightSpecularLocation;

		GLint lightShininessFactorLocation;

		Vec3f materialAmbient = { 1.0f, 1.0f, 1.0f };
		Vec3f materialDiffuse = { 0.171313f, 0.17322f, 0.64f };
		Vec3f materialSpecular = { 0.518519f, 0.441559f, 0.427946f };
		float shininessFactor = 96.0784f;

		Vec3f lightPosition = { -300.0f, 1000.0f, 300.0f };
		Vec3f lightAmbient = { 0.3f, 0.3f, 0.3f };
		Vec3f lightDiffuse = { 1.0f, 1.0f, 1.0f };
		Vec3f lightSpecular = { 1.0f, 1.0f, 1.0f };

		std::string textureFilename = "devops.png";

		void initVBO();
		void setUpPositionAttribute();
		void setUpNormalAttribute();
		void setUpTextureAttribute();

	public:
		~CustomCube();

		void init();

		void render(Mat4f projectionViewMatrix);
	};
}
