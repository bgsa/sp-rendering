/*
#include <cstdlib>
#include <vector>

#include "Shader.h"
#include "CustomModel.h"

#include <Factory.h>
#include <IFileManager.h>
#include <Image.h>
#include <ImageFactory.h>

namespace NAMESPACE_RENDERING
{
	void CustomModel::scale(float scaleFactorUniform)
	{
		model.scale(scaleFactorUniform, scaleFactorUniform, scaleFactorUniform);
	}

	void CustomModel::scale(float scaleX, float scaleY, float scaleZ)
	{
		model.scale(scaleX, scaleY, scaleZ);
	}

	void CustomModel::rotate(float angleInRadians, float x, float y, float z)
	{
		model = model * Mat4f::createRotate(angleInRadians, x, y, z);
	}

	void CustomModel::translate(float x, float y, float z)
	{
		model = model * Mat4f::createTranslate(x, y, z);
	}

	void CustomModel::setLightAmbient(Vec3f color)
	{
		lightAmbient = color;
	}

	void CustomModel::init(std::string filename)
	{
		//*   *********** PHONG SHADING (ILUMINATION) ****************
		const char* vertexShaderSource = "#version 300 es \n"
			"in  vec3 Position;      \n"
			"in  vec3 Normal;        \n"
			"in  vec2 Texture;       \n"
			"in ivec4 BonesIndex;    \n"
			"in  vec4 BonesWeight;   \n"

			"uniform mat4 projectionView;          \n"
			"uniform mat4 model;                   \n"
			"uniform mat3 normalMatrix;            \n"
			"uniform mat4 BonesOffSet[255];  \n"   // constant MAX_BONES_PER_MODEL

			"out vec3 normalCoord, eyeCoord;       \n"
			"out vec2 fragmentTexture;             \n"

			"void main() \n"
			"{		     \n"

			/*
			//"	mat4 BoneTransform = BonesOffSet[BonesIndex[0]] * BonesWeight[0];    \n"
			//"	BoneTransform     += BonesOffSet[BonesIndex[1]] * BonesWeight[1];    \n"
			//"	BoneTransform     += BonesOffSet[BonesIndex[2]] * BonesWeight[2];    \n"
			//"	BoneTransform     += BonesOffSet[BonesIndex[3]] * BonesWeight[3];    \n"

			"	float peso1 = BonesWeight[0];    \n"
			"	float peso2 = BonesWeight[1];    \n"
			"	float peso3 = BonesWeight[2];    \n"
			"	float peso4 = BonesWeight[3];    \n"

			//"	if( peso1 == 0.0 );    \n" //peso1 está zero
			//"		peso1 = 0.0001;    \n"

			//"	if( peso2 == 0.0 );    \n" //peso2 está zero
			//"		peso2 = 0.0001;    \n"

			//"	if( peso3 == 0.0 );    \n" //peso3 está zero
			//"		peso3 = 0.0001;    \n"

			//"	if( peso4 == 0.0 );    \n" //peso4 está zero
			//"		peso4 = 0.0001;    \n"

			"	mat4 BoneTransform = BonesOffSet[BonesIndex[0]] * peso1;    \n"
			"	BoneTransform     += BonesOffSet[BonesIndex[1]] * peso2;    \n"
			"	BoneTransform     += BonesOffSet[BonesIndex[2]] * peso3;    \n"
			"	BoneTransform     += BonesOffSet[BonesIndex[3]] * peso4;    \n"

			//"	int index              = BonesIndex[3];    \n"
			//"	int index              = 15;    \n"
			//"	mat4 BoneTransform     = BonesOffSet[index];    \n"

			//"	mat4 BoneTransform = BonesOffSet[BonesIndex[0]];    \n"
			//"	BoneTransform     += BonesOffSet[BonesIndex[1]];    \n"
			//"	BoneTransform     += BonesOffSet[BonesIndex[2]];    \n"
			//"	BoneTransform     += BonesOffSet[BonesIndex[3]];    \n"

			//"	gl_Position = projectionView * model * BoneTransform * vec4(Position, 1.0);        \n"
			

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

			"uniform int hasTexture; \n"
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
			"	if ( hasTexture == 0 )												\n"
			"		FragColor = PhongShading(); \n"
			"	else															\n"
			"		FragColor = texture(tex, fragmentTexture) * PhongShading(); \n"
			"}";

		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		initVBO(filename);
	}

	void CustomModel::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0);

		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	void CustomModel::setUpNormalAttribute()
	{
		glVertexAttribPointer(normalAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(vertexCount * sizeof(GLfloat)) // deslocamento do primeiro elemento
		);

		glEnableVertexAttribArray(normalAttribute);
	}

	void CustomModel::setUpTextureAttribute()
	{
		if (!hasTexture)
			return;

		glVertexAttribPointer(textureAttribute,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)((vertexCount * 2) * sizeof(GLfloat)) // deslocamento do primeiro elemento
		);

		glEnableVertexAttribArray(textureAttribute); //habilita atributo de coordenadas	
	}

	void CustomModel::setUpIndexBoneAttribute()
	{
		glVertexAttribPointer(bonesIndexAttribute,
			4,
			GL_INT,
			GL_FALSE,
			0,
			0);

		glEnableVertexAttribArray(bonesIndexAttribute);
	}

	void CustomModel::setUpWeightBoneAttribute()
	{
		glVertexAttribPointer(bonesWeightAttribute,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			//(void*)((vertexCount/3) * 4 * sizeof(GLint)) // bone index count * 4 bones
			0
		);

		glEnableVertexAttribArray(bonesWeightAttribute);
	}

	void CustomModel::initVBO(std::string filename)
	{
		ModelFile modelFile;
		ModelFileHeader header = modelFile.load(filename);

		vertexCount = header.VertexCount;
		textureCount = header.TextureUVCount;
		facesCount = header.FacesCount;
		hasTexture = header.TextureUVCount > 0;
		bonesCount = header.BonesCount;
		hasBones = header.BonesCount > 0;
		hasBones = false;

		materialAmbient = header.MaterialAmbient;
		materialDiffuse = header.MaterialDiffuse;
		materialSpecular = header.MaterialSpecular;
		shininessFactor = header.ShininessFactor;

		faces = modelFile.getFaces();
		GLfloat* fullAttributes = modelFile.getPositionNormalsTexture();

		glGenBuffers(1, &modelVBO);
		glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
		glBufferData(GL_ARRAY_BUFFER, ((vertexCount * 2) + textureCount) * sizeof(GLfloat), fullAttributes, GL_STATIC_DRAW);
		delete[] fullAttributes;

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

		hasTextureLocation = glGetUniformLocation(programShader, "hasTexture");

		positionAttribute = glGetAttribLocation(programShader, "Position");
		normalAttribute = glGetAttribLocation(programShader, "Normal");
		textureAttribute = glGetAttribLocation(programShader, "Texture");

		if (hasBones)
		{
			std::vector<std::string> bonesName = modelFile.getBonesName();
			Mat4f* bonesOffset = modelFile.getBonesOffset();
			m_BoneInfo.resize(bonesCount);
			for (size_t i = 0; i < bonesCount; i++) {
				m_BoneInfo[i].BoneOffset = bonesOffset[i];
				m_BoneInfo[i].Name = bonesName[i];
			}
			delete[] bonesOffset;

			GLint* bonesIndexesAttributes = modelFile.getBonesIndexes();
			glGenBuffers(1, &boneIndexVBO);
			glBindBuffer(GL_ARRAY_BUFFER, boneIndexVBO);
			glBufferData(GL_ARRAY_BUFFER, modelFile.getBonesIndexesSize(), bonesIndexesAttributes, GL_STATIC_DRAW);
			delete[] bonesIndexesAttributes;

			GLfloat* bonesWeightsAttributes = modelFile.getBonesWeights();
			glGenBuffers(1, &boneWeightVBO);
			glBindBuffer(GL_ARRAY_BUFFER, boneWeightVBO);
			glBufferData(GL_ARRAY_BUFFER, modelFile.getBonesWeightsSize(), bonesWeightsAttributes, GL_STATIC_DRAW);
			delete[] bonesWeightsAttributes;

			for (size_t i = 0; i < bonesCount; i++)
			{
				std::string uniformVariableName = "BonesOffSet[" + StringHelper::toString(i) + "]";
				bonesOffsetLocation[i] = glGetUniformLocation(programShader, uniformVariableName.c_str());
			}

			bonesIndexAttribute = glGetAttribLocation(programShader, "BonesIndex");
			bonesWeightAttribute = glGetAttribLocation(programShader, "BonesWeight");
		}

		if (hasTexture)
		{
			Image *image = ImageFactory::load((std::string("resources/images/") + textureFilename).c_str());
			unsigned char* pixels = image->getData();
			GLsizei width = image->getWidth();
			GLsizei height = image->getHeight();
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
	}


	const aiNodeAnim* CustomModel::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
	{
		for (size_t i = 0; i < pAnimation->mNumChannels; i++) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

			if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
				return pNodeAnim;
			}
		}

		return NULL;
	}

	int CustomModel::FindBoneIndexByName(std::string boneName)
	{
		for (int i = 0; i < m_BoneInfo.size(); i++)
			if (m_BoneInfo[i].Name.compare(boneName) == 0)
				return i;

		return -1;
	}

	unsigned int CustomModel::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumScalingKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);

		return 0;
	}

	void CustomModel::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumScalingKeys == 1) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	unsigned int CustomModel::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		assert(pNodeAnim->mNumRotationKeys > 0);

		for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
			if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
				return i;
			}
		}

		assert(0);

		return 0;
	}

	void CustomModel::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if (pNodeAnim->mNumRotationKeys == 1) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}

		unsigned int  RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}

	unsigned int CustomModel::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
			if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
				return i;

		assert(0);
		return 0;
	}

	void CustomModel::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if (pNodeAnim->mNumPositionKeys == 1) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}

		unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void CustomModel::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, Mat4f ParentTransform, const aiScene* m_pscene)
	{
		std::string NodeName = std::string(pNode->mName.data);

		const aiAnimation* pAnimation = m_pscene->mAnimations[0];

		Mat4f NodeTransformation(
			pNode->mTransformation.a1, pNode->mTransformation.b1, pNode->mTransformation.c1, pNode->mTransformation.d1,
			pNode->mTransformation.a2, pNode->mTransformation.b2, pNode->mTransformation.c2, pNode->mTransformation.d2,
			pNode->mTransformation.a3, pNode->mTransformation.b3, pNode->mTransformation.c3, pNode->mTransformation.d3,
			pNode->mTransformation.a4, pNode->mTransformation.b4, pNode->mTransformation.c4, pNode->mTransformation.d4
		);

		const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

		if (pNodeAnim)
		{
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			Mat4f ScalingM = Mat4f::createScale(Scaling.x, Scaling.y, Scaling.z);

			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
			aiMatrix3x3 mat3x3 = RotationQ.GetMatrix();

			Mat4f RotationM = Mat4f(
				mat3x3.a1, mat3x3.b1, mat3x3.c1, 0.0f,
				mat3x3.a2, mat3x3.b2, mat3x3.c2, 0.0f,
				mat3x3.a3, mat3x3.b3, mat3x3.c3, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			Mat4f TranslationM = Mat4f::createTranslate(Translation.x, Translation.y, Translation.z);

			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM;
		}

		Mat4f GlobalTransformation = ParentTransform * NodeTransformation;

		int boneIndex = FindBoneIndexByName(NodeName);
		if (boneIndex != -1)
			m_BoneInfo[boneIndex].FinalTransformation = boneGlobalInverseTransform * GlobalTransformation * m_BoneInfo[boneIndex].BoneOffset;

		for (unsigned int i = 0; i < pNode->mNumChildren; i++)
			ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation, m_pscene);
	}

	Mat4f* CustomModel::getBonesTransform(const aiScene* scene)
	{
		// calcula as matrizes dos bones em função à animação atual
		Mat4f* transforms = new Mat4f[bonesCount];
		Mat4f identityMatrix = Mat4f::identity();

		//float elapsedTime = 30;
		float elapsedTime = 50;
		float TimeInSeconds = elapsedTime / 1000;

		// calcula o tempo da animação ****
		float TicksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 30.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)scene->mAnimations[0]->mDuration);
		//*********************************

		ReadNodeHeirarchy(AnimationTime, scene->mRootNode, identityMatrix, scene);

		for (unsigned int i = 0; i < bonesCount; i++) {
			transforms[i] = m_BoneInfo[i].FinalTransformation;
			//transforms[i] = Mat4f::identity();
			//transforms[i] = Mat4f();
		}
		//************************************************

		return transforms;
	}


	void CustomModel::render(Mat4f projectionViewMatrix, const aiScene* scene)
	{
		Mat3f normalMatrix = model.toMat3();

		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, modelVBO);
		setUpPositionAttribute();
		setUpNormalAttribute();
		setUpTextureAttribute();

		if (hasTexture)
			glBindTexture(GL_TEXTURE_2D, texture);

		if (hasBones)
		{
			glBindBuffer(GL_ARRAY_BUFFER, boneIndexVBO);
			setUpIndexBoneAttribute();

			glBindBuffer(GL_ARRAY_BUFFER, boneWeightVBO);
			setUpWeightBoneAttribute();

			bonesTransform = getBonesTransform(scene);
			for (size_t i = 0; i < bonesCount; i++)
				glUniformMatrix4fv(bonesOffsetLocation[i], 1, GL_FALSE, bonesTransform[i].getValues());
		}

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

		glUniform1i(hasTextureLocation, hasTexture ? 1 : 0);

		glDrawElements(GL_TRIANGLES, facesCount, GL_UNSIGNED_SHORT, faces);
	}

	CustomModel::~CustomModel()
	{
		if (programShader != 0)
			glDeleteProgram(programShader);

		if (modelVBO != 0)
			glDeleteBuffers(1, &modelVBO);

		if (faces != nullptr)
			delete[] faces;

		for (size_t i = 0; i < m_BoneInfo.size(); i++)
			delete[] m_BoneInfo[i].BoneOffset.getValues();
	}
}
*/