#include "ModelFile.h"

namespace NAMESPACE_RENDERING
{
	ModelFileHeader ModelFile::getHeader()
	{
		return header;
	}

	GLfloat* ModelFile::getVertexes()
	{
		file.seek(sizeof(ModelFileHeader));

		GLfloat* vertexes = new GLfloat[header.VertexCount];

		file.read((sp_char*)&vertexes[0], header.VertexCount * sizeof(GLfloat));

		return vertexes;
	}

	GLfloat* ModelFile::getNormals()
	{
		file.seek(sizeof(ModelFileHeader) + header.VertexCount * sizeof(GLfloat));

		GLfloat* normals = new GLfloat[header.VertexCount];

		file.read((sp_char*)&normals[0], header.VertexCount * sizeof(GLfloat));

		return normals;
	}

	GLfloat* ModelFile::getTexturesUV()
	{
		if (header.TextureUVCount == 0)
			return nullptr;

		file.seek(sizeof(ModelFileHeader) + ((header.VertexCount * 2) * sizeof(GLfloat)));

		GLfloat* textures = new GLfloat[header.TextureUVCount];

		file.read((sp_char*) textures, header.TextureUVCount * sizeof(GLfloat));

		return textures;
	}

	GLushort* ModelFile::getFaces()
	{
		size_t offset = sizeof(ModelFileHeader) + ((header.VertexCount * sizeof(GLfloat)) * 2);

		if (header.TextureUVCount > 0)
			offset += header.TextureUVCount * sizeof(GLfloat);

		file.seek(offset);

		GLushort* faces = new GLushort[header.FacesCount];

		file.read((sp_char*)faces, header.FacesCount * sizeof(GLushort));

		return faces;
	}

	GLfloat* ModelFile::getPositionNormalsTexture()
	{
		file.seek(sizeof(ModelFileHeader));

		size_t size = (header.VertexCount * 2) + header.TextureUVCount;

		GLfloat* buffer = new GLfloat[size];

		file.read((sp_char*)buffer, sizeof(GLfloat) * size);

		return buffer;
	}

	size_t ModelFile::getBonesNameSize()
	{
		if (header.BonesCount == 0)
			return 0;

		size_t offset = sizeof(ModelFileHeader); // header offset
		offset += (header.VertexCount * sizeof(GLfloat)) * 2; // + (vertex + normals) offset

		if (header.TextureUVCount > 0)
			offset += header.TextureUVCount * sizeof(GLfloat);  // + texture offset

		offset += header.FacesCount * sizeof(GLushort); // + faces offset

		file.seek(offset);

		size_t totalBonesNameSize;
		file.read((sp_char*)&totalBonesNameSize, sizeof(size_t));

		return totalBonesNameSize;
	}

	std::vector<std::string> ModelFile::getBonesName()
	{
		std::vector<std::string> names;
		size_t totalBonesNameSize = getBonesNameSize();

		char* buffer = (char*)malloc(totalBonesNameSize);
		file.read(buffer, totalBonesNameSize);

		size_t index = 0;
		for (size_t i = 0; i < header.BonesCount; i++)
		{
			size_t length = buffer[index];

			char* nameTemp = new char[length];
			memcpy(nameTemp, &buffer[sizeof(size_t) + index], length);
			nameTemp[length] = '\0';

			std::string name = std::string(nameTemp);
			names.push_back(name);

			index += sizeof(size_t) + length;
		}

		return names;
	}

	Mat4* ModelFile::getBonesOffset()
	{
		if (header.BonesCount == 0)
			return nullptr;

		size_t offset = sizeof(ModelFileHeader); // header offset
		offset += (header.VertexCount * sizeof(GLfloat)) * 2; // + (vertex + normals) offset

		if (header.TextureUVCount > 0)
			offset += header.TextureUVCount * sizeof(GLfloat);  // + texture offset

		offset += header.FacesCount * sizeof(GLushort); // + faces offset
		offset += sizeof(size_t);     // + bones name total size offset
		offset += getBonesNameSize(); // + bones name offset

		file.seek(offset);

		GLfloat* temp = new GLfloat[header.BonesCount * MAT4_LENGTH];
		file.read((char*)&temp[0], header.BonesCount * MAT4_LENGTH * sizeof(GLfloat));

		Mat4* result = new Mat4[header.BonesCount];
		for (sp_size i = 0; i < header.BonesCount; i++)
			for (sp_size j = 0; j < MAT4_LENGTH; j++)
				result[i][j] = temp[(i * MAT4_LENGTH) + j];

		return result;
	}

	size_t ModelFile::getBonesIndexesSize()
	{
		return (header.VertexCount / 3) * 4 * sizeof(GLint);
	}

	size_t ModelFile::getBonesWeightsSize()
	{
		return (header.VertexCount / 3) * 4 * sizeof(GLfloat);
	}

	size_t ModelFile::getBonesIndexesAndWeightsSize()
	{
		return getBonesIndexesSize() + getBonesIndexesSize();
	}

	GLint* ModelFile::getBonesIndexes()
	{
		if (header.BonesCount == 0)
			return nullptr;

		size_t offset = sizeof(ModelFileHeader); // header offset
		offset += (header.VertexCount * sizeof(GLfloat)) * 2; // + (vertex + normals) offset

		if (header.TextureUVCount > 0)
			offset += header.TextureUVCount * sizeof(GLfloat);  // + texture offset

		offset += header.FacesCount * sizeof(GLushort); // + faces offset
		offset += sizeof(size_t);     // + bones name total size offset
		offset += getBonesNameSize(); // + bones name offset

		offset += header.BonesCount * MAT4_LENGTH * sizeof(GLfloat); // + (bones matrixes) offset

		file.seek(offset);

		size_t totalSize = getBonesIndexesSize();

		GLint* buffer = (GLint*)malloc(totalSize);
		file.read((sp_char*)buffer, totalSize);

		return buffer;
	}

	GLfloat* ModelFile::getBonesWeights() {
		if (header.BonesCount == 0)
			return nullptr;

		size_t offset = sizeof(ModelFileHeader); // header offset
		offset += (header.VertexCount * sizeof(GLfloat)) * 2; // + (vertex + normals) offset

		if (header.TextureUVCount > 0)
			offset += header.TextureUVCount * sizeof(GLfloat);  // + texture offset

		offset += header.FacesCount * sizeof(GLushort); // + faces offset
		offset += sizeof(size_t);     // + bones name total size offset
		offset += getBonesNameSize(); // + bones name offset

		offset += header.BonesCount * MAT4_LENGTH * sizeof(GLfloat); // + (bones matrixes) offset
		offset += getBonesIndexesSize(); // + bones indexes

		file.seek(offset);

		size_t totalSize = getBonesWeightsSize();

		GLfloat* buffer = (GLfloat*)malloc(totalSize);
		file.read((sp_char*)buffer, totalSize);

		return buffer;
	}

	void* ModelFile::getBonesIndexesAndWeights()
	{
		if (header.BonesCount == 0)
			return nullptr;

		size_t offset = sizeof(ModelFileHeader); // header offset
		offset += (header.VertexCount * sizeof(GLfloat)) * 2; // + (vertex + normals) offset

		if (header.TextureUVCount > 0)
			offset += header.TextureUVCount * sizeof(GLfloat);  // + texture offset

		offset += header.FacesCount * sizeof(GLushort); // + faces offset
		offset += sizeof(size_t);     // + bones name total size offset
		offset += getBonesNameSize(); // + bones name offset

		offset += header.BonesCount * MAT4_LENGTH * sizeof(GLfloat); // + (bones matrixes) offset

		file.seek(offset);

		size_t totalSize = getBonesIndexesAndWeightsSize();

		void* buffer = malloc(totalSize);
		file.read((sp_char*)buffer, totalSize);

		return buffer;
	}

	ModelFileHeader ModelFile::load(std::string filename)
	{
		file.open(filename.c_str(), std::ios::in);

		file.read((sp_char*)&header, sizeof(ModelFileHeader));

		return header;
	}

	ModelFile::~ModelFile()
	{
		if (file.isOpened())
			file.close();
	}
}