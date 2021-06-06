#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace NAMESPACE_RENDERING
{
	OpenGLTexture::OpenGLTexture(const sp_char* filename)
	{
        sp_uchar* image_data = stbi_load(filename, &_size.width, &_size.height, NULL, 4);
        if (image_data == NULL)
            return;

        // Create a OpenGL texture identifier
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

        // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _size.width, _size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);
	}
}
