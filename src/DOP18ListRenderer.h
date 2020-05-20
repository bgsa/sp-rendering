#ifndef DOP_18_LIST_HEADER
#define DOP_18_LIST_HEADER

#include "SpectrumRendering.h"
#include <DOP18.h>
#include "GraphicObject3DList.h"
#include "OpenGLShader.h"

namespace NAMESPACE_RENDERING
{
	class DOP18ListRenderer
		: public GraphicObject3DList
	{
	private:
		OpenGLShader* shader;

		void initIndexBuffer();
		void initVertexBuffer();		
		void updateVertexBuffer(DOP18* listOfDOP18, sp_uint length);

		void render(const RenderData& renderData) override { }

	public:

		API_INTERFACE DOP18ListRenderer();
		
		API_INTERFACE void init() override;

		API_INTERFACE void translate(const sp_uint index, Vec3 translation) override { }

		API_INTERFACE void scale(const sp_uint index, Vec3 factors) override { }

		API_INTERFACE void render(const RenderData& renderData, DOP18* listOfDOP18, sp_uint length);

		API_INTERFACE void dispose() override;

		API_INTERFACE const sp_char* toString() override
		{
			return "k-DOP 18";
		}

	};
}

#endif // DOP_18_LIST_HEADER