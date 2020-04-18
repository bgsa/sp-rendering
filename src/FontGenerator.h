/*
#ifndef FONT_GENERATOR_HEADER
#define FONT_GENERATOR_HEADER

#include "SpectrumRendering.h"
#include <map>
#include <FREETYPE/ft2build.h>
#include <FREETYPE/freetype.h>
#include <FREETYPE/ftglyph.h>
#include <FREETYPE/ftoutln.h>
#include <FREETYPE/fttrigon.h>

namespace NAMESPACE_RENDERING
{
	struct Glyph
	{
		// Glyph metric
		FT_Glyph_Metrics metric;

		// Horizontal advance
		float advanceHorizontal;

		// Horizontal advance
		float advanceVertical;

		// Texture Coords in the texture atlas
		float texCoordX, texCoordY;

		// Position in the texture atlas
		float atlasX, atlasY;
	};

	class FontGenerator
	{
	private:
		FT_Library library;

		bool readFont(const FT_Face& fontFace, int resolution, int glyphMargin);

		bool getCorrectResolution(const FT_Face& fontFace, int resolution, int& newResolution, int& newMargin);

		void generateTexFromGlyph(FT_GlyphSlot glyph, GLubyte* texture, int atlasX, int atlasY, int texSize, int resolution, int marginSize, bool drawBorder);

		void setPixel(GLubyte* texture, int offset, int size, int x, int y, GLubyte val);

	public:
		GLuint                      atlasTexture;
		std::map<unsigned long, Glyph>   glyphs;
		float                       textureDimension;
		int                         squareSize;
		FT_Face                     fontFace;

		FontGenerator();
		~FontGenerator();

		bool loadFont(char* buffer, size_t size, int resolution);
		bool loadFont(const char* filename, int resolution);
	};

#endif // !FONT_GENERATOR_HEADER
}
*/