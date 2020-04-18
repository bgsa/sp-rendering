#ifndef MODEL_FILE_RENDERING
#define MODEL_FILE_RENDERING

#include "SpectrumRendering.h"
#include <fstream>
#include <IFile.h>
#include <IFileManager.h>
#include <Factory.h>
#include <IFile.h>

namespace NAMESPACE_RENDERING
{
	enum ModelFileOperationStatus {
		SUCCESS = 0,
		PERMISSION_DENIED = 1
	};

	struct ModelFileHeader {
		unsigned short Version;
		unsigned int VertexCount;
		unsigned int TextureUVCount;
		unsigned int FacesCount;
		unsigned int BonesCount;
		Vec3f MaterialAmbient;
		Vec3f MaterialDiffuse;
		Vec3f MaterialSpecular;
		float ShininessFactor;
	};

	struct BoneName {
		unsigned char length;
		const char* name;
	};

	struct ModelFileContent {
		GLfloat* vertexes;
		GLfloat* normals;
		GLushort* faces;
		GLfloat* textureUVs;
		size_t bonesNameSize;
		BoneName* bonesName;
		GLfloat* bonesOffset;
		GLint* bonesIndex;
		GLfloat* bonesWeight;
	};

	class ModelFile
	{
	private:
		IFile* file;
		ModelFileHeader header;

	public:
		ModelFileHeader load(std::string filename);

		ModelFileHeader getHeader();

		GLfloat* getVertexes();
		GLfloat* getNormals();
		GLfloat* getTexturesUV();
		GLushort* getFaces();
		GLfloat* getPositionNormalsTexture();
		Mat4f* getBonesOffset();
		std::vector<std::string> getBonesName();
		size_t getBonesNameSize();
		GLint* getBonesIndexes();
		GLfloat* getBonesWeights();
		void* getBonesIndexesAndWeights();
		size_t getBonesIndexesSize();
		size_t getBonesWeightsSize();
		size_t getBonesIndexesAndWeightsSize();

		static ModelFileOperationStatus writeFile(std::string filename, ModelFileHeader* header, ModelFileContent* content)
		{
			bool hasTexture = header->TextureUVCount > 0;
			bool hasBones = header->BonesCount > 0;

			std::ofstream newFile(filename + ".pxm", std::ofstream::out | std::ofstream::binary);

			if (!newFile.is_open())
				return ModelFileOperationStatus::PERMISSION_DENIED;

			newFile.write((char*)header, sizeof(ModelFileHeader));

			newFile.write((char*)content->vertexes, header->VertexCount * sizeof(float)); //write vertices
			newFile.write((char*)content->normals, header->VertexCount * sizeof(float)); //write normals

			if (hasTexture)
				newFile.write((char*)content->textureUVs, header->TextureUVCount * sizeof(float)); //write UVs

			newFile.write((char*)content->faces, header->FacesCount * sizeof(unsigned short)); //write faces

			if (hasBones)
			{
				size_t totalBonesNameSize = 0;
				for (size_t i = 0; i < header->BonesCount; i++)
					totalBonesNameSize += sizeof(size_t) + content->bonesName[i].length;

				newFile.write((char*)&totalBonesNameSize, sizeof(unsigned int)); //write bones name size

				for (size_t i = 0; i < header->BonesCount; i++) {
					size_t length = content->bonesName[i].length;

					newFile.write((char*)&length, sizeof(size_t)); //write bones name length
					newFile.write((char*)content->bonesName[i].name, length); //write bones name
				}

				//cout << content->bonesWeight[0] << '\n';
				//cout << content->bonesWeight[header->VertexCount / 3 * 4 - 1] << '\n';

				newFile.write((char*)content->bonesOffset, header->BonesCount * MAT4_SIZE * sizeof(float)); //write bones offset matrix4x4
				newFile.write((char*)content->bonesIndex, (header->VertexCount / 3) * 4 * sizeof(int)); //write bones indexes (max 4 bones per vertex)
				newFile.write((char*)content->bonesWeight, (header->VertexCount / 3) * 4 * sizeof(float)); //write bones weight (max 4 bones per vertex)
			}

			newFile.flush();
			newFile.close();

			return ModelFileOperationStatus::SUCCESS;
		}

		~ModelFile();
	};
}

#endif // MODEL_FILE_RENDERING