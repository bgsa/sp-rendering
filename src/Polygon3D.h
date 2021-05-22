#ifndef POLYGON_3D_HEADER
#define POLYGON_3D_HEADER

#include "Shader.h"
#include "GraphicObject3D.h"
#include <ColorRGBA.h>
#include "SpRenderData.h"
#include <Image.h>
#include <ImageFactory.h>
#include <FileSystem.h>

namespace NAMESPACE_RENDERING
{
	class Polygon3D
		: public GraphicObject3D
	{
	private:
		GLint textureAttribute;
		GLint textureEnableLocation;

		std::string textureFilename;
		bool isTextureEnabledValue = false;

		GLuint textureBuffer;

		void initVBO();
		void setUpPositionAttribute();
		void setUpTextureAttribute();

		float* getVertexBuffer();

		void setUpImage(std::string filename);

	public:
		Vec3List vertexes;
		GLubyte* faces;
		sp_size facesSize = ZERO_SIZE;

		sp_float borderWidth = 0.0f;
		ColorRGBA* borderColor = new ColorRGBA(0.3f, 0.3f, 0.3f, 1.0f);

		std::string API_INTERFACE getTexture();
		void API_INTERFACE setTexture(std::string filename);

		bool API_INTERFACE isTextureEnabled();
		void API_INTERFACE setTextureEnabled(bool isEnabled);

		void init();

		void renderBorder(SpRenderData renderData);
		void render(SpRenderData renderData);

		API_INTERFACE const char* toString() override
		{
			return "Polygon 3D";
		}

		~Polygon3D();
	};
}

#endif // !POLYGON_3D_HEADER