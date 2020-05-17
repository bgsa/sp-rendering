/*
#pragma once

#include "SpectrumRendering.h"
#include "ModelFile.h"

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define MAX_BONES_PER_MODEL 255
#define MAX_BONES_PER_VERTEX 4

namespace NAMESPACE_RENDERING
{
	struct CustomModelAttributes {
		GLfloat* position;
		GLfloat* normal;
		GLfloat* texture;
	};

	struct BoneInfo
	{
		std::string Name;
		Mat4 BoneOffset = Mat4();
		Mat4 FinalTransformation = Mat4();
	};

	class CustomModel
	{
	private:
		GLushort* faces;

		Mat4 model = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		GLuint programShader;
		GLuint modelVBO;
		GLuint boneIndexVBO;
		GLuint boneWeightVBO;

		GLint modelLocation;
		GLint projectionViewLocation;
		GLint normalMatrixLocation;
		GLint bonesOffsetLocation[MAX_BONES_PER_MODEL];

		Mat4* bonesTransform;

		GLint positionAttribute;
		GLint normalAttribute;
		GLint textureAttribute;
		GLint bonesIndexAttribute;
		GLint bonesWeightAttribute;

		GLuint texture;
		std::vector<BoneInfo> m_BoneInfo;

		GLint materialAmbientLocation;
		GLint materialDiffuseLocation;
		GLint materialSpecularLocation;

		GLint lightPositionLocation;
		GLint lightAmbientLocation;
		GLint lightDiffuseLocation;
		GLint lightSpecularLocation;

		GLint lightShininessFactorLocation;

		GLint hasTextureLocation;

		Vec3 materialAmbient;
		Vec3 materialDiffuse;
		Vec3 materialSpecular;
		float shininessFactor;

		Vec3 lightPosition = { -300.0f, 1000.0f, 300.0f };
		Vec3 lightAmbient = { 0.5f, 0.5f, 0.5f };
		Vec3 lightDiffuse = { 1.0f, 1.0f, 1.0f };
		Vec3 lightSpecular = { 1.0f, 1.0f, 1.0f };

		unsigned int vertexCount;
		unsigned int textureCount;
		unsigned int facesCount;
		unsigned int bonesCount;
		bool hasTexture = false;
		bool hasBones = false;
		std::string textureFilename = "Character_Man.png";

		void initVBO(std::string filename);
		void setUpPositionAttribute();
		void setUpNormalAttribute();
		void setUpTextureAttribute();

		void setUpIndexBoneAttribute();
		void setUpWeightBoneAttribute();

		int FindBoneIndexByName(std::string boneName);
		const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
		unsigned int FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, Mat4 ParentTransform, const aiScene* m_pscene);
		Mat4* getBonesTransform(const aiScene* scene);

	public:
		Mat4 boneGlobalInverseTransform;

		void translate(float x, float y, float z);
		void scale(float scaleFactorUniform);
		void scale(float scaleX, float scaleY, float scaleZ);
		void rotate(float angleInRadians, float x, float y, float z);

		void setLightAmbient(Vec3 color);

		void init(std::string filename);

		void render(Mat4 projectionViewMatrix, const aiScene* scene);

		~CustomModel();
	};
}
*/