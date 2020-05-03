#ifndef OPENGL_SHADER_HEADER
#define OPENGL_SHADER_HEADER

#include "SpectrumRendering.h"
#include <SpVector.h>

namespace NAMESPACE_RENDERING
{
	class OpenGLShader
	{
	private:
		GLuint program;
		SpVector<sp_uint> shadersId;

	public:

		/// <summary>
		/// Create a new shader program for OpenGL API
		/// </summary>
		OpenGLShader()
		{
			program = glCreateProgram();

			if (program == 0)
				Log::error("glCreateProgram was not able to create a new shader program");
		}

		/// <summary>
		/// Set the OpenGL state to use this program
		/// </summary>
		API_INTERFACE inline void use()
		{
			glUseProgram(program);
		}

		/// <summary>
		/// Build/Compile the shader source from disk file
		/// </summary>
		API_INTERFACE OpenGLShader* buildFromFile(GLenum type, const sp_char* filename)
		{
			SP_FILE file;
			SpString* source = file.readTextFile(filename);

			build(type, source->data());

			sp_mem_delete(source, SpString);

			return this;
		}

		/// <summary>
		/// Build/Compile the shader source
		/// </summary>
		API_INTERFACE OpenGLShader* build(GLenum type, const sp_char* source)
		{
			GLuint shader = glCreateShader(type);

			if (shader == NULL)
				return NULL;

			glShaderSource(shader, 1, &source, nullptr);
			glCompileShader(shader);

			GLint compiled;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

			if (!compiled)
			{
				GLint infoLen = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 1)
				{
					sp_char* infoLog = ALLOC_ARRAY(sp_char, SIZEOF_CHAR * infoLen);
					glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);

					Log::error(infoLog);
					Log::error("Shader Source: ");
					Log::error(source);

					ALLOC_RELEASE(infoLog);
				}

				glDeleteShader(shader);
				return NULL;
			}

			shadersId.add(shader);

			return this;
		}

		/// <summary>
		/// Link all shaders built on shader program
		/// </summary>
		API_INTERFACE void link()
		{
			for (SpVectorItem<sp_uint>* item = shadersId.begin(); item != NULL; item = item->next())
				glAttachShader(program, item->value());

			glLinkProgram(program);

			GLint linked;
			glGetProgramiv(program, GL_LINK_STATUS, &linked);

			if (!linked)
			{
				GLint infoLen = ZERO_INT;

				glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

				if (infoLen > 1)
				{
					sp_char *infoLog = ALLOC_ARRAY(sp_char, SIZEOF_CHAR * infoLen);
					glGetProgramInfoLog(program, infoLen, nullptr, infoLog);

					Log::error(infoLog);

					ALLOC_RELEASE(infoLog);
				}

				glDeleteProgram(program);
			}

			for (SpVectorItem<sp_uint>* item = shadersId.begin(); item != NULL; item = item->next())
			{
				glDetachShader(program, item->value());
				glDeleteShader(item->value());
			}

			shadersId.dispose();
		}

		/// <summary>
		/// Get the uniform location from a shader
		/// </summary>
		API_INTERFACE inline sp_int getUniform(const sp_char* name)
		{
			return glGetUniformLocation(program, name);
		}

		/// <summary>
		/// Get the attribute location from a shader
		/// </summary>
		API_INTERFACE inline sp_int getAttribute(const sp_char* name)
		{
			return glGetAttribLocation(program, name);
		}

		/// <summary>
		/// Set the uniform value of shader. Use the template to Work! setUniform<Mat4f>(matrix)
		/// </summary>
		template <typename T>
		API_INTERFACE inline OpenGLShader* setUniform(const GLint id, const T& value)
		{
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader for Mat4f value
		/// </summary>
		template <>
		API_INTERFACE inline OpenGLShader* setUniform(const GLint id, const Mat4f& value)
		{
			glUniformMatrix4fv(id, ONE_INT, GL_FALSE, value);
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader for Mat3f value
		/// </summary>
		template <>
		API_INTERFACE inline OpenGLShader* setUniform(const GLint id, const Mat3f& value)
		{
			glUniformMatrix3fv(id, ONE_INT, GL_FALSE, value);
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader for Mat2f value
		/// </summary>
		template <>
		API_INTERFACE inline OpenGLShader* setUniform(const GLint id, const Mat2f& value)
		{
			glUniformMatrix3fv(id, ONE_INT, GL_FALSE, value);
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader for Mat2f value
		/// </summary>
		template <>
		API_INTERFACE inline OpenGLShader* setUniform(const GLint id, const Vec4f& value)
		{
			glUniform4fv(id, ONE_INT, value);
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader for Mat2f value
		/// </summary>
		template <>
		API_INTERFACE inline OpenGLShader* setUniform(const GLint id, const Vec3f& value)
		{
			glUniform3fv(id, ONE_INT, value);
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader for Mat2f value
		/// </summary>
		template <>
		API_INTERFACE inline OpenGLShader* setUniform(const GLint id, const Vec2f& value)
		{
			glUniform2fv(id, ONE_INT, value);
			return this;
		}

		/// <summary>
		/// Destruct the shader
		/// </summary>
		API_INTERFACE ~OpenGLShader()
		{
			if (program != NULL)
				glDeleteProgram(program);
		}
	};
}

#endif // OPENGL_SHADER_HEADER