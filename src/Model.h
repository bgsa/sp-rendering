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
	protected:
		SpArray<Vec3f>* vertexes;
		SpArray<Vec3f>* normals;
		SpArray<Vec2f>* textureCoordinates;

	public:

		API_INTERFACE Model()
		{
			vertexes = NULL;
			normals = NULL;
			textureCoordinates = NULL;
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
		}

		~Model()
		{
			dispose();
		}

	};
}

#endif // MODEL_HEADER