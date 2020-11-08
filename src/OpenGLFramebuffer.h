#ifndef OPENGL_FRAMEBUFFER_HEADER
#define OPENGL_FRAMEBUFFER_HEADER

#include "SpectrumRendering.h"
#include "SpSize.h"

namespace NAMESPACE_RENDERING
{

	class OpenGLFramebuffer
	{
#define SP_OPENGL_FRAMEBUFFER_DEFAULT_FORMAT GL_RGBA
	private:
		sp_uint _id = 0u;
		SpSize<sp_int> _size;
		sp_uint colorAttachment = 0u;
		sp_uint depthStencilAttachment = 0u;

	public:

		API_INTERFACE inline SpSize<sp_int> size() const
		{
			return _size;
		}
		
		API_INTERFACE inline sp_uint colorTexture() const
		{
			return colorAttachment;
		}

		/// <summary>
		/// Create a new OpenGL Framebuffer
		/// </summary>
		API_INTERFACE inline OpenGLFramebuffer()
		{
			glGenFramebuffers(1, &_id);
			glGenTextures(1, &colorAttachment);
			glGenTextures(1, &depthStencilAttachment);
		}

		API_INTERFACE inline OpenGLFramebuffer* use()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _id);
			sp_assert(glCheckNamedFramebufferStatus(_id, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "OpenGLFramebufferException");
			return this;
		}

		API_INTERFACE inline OpenGLFramebuffer* setup(sp_int width, sp_int height, sp_uint internalformat = SP_OPENGL_FRAMEBUFFER_DEFAULT_FORMAT, sp_uint mipMapLevel = ZERO_UINT)
		{
			_size.width = width;
			_size.height = height;

			glBindTexture(GL_TEXTURE_2D, colorAttachment);
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, internalformat, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_2D, depthStencilAttachment);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
				GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
			);

			glBindTexture(GL_TEXTURE_2D, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, _id);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorAttachment, mipMapLevel);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthStencilAttachment, mipMapLevel);

			GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(ONE_UINT, drawbuffers);

			return this;
		}

		API_INTERFACE inline void disable()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		API_INTERFACE inline const sp_char* toString()
		{
			return "OpenGLFramebuffer";
		}

		API_INTERFACE inline void dispose()
		{
			if (_id != 0)
			{
				glDeleteFramebuffers(1, &_id);
				_id = 0;
			}

			if (colorAttachment == 0)
			{
				glDeleteTextures(1, &colorAttachment);
				colorAttachment = 0;
			}

			if (depthStencilAttachment == 0)
			{
				glDeleteTextures(1, &depthStencilAttachment);
				depthStencilAttachment = 0;
			}
		}

		API_INTERFACE ~OpenGLFramebuffer()
		{
			dispose();
		}

#undef SP_OPENGL_FRAMEBUFFER_DEFAULT_FORMAT
	};
}

#endif // OPENGL_FRAMEBUFFER_HEADER