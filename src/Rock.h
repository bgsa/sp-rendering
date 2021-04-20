#ifndef ROCK_HEADER
#define ROCK_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3D.h"
#include "DOP18.h"
#include "IGraphicObjectRenderer.h"
#include "ObjModel.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "ImageBMP.h"
#include "SpPhysicObject.h"

namespace NAMESPACE_RENDERING
{
	class Rock
		: public GraphicObject3D
		, public SpPhysicObject
	{
	private:
		OpenGLBuffer* buffer;
		OpenGLShader* shader;
		OpenGLBuffer* indexesBuffer;
		OpenGLTexture* texture = NULL;

		sp_uint sizeOfVertexes;
		sp_uint sizeOfNormals;
		sp_uint facesLength;

		sp_int textureAttribute;

		DOP18* _boundingVolume;

	public:

		API_INTERFACE void init() override;

		API_INTERFACE void update(sp_float elapsedTime) override
		{
		}

		API_INTERFACE DOP18* boundingVolume() const override
		{
			return _boundingVolume;
		}

		API_INTERFACE void render(const SpRenderData& renderData) override;

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Rock";
		}

		API_INTERFACE inline void dispose() override;

		API_INTERFACE ~Rock();
		
	};
}

#endif // ROCK_HEADER