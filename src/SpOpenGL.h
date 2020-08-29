#ifndef SP_OPENGL_HEADER

#include "SpectrumFoundation.h"
#include "Log.hpp"

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
				NAMESPACE_FOUNDATION::Log::error(errorMessage);
			}
		}

		static void initOpenGLOffScreen()
		{
			if (isContextCreated())
				return;

			if (!glfwInit())
			{
				NAMESPACE_FOUNDATION::Log::error("Failed to init GLFW");
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
				NAMESPACE_FOUNDATION::Log::error("Failed to create GLFW Window");
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
}

#endif // !SP_OPENGL_HEADER
