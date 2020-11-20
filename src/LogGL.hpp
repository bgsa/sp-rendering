#ifndef LOG_GL_HEADER
#define LOG_GL_HEADER

#include "SpectrumRendering.h"
#include "SpLogger.h"

namespace NAMESPACE_RENDERING
{
	class LogGL
	{
	public:

		static void glErrors(const char *file, int line)
		{
			GLenum error = glGetError();
			std::string msg;

			while (error != GL_NO_ERROR) {
				std::string errorMessage = "";

				switch (error)
				{
				case GL_INVALID_OPERATION:
					errorMessage = "INVALID_OPERATION";
					break;

				case GL_INVALID_ENUM:
					errorMessage = "INVALID_ENUM";
					break;

				case GL_INVALID_VALUE:
					errorMessage = "INVALID_VALUE";
					break;

				case GL_OUT_OF_MEMORY:
					errorMessage = "OUT_OF_MEMORY";
					break;

				case GL_INVALID_FRAMEBUFFER_OPERATION:
					errorMessage = "INVALID_FRAMEBUFFER_OPERATION";
					break;
				}

				sp_log_info1s(errorMessage.c_str());
				sp_log_newline();

				error = glGetError();
			}
		}

	};
}

#endif // LOG_GL_HEADER