#ifndef CUBE_RENDERER_HEADER
#define CUBE_RENDERER_HEADER

#include "SpectrumRendering.h"
#include <BoundingVolume.h>
#include "GraphicObject3D.h"

namespace NAMESPACE_RENDERING
{
	class Cube;

	struct CubeShaderAttributes {
		GLfloat position[24];
		GLfloat normal[24];
	};

	static CubeShaderAttributes cubeAttributes = CubeShaderAttributes{
		{ //position  (space coordinates)
			 0.5f, -0.5f, -0.5f,  //left-bottom-front
			-0.5f, -0.5f, -0.5f,  //right-bottom-front
			-0.5f,  0.5f, -0.5f,  //right-top-front
			 0.5f,  0.5f, -0.5f,  //left-top-front

			-0.5f, -0.5f,  0.5f,   //left-bottom-back
			 0.5f, -0.5f,  0.5f,   //right-bottom-back
			 0.5f,  0.5f,  0.5f,   //right-top-back
			-0.5f,  0.5f,  0.5f    //left-top-back
		},
		{ //normal surface
			 0.577350259f,  -0.577350259f,  -0.577350259f,   //bottom-right-back
			-0.577350259f,  -0.577350259f,  -0.577350259f    //bottom-left-back
			- 0.577350259f,   0.577350259f,  -0.577350259f,   //top-left-back
			 0.577350259f,   0.577350259f,  -0.577350259f,   //top-right-back
			-0.577350259f,  -0.577350259f,   0.577350259f,   //bottom-left-front
			 0.577350259f,  -0.577350259f,   0.577350259f,   //bottom-right-front
			 0.577350259f,   0.577350259f,   0.577350259f,   //top-right-front
			-0.577350259f,   0.577350259f,   0.577350259f,   //top-left-front
		}
	};

	const GLubyte cubeIndices[36] = {
		0,1,2, //face frontal
		2,3,0,
		4,5,6, //face-traseira
		6,7,4,
		0,5,4, //fundo
		4,1,0,
		3,2,7, //topo
		7,6,3,
		6,5,0,
		0,3,6,
		1,4,7, //face direita
		7,2,1
	};

	class CubeRenderer //: public GraphicObject3D
	{
	private:

		GLuint programShader;
		GLuint vertexBufferObject;
		GLuint modelBufferObject;

		GLint projectionMatrixLocation;
		GLint viewMatrixLocation;

		GLint positionAttribute = -1;
		GLint normalAttribute;

		GLuint modelMatrixTBO;

		GLint materialAmbientLocation;
		GLint materialDiffuseLocation;
		GLint materialSpecularLocation;

		GLint lightPositionLocation;
		GLint lightAmbientLocation;
		GLint lightDiffuseLocation;
		GLint lightSpecularLocation;

		GLint lightShininessFactorLocation;

		Vec3f materialAmbient = { 1.0f, 0.0f, 0.0f };
		Vec3f materialDiffuse = { 1.0f, 1.0f, 1.0f };
		Vec3f materialSpecular = { 1.0f, 1.0f, 1.0f };

		Vec3f lightPosition = { -300.0f, 1000.0f, 300.0f };
		Vec3f lightAmbient = { 1.0f, 1.0f, 1.0f };
		Vec3f lightDiffuse = { 1.0f, 1.0f, 1.0f };
		Vec3f lightSpecular = { 1.0f, 1.0f, 1.0f };
		float shininessFactor = 1000.0f;

		void initVBO();
		void setUpPositionAttribute();
		void setUpNormalAttribute();

	public:

		API_INTERFACE void init();

		API_INTERFACE void beforeRender(const RenderData& renderData, Cube* cubes, size_t cubesCount);
		API_INTERFACE void render(size_t cubesCount);

		API_INTERFACE std::string toString()
		{
			return "Cube";
		}

	};
}

#endif // CUBE_RENDERER_HEADER