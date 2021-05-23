#ifdef OPENGL_ENABLED

#ifndef OPENGL_TEXTURE_HEADER
#define OPENGL_TEXTURE_HEADER

#include "SpectrumRendering.h"
#include "Framebuffer.h"
#include "SpSize.h"

namespace NAMESPACE_RENDERING
{
	class OpenGLTexture
		: public Object
	{
	private:
		sp_uint _id;
		sp_uint _length;
		SpSize<sp_int> _size;

	public:

		API_INTERFACE OpenGLTexture()
		{
			_id = ZERO_UINT;
			_length = ZERO_UINT;
		}

		API_INTERFACE inline sp_uint id() const
		{
			return _id;
		}

		API_INTERFACE inline SpSize<sp_int> size() const
		{
			return _size;
		}

		API_INTERFACE inline OpenGLTexture* init(const sp_uint length, const SpSize<sp_int>& size)
		{
			_length = length;
			_size = size;
			glGenTextures(length, &_id);
			return this;
		}

		API_INTERFACE inline OpenGLTexture* use()
		{
			glBindTexture(GL_TEXTURE_2D, _id);
			return this;
		}

		API_INTERFACE inline OpenGLTexture* resize(const SpSize<sp_int>& size)
		{
			_size = size;
			return this;
		}

		API_INTERFACE inline void disable()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		API_INTERFACE inline OpenGLTexture* property(sp_int name, sp_int value)
		{
			glTexParameteri(GL_TEXTURE_2D, name, value);
			return this;
		}

		API_INTERFACE static OpenGLTexture* createFromFramebuffer(GLenum framebuffer = GL_BACK)
		{
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

			sp_uchar* data = ALLOC_ARRAY(sp_uchar, FOUR_UINT * viewport[2] * viewport[3]);

			Framebuffer::getFramebuffer(data, framebuffer);

			OpenGLTexture* texture = sp_mem_new(OpenGLTexture)();
			texture
				->init(1u, SpSize<sp_int>(viewport[2], viewport[3]))
				->use()
				->property(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
				->property(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
				->updateData(data, GL_RGBA);
			
			ALLOC_RELEASE(data);
			return texture;
		}

		API_INTERFACE inline OpenGLTexture* updateData(const sp_uchar* data, const sp_int colorFormat)
		{
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, _size.width, _size.height, 0, colorFormat, GL_UNSIGNED_BYTE, data);
			return this;
		}

		API_INTERFACE inline void readData(sp_uchar* data, sp_int outputFormat = GL_RGBA)
		{
			use();
			glGetTexImage(GL_TEXTURE_2D, 0, outputFormat, GL_UNSIGNED_BYTE, data);
		}

		API_INTERFACE const sp_char* toString() override
		{
			return "OpenGL Texture";
		}

		API_INTERFACE void dispose() override
		{
			if (_id != ZERO_UINT)
			{
				glDeleteTextures(_length, &_id);
				_id = ZERO_UINT;
			}
		}

		~OpenGLTexture()
		{
			dispose();
		}

	};
}

#endif // OPENGL_TEXTURE_HEADER

#endif // OPENGL_ENABLED
