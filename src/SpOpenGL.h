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

#ifdef DEBUG
	#define sp_opengl_check(command) \
		command; \
		sp_assert(glGetError() == GL_NO_ERROR, "OpenCLException" );
#else
	#define sp_opengl_check(command) command
#endif
}

#endif // !SP_OPENGL_HEADER
