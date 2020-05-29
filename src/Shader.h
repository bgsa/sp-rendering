#ifndef SHADER_HEADER
#define SHADER_HEADER

#include "SpectrumRendering.h"
#include <cstdlib>

namespace NAMESPACE_RENDERING
{
	class Shader
	{
	public:
		Shader();
		~Shader();

		static GLuint loadShaderProgramWithGeometry(const char * vertexShaderSource, const char * geometryShaderSource, const char * fragmentShaderSource) {
			GLuint vertexShader;
			GLuint geometryShader;
			GLuint fragmentShader;
			GLuint shaderProgram;
			GLint linked;

			// Load the vertex/fragment shaders
			vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSource);
			geometryShader = loadShader(GL_GEOMETRY_SHADER, geometryShaderSource);
			fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

			// Create the program object
			shaderProgram = glCreateProgram();

			if (shaderProgram == 0)
				return 0; //error

			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, geometryShader);
			glAttachShader(shaderProgram, fragmentShader);

			// Link the program
			glLinkProgram(shaderProgram);

			// Check the link status
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);

			if (!linked)
			{
				GLint infoLen = 0;

				glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 1)
				{
					sp_char* infoLog = ALLOC_ARRAY(sp_char, infoLen);

					glGetProgramInfoLog(shaderProgram, infoLen, nullptr, infoLog);

					Log::error(infoLog);

					ALLOC_RELEASE(infoLog);
				}

				glDeleteProgram(shaderProgram);

				return 0;
			}

			glDetachShader(shaderProgram, vertexShader);
			glDetachShader(shaderProgram, geometryShader);
			glDetachShader(shaderProgram, fragmentShader);

			glDeleteShader(vertexShader);
			glDeleteShader(geometryShader);
			glDeleteShader(fragmentShader);

			return shaderProgram;
		}

		static GLuint loadShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource) {
			GLuint vertexShader;
			GLuint fragmentShader;
			GLuint shaderProgram;
			GLint linked;

			// Load the vertex/fragment shaders
			vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderSource);
			fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

			// Create the program object
			shaderProgram = glCreateProgram();

			if (shaderProgram == 0)
				return 0; //error

			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);

			// Link the program
			glLinkProgram(shaderProgram);

			// Check the link status
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);

			if (!linked)
			{
				GLint infoLen = 0;

				glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 1)
				{
					sp_char* infoLog = ALLOC_ARRAY(sp_char, infoLen);

					glGetProgramInfoLog(shaderProgram, infoLen, nullptr, infoLog);

					Log::error(infoLog);

					ALLOC_RELEASE(infoLog);
				}

				glDeleteProgram(shaderProgram);

				return 0;
			}

			glDetachShader(shaderProgram, vertexShader);
			glDetachShader(shaderProgram, fragmentShader);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return shaderProgram;
		}

		static GLuint loadShader(GLenum type, const char * source)
		{
			GLuint shader;
			GLint compiled;

			// Create the shader object
			shader = glCreateShader(type);

			if (shader == 0)
				return 0;

			// Load the shader source
			glShaderSource(shader, 1, &source, nullptr);

			// Compile the shader
			glCompileShader(shader);

			// Check the compile status
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

			if (!compiled)
			{
				GLint infoLen = 0;

				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 1)
				{
					sp_char* infoLog = ALLOC_ARRAY(sp_char, infoLen);

					glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);

					Log::error(infoLog);
					Log::error("Shader Source: ");
					Log::error(source);

					ALLOC_RELEASE(infoLog);
				}

				glDeleteShader(shader);

				return 0;
			}

			return shader;
		}

	};
}

#endif // SHADER_HEADER