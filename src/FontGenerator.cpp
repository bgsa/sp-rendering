/*
#include "FontGenerator.h"

namespace NAMESPACE_RENDERING
{
	FontGenerator::FontGenerator()
	{
		if (FT_Init_FreeType(&library))
			Log::error("Free type initialization failed");

		atlasTexture = 0;
		textureDimension = 0;
		squareSize = 0;
	}

	FontGenerator::~FontGenerator()
	{
		FT_Done_Face(fontFace);
		FT_Done_FreeType(library);

		if (atlasTexture)
		{
			glDeleteTextures(1, &atlasTexture);
			atlasTexture = 0;
		}
	}

	void FontGenerator::setPixel(GLubyte* texture, int offset, int size, int x, int y, GLubyte val)
	{
		texture[2 * (offset + x + y * size)] = texture[2 * (offset + x + y * size) + 1] = val;
	}

	void FontGenerator::generateTexFromGlyph(FT_GlyphSlot glyph, GLubyte* texture, int atlasX, int atlasY, int texSize, int resolution, int marginSize, bool drawBorder)
	{
		const int squareSize = resolution + marginSize;
		const int baseOffset = atlasX * squareSize + atlasY * squareSize*texSize;

		if (drawBorder)
		{
			for (int w = 0; w < squareSize; w++)
				setPixel(texture, baseOffset, texSize, w, 0, 255);

			for (int h = 1; h < squareSize; h++)
				for (int w = 0; w < squareSize; w++)
					setPixel(texture, baseOffset, texSize, w, h, (w == 0 || w == squareSize - 1) ? 255 : (h == squareSize - 1) ? 255 : 0);
		}

		const int gr = glyph->bitmap.rows;
		const int gw = glyph->bitmap.width;

		for (int h = 0; h < gr; h++)
			for (int w = 0; w < gw; w++)
				setPixel(texture, baseOffset + marginSize, texSize, w, marginSize + h, glyph->bitmap.buffer[w + h * gw]);

		//    return;
		//    if (!drawBorder) {
		//        return;
		//    }
		//    
		//    for (int h=0; h<squareSize; h++) {
		//        if (h < marginSize || h >= marginSize+gr) {
		//            for (int w=0; w<squareSize; w++)//bottom padding => whole rows
		//                { setPixel(texture, baseOffset, texSize, w, h, 0); }
		//        } else {
		//            for (int w=0; w<marginSize; w++) //left padding
		//                { setPixel(texture, baseOffset, texSize, w, h, 0); }
		//            for (int w=gw+marginSize; w<squareSize; w++) //right padding
		//                { setPixel(texture, baseOffset, texSize, w, h, 0); }
		//        }
		//    }

	}

	bool FontGenerator::readFont(const FT_Face& fontFace, int resolution, int glyphMargin)
	{
		FT_Set_Pixel_Sizes(fontFace, resolution, resolution);
		const int numGlyphs = fontFace->num_glyphs;

		//The actual size of a side of the square that will contain our glyph
		squareSize = resolution + glyphMargin;

		//Calculate the size of the texture we'll need to store these glyphs
		const int numGlyphsPerRow = (int)ceilf((float)sqrt((double)numGlyphs)); //=numRows (texture is a square)

		const int texSize = (numGlyphsPerRow)*squareSize;

		//OpenGL ES requires power of 2 textures
		int realTexSize = OpenML::nextPowOf2(texSize);

		//we use two channels (one for luminance, one for alpha)
		GLubyte* textureData = new GLubyte[realTexSize*realTexSize * 2];
		memset((void*)textureData, 0, realTexSize * realTexSize * 2 * sizeof(GLubyte));

		// if there exist an old atlas delete it.
		if (atlasTexture) {
			glDeleteTextures(1, &atlasTexture);
			atlasTexture = 0;
		}

		glGenTextures(1, &atlasTexture);
		glBindTexture(GL_TEXTURE_2D, atlasTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		LogGL::glErrors(__FILE__, __LINE__);


		int texAtlasX = 0;
		int texAtlasY = 0;
		FT_UInt gindex = 0;
		FT_ULong charcode = 0;

		for (charcode = FT_Get_First_Char(fontFace, &gindex);
			gindex != 0;
			charcode = FT_Get_Next_Char(fontFace, charcode, &gindex))
		{

			//gindex = myc;
			if (FT_Load_Glyph(fontFace, gindex, FT_LOAD_DEFAULT)) {
				Log::error("Error loading glyph with index i and charcode i. Skipping.");
				continue;
			}

			FT_GlyphSlot glyph = fontFace->glyph;
			FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);

			//Calculate glyph informations
			Glyph glyphInfo;
			glyphInfo.metric = glyph->metrics;

			// Get texture offset in the image
			glyphInfo.atlasX = float(texAtlasX*squareSize) / float(realTexSize);
			glyphInfo.atlasY = float(texAtlasY*squareSize) / float(realTexSize);

			//advance is stored in fractional pixel format (=1/64 pixels), as per free type specifications
			glyphInfo.advanceHorizontal = float(glyph->advance.x) / 64.0f;
			glyphInfo.advanceVertical = float(glyph->advance.y) / 64.0f;
			glyphs[charcode] = glyphInfo;

			//Copy the bitmap to the atlas
			generateTexFromGlyph(glyph, textureData, texAtlasX, texAtlasY, realTexSize, resolution, glyphMargin, false);

			texAtlasX++;
			if (texAtlasX >= numGlyphsPerRow)
			{
				texAtlasX = 0;
				texAtlasY++;
			}
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, realTexSize, realTexSize, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, textureData);

		delete[] textureData;

		LogGL::glErrors(__FILE__, __LINE__);

		textureDimension = (squareSize) / (float)realTexSize;

		return true;
	}

	bool FontGenerator::getCorrectResolution(const FT_Face& fontFace, int resolution, int& newResolution, int& newGlyphMargin)
	{
		// Create the texture as per correct size specified by the OEM provider.
		int glyphMargin = (int)ceil(float(resolution) * 0.1f);
		GLint MaxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTextureSize);

		while (resolution > 0)
		{
			glyphMargin = (int)ceil(float(resolution) * 0.1f);
			const long numGlyphs = fontFace->num_glyphs;

			const int squareSize = resolution + glyphMargin;

			const int numGlyphsPerRow = (int)ceilf((float)sqrt((double)numGlyphs));
			const int texSize = (numGlyphsPerRow)*squareSize;
			int realTexSize = OpenML::nextPowOf2(texSize);

			if (realTexSize <= MaxTextureSize)
				break;

			resolution = resolution - 5;    //Decrease the resolution by 5 units.
		}

		if (resolution > 0)
		{
			newResolution = resolution;
			newGlyphMargin = glyphMargin;
			return true;
		}
		else
			return false;
	}

	bool FontGenerator::loadFont(char* buffer, size_t size, int resolution)
	{
		// Generate the face object
		if (FT_New_Memory_Face(library, (unsigned char*)buffer, size, 0, &fontFace))
		{
			Log::error("Error loading font face");
			return false;
		}

		// Get correct resolution from system and generate font bitmaps.
		int calculatedResoution; int calculatedMargin;
		if (getCorrectResolution(fontFace, resolution, calculatedResoution, calculatedMargin))
		{
			return readFont(fontFace, calculatedResoution, calculatedMargin);
		}

		return true;
	}


	bool FontGenerator::loadFont(const char* filename, int resolution)
	{
		// Generate the face object
		if (FT_New_Face(library, filename, 0, &fontFace)) {
			Log::error("Error loading font face");
			return false;
		}

		// Get correct resolution from system and generate font bitmaps.
		int calculatedResoution; int calculatedMargin;
		if (getCorrectResolution(fontFace, resolution, calculatedResoution, calculatedMargin))
		{
			return readFont(fontFace, calculatedResoution, calculatedMargin);
		}

		return true;
	}
}
*/