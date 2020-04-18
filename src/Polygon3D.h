#ifndef POLYGON_3D_HEADER
#define POLYGON_3D_HEADER

#include "Shader.h"
#include "GraphicObject3D.h"
#include <ColorRGBA.h>
#include "RenderData.h"
#include <Factory.h>
#include <IFileManager.h>
#include <Image.h>
#include <ImageFactory.h>

namespace NAMESPACE_RENDERING
{
	class Polygon3D
		: public GraphicObject3D<AABB>
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
		Vec3List<float> vertexes;
		GLubyte* faces;
		size_t facesSize = 0;

		float borderWidth = 0.0f;
		ColorRGBAf* borderColor = new ColorRGBAf(0.3f, 0.3f, 0.3f, 1.0f);

		std::string API_INTERFACE getTexture();
		void API_INTERFACE setTexture(std::string filename);

		bool API_INTERFACE isTextureEnabled();
		void API_INTERFACE setTextureEnabled(bool isEnabled);

		void init();

		void renderBorder(RenderData renderData);
		void render(RenderData renderData);

		API_INTERFACE const char* toString() override
		{
			return "Polygon 3D";
		}

		~Polygon3D();
	};
}

#endif // !POLYGON_3D_HEADER