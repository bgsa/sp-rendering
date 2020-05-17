#ifdef ABCD

#include <cstdlib>
#include <vector>

#include "Log.h"
#include "CompositeModel.h"

#include "Factory.h"
#include "IFileManager.h"
#include "Image.h"
#include "ImageFactory.h"

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Assimp::Importer importer;

namespace NAMESPACE_RENDERING
{
	void CompositeModel::init(string modelName)
	{
		IFileManager * fileManager = Factory::getFileManagerInstance();
		string folder = "resources/models/" + modelName;
		vector<string> files = fileManager->getFilesFromFolder(folder, ".pxm");

		//string sourceFilename = "C:\\Users\\Bruno\\Downloads\\Wolf Rigged and Game Ready\\Wolf_With_Baked_Action_Animations_For_Export_One_Mesh.fbx";
		//string sourceFilename = "C:\\Users\\Bruno\\Downloads\\Low Poly Spider\\Only_Spider_with_Animations_Export.dae";	
		//string sourceFilename = "C:\\Users\\Bruno\\Downloads\\Character Running.fbx";
		//string sourceFilename = "C:\\Users\\Bruno\\Downloads\\Character Running.blend";
		string sourceFilename = "C:\\Users\\Bruno\\Downloads\\Ted\\Ted.fbx";

		scene = importer.ReadFile(sourceFilename,
			aiProcess_CalcTangentSpace
			| aiProcess_FindInvalidData
			| aiProcess_FixInfacingNormals
			| aiProcess_GenSmoothNormals
			| aiProcess_JoinIdenticalVertices
			| aiProcess_Triangulate
			| aiProcess_SortByPType
		);

		if (!scene) {
			cout << importer.GetErrorString();
			return;
		}


		aiMatrix4x4 mat4 = scene->mRootNode->mTransformation;
		mat4.Inverse();
		Mat4 boneGlobalInverseTransform = Mat4(
			mat4.a1, mat4.b1, mat4.c1, mat4.d1,
			mat4.a2, mat4.b2, mat4.c2, mat4.d2,
			mat4.a3, mat4.b3, mat4.c3, mat4.d3,
			mat4.a4, mat4.b4, mat4.c4, mat4.d4);
		////*
		*comment

			for (size_t i = 0; i < files.size(); i++)
			{
				CustomModel * model = new CustomModel;
				models.push_back(model);

				//models[i]->boneGlobalInverseTransform = boneGlobalInverseTransform;
				models[i]->init("resources/models/" + modelName + "/" + files[i]);

				models[i]->setLightAmbient({ 0.5f, 0.5f, 0.5f });
				//models[i]->scale(0.01f);
				//models[i]->scale(0.1f);
				//models[i]->scale(5.0f);
				//models[i]->translate(0.0f, 2000.0f, 0.0f);
				//models[i]->rotate(degreesToRadians(270), 1.0f, 0.0f, 0.0f);
				//models[i]->rotate(degreesToRadians(180), 1.0f, 0.0f, 0.0f);
				//models[i]->rotate(degreesToRadians(90), 1.0f, 0.0f, 0.0f);
			}

		delete fileManager;
	}


	void CompositeModel::render(Mat4 projectionViewMatrix)
	{
		//primeiro mesh corpo, rabo, parte da cabeça, patas ...
		//segundo mesh sao os olhos apenas - index == 1
		//terceiro mesh sao as unhas das patas
		//quarto mesh sao os dentes
		//quinto mesh parte do torax, porem deformada

		for (size_t i = 0; i < models.size(); i++) {
			models[i]->render(projectionViewMatrix, scene);
		}
	}

	CompositeModel::~CompositeModel()
	{
		for (size_t i = 0; i < models.size(); i++)
			delete models[i];
	}
}

#endif