#ifndef SP_OPENGL_HEADER

#include "SpectrumFoundation.h"
#include "SpLogger.h"

#ifdef OPENGL_ENABLED
	#define GLEW_STATIC
	#include <GL/glew.h>
	#include <GLFW/glfw3.h>
#endif

#ifdef OPENGLES_ENABLED
	#include <GLES3/gl3.h>
#endif

namespace NAMESPACE_RENDERING
{
	class SpOpenGL
	{
	private:

		static void initGlew()
		{
			GLenum glewinit = glewInit();

			if (glewinit != GLEW_OK)
			{
				const sp_char* errorMessage = reinterpret_cast<sp_char*>(((GLubyte*)glewGetErrorString(glewinit)));
				sp_log_error1s(errorMessage); sp_log_newline();
			}
		}

		static void initOpenGLOffScreen()
		{
			if (isContextCreated())
				return;

			if (!glfwInit())
			{
				sp_log_error1s("Failed to init GLFW"); sp_log_newline();
				return;
			}

#ifdef OPENGLES_ENABLED
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
			glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#endif

			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

			GLFWwindow* window = glfwCreateWindow(300, 300, "Spectrum Engine", NULL, NULL);

			if (window == nullptr)
			{
				sp_log_error1s("Failed to create GLFW Window"); sp_log_newline();
				glfwTerminate();
				return;
			}

			glfwMakeContextCurrent(window);
		}

	public:

		API_INTERFACE static void init()
		{
			initGlew();
		}

		API_INTERFACE static void initOffScreenRendering()
		{
			initOpenGLOffScreen();
			initGlew();
		}

		API_INTERFACE static sp_bool isContextCreated()
		{
			return glfwGetCurrentContext() != nullptr;
		}

		API_INTERFACE static void dispose()
		{
			GLFWwindow* window = glfwGetCurrentContext();

			if (window != nullptr)
			{
				glfwDestroyWindow(window);
				window = nullptr;

				glfwTerminate();
			}
		}

	};

	static inline void logOpenGLError(GLenum errorId, const sp_char* file, sp_int line)
	{
		if (errorId == GL_NO_ERROR)
			return;

		sp_char errorMessage[300];

		switch (errorId)
		{
		case GL_INVALID_OPERATION:
			std::strcpy(errorMessage, "INVALID_OPERATION - ");
			break;

		case GL_INVALID_ENUM:
			std::strcpy(errorMessage, "INVALID_ENUM - ");
			break;

		case GL_INVALID_VALUE:
			std::strcpy(errorMessage, "INVALID_VALUE - ");
			break;

		case GL_OUT_OF_MEMORY:
			std::strcpy(errorMessage, "OUT_OF_MEMORY - ");
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::strcpy(errorMessage, "INVALID_FRAMEBUFFER_OPERATION - ");
			break;
		}

		std::strcat(errorMessage, file);
		std::strcat(errorMessage, ":");

		sp_char lineAsString[8];
		sp_uint len;
		convert(line, lineAsString, len);
		std::strcat(errorMessage, lineAsString);

		sp_log_info1s(errorMessage);
		sp_log_newline();
	}

#ifdef DEBUG
	#define sp_opengl_check(command) \
		command; \
		{ \
			GLenum errorId = glGetError(); \
			if (errorId != GL_NO_ERROR) \
			{ \
				logOpenGLError(errorId, __FILENAME__, __LINE__ ); \
				sp_assert(false, "OpenGLException"); \
			} \
		}

#define sp_opengl_check_error() \
		{ \
			GLenum errorId = glGetError(); \
			if (errorId != GL_NO_ERROR) \
			{ \
				logOpenGLError(errorId, __FILENAME__, __LINE__ ); \
				sp_assert(false, "OpenGLException"); \
			} \
		}
#else
	#define sp_opengl_check(command) command
	#define sp_opengl_check_error()
#endif
}

#endif // !SP_OPENGL_HEADER
