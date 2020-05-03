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
		SpArray<Vec3f>* vertexes;
		SpArray<Vec3f>* normals;
		SpArray<Vec3ui>* faces;
		SpArray<Vec2f>* textureCoordinates;

		API_INTERFACE Model()
		{
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
		API_INTERFACE inline sp_size sizeOftextures()
		{
			return textureCoordinates->length() * sizeof(Vec2f);
		}

		API_INTERFACE virtual void load(const SpString& filename) = 0;

		API_INTERFACE virtual void dispose() override
		{
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