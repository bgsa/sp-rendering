#ifndef MODEL_HEADER
#define MODEL_HEADER

#include "SpectrumRendering.h"
#include <SpArray.h>
#include <SpString.h>

namespace NAMESPACE_RENDERING
{
	class Model
		: public Object
	{
	public:
		SpString* name;
		SpArray<Vec3f>* vertexes;
		SpArray<Vec3f>* normals;
		SpArray<Vec3ui>* faces;
		SpArray<Vec2f>* textureCoordinates;

		API_INTERFACE Model()
		{
			name = NULL;
			vertexes = NULL;
			normals = NULL; 
			faces = NULL;
			textureCoordinates = NULL;
		}

		API_INTERFACE inline sp_size sizeOfVertexes()
		{
			return vertexes->length() * sizeof(Vec3f);
		}
		API_INTERFACE inline sp_size sizeOfFaces()
		{
			return faces->length() * sizeof(Vec3ui);
		}
		API_INTERFACE inline sp_size sizeOfNormals()
		{
			return normals->length() * sizeof(Vec3f);
		}
		API_INTERFACE inline sp_size sizeOfTextures()
		{
			return textureCoordinates->length() * sizeof(Vec2f);
		}

		API_INTERFACE virtual void load(const SpString& filename) = 0;

		API_INTERFACE virtual sp_size sizeOfAllBuffers()
		{
			return sizeOfVertexes() + sizeOfNormals() + sizeOfTextures();
		}

		API_INTERFACE virtual void allBuffers(sp_char* allocatedBuffer)
		{
			const sp_size sizeVertexes = sizeOfVertexes();
			const sp_size sizeNormals = sizeOfNormals();
			const sp_size sizeTextures = sizeOfTextures();

			std::memcpy(allocatedBuffer, (sp_char*) vertexes->data(), sizeVertexes);
			std::memcpy(&allocatedBuffer[sizeVertexes], (sp_char*)normals->data(), sizeNormals);
			std::memcpy(&allocatedBuffer[sizeVertexes + sizeNormals], (sp_char*)textureCoordinates->data(), sizeTextures);
		}

		API_INTERFACE virtual void dispose() override
		{
			if (name != NULL)
			{
				sp_mem_delete(name, SpString);
				name = NULL;
			}

			if (vertexes != NULL)
			{
				sp_mem_release(vertexes);
				vertexes = NULL;
			}

			if (normals != NULL)
			{
				sp_mem_release(normals);
				normals = NULL;
			}

			if (textureCoordinates != NULL)
			{
				sp_mem_release(textureCoordinates);
				textureCoordinates = NULL;
			}

			if (faces != NULL)
			{
				sp_mem_release(faces);
				faces = NULL;
			}
		}

		~Model()
		{
			dispose();
		}

	};
}

#endif // MODEL_HEADER