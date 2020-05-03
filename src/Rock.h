#ifndef ROCK_HEADER
#define ROCK_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3D.h"
#include "DOP18.h"
#include "IRenderer.h"
#include "ObjModel.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"

namespace NAMESPACE_RENDERING
{
	class Rock
		: public GraphicObject3D<Rock, DOP18>
	{
	private:
		OpenGLBuffer* vertexBuffer;
		OpenGLShader* shader;


		ObjModel model; // teste only

	public:

		API_INTERFACE void init() override;

		API_INTERFACE void update(long long elapsedTime) override
		{
		}

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Rock";
		}

		API_INTERFACE inline void dispose() override;

		API_INTERFACE ~Rock();
		
	};
}

#endif // ROCK_HEADER