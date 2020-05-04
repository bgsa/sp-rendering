#ifndef OPENGL_SHADER_HEADER
#define OPENGL_SHADER_HEADER

#include "SpectrumRendering.h"
#include <SpVector.h>
#include <SpArray.h>
#include "ShaderAttribute.h"
#include "ShaderUniform.h"

namespace NAMESPACE_RENDERING
{
	class OpenGLShader
	{
	private:
		GLuint program;
		SpVector<sp_uint> shadersId;
		SpArray<ShaderAttribute*>* attributes;
		SpArray<ShaderUniform*>* uniforms;

		void loadAttributes()
		{
			sp_int count;
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);

			attributes = sp_mem_new(SpArray<ShaderAttribute*>)(count);

			for (sp_int i = ZERO_INT; i < count; i++)
			{
				GLint arraySize = 0;
				GLenum type = 0;
				GLsizei actualLength = 0;
				sp_char* name = (sp_char*) ALLOC_ARRAY(sp_char, SP_DEFAULT_STRING_LENGTH);

				glGetActiveAttrib(program, (GLuint)i, SP_DEFAULT_STRING_SIZE, &actualLength, &arraySize, &type, name);

				ShaderAttribute* attr = sp_mem_new(ShaderAttribute)();
				attr->name = sp_mem_new(SpString)(name);
				attr->type = type;
				attr->location = getAttribute(attr->name->data());

				attributes->add(attr);

				ALLOC_RELEASE(name);
			}
		}

		void loadUniforms()
		{
			sp_int count;
			glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

			uniforms = sp_mem_new(SpArray<ShaderUniform*>)(count);

			for (sp_int i = ZERO_INT; i < count; i++)
			{
				GLint arraySize = 0;
				GLenum type = 0;
				GLsizei actualLength = 0;
				sp_char* name = (sp_char*)ALLOC_ARRAY(sp_char, SP_DEFAULT_STRING_LENGTH);

				glGetActiveUniform(program, (GLuint)i, SP_DEFAULT_STRING_SIZE, &actualLength, &arraySize, &type, name);

				ShaderUniform* attr = sp_mem_new(ShaderUniform)();
				attr->name = sp_mem_new(SpString)(name);
				attr->type = type;
				attr->location = getUniform(attr->name->data());

				uniforms->add(attr);

				ALLOC_RELEASE(name);
			}
		}

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

			loadAttributes();
			loadUniforms();
		}

		/// <summary>
		/// Get the uniforms of actived shader
		/// </summary>
		API_INTERFACE inline SpArray<ShaderUniform*>* getUniforms()
		{
			return uniforms;
		}

		/// <summary>
		/// Get the attributes of actived shader
		/// </summary>
		API_INTERFACE inline SpArray<ShaderAttribute*>* getAttributes()
		{
			return attributes;
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
		/// Eisable all attributes enabled. It is usually used at the begining of shader render
		/// </summary>
		API_INTERFACE inline void enableAttributes()
		{
			for (sp_uint i = 0; i < attributes->length(); i++)
				glEnableVertexAttribArray(attributes->data()[i]->location);
		}

		/// <summary>
		/// Disable all attributes enabled. It is usually used at the end of shader render
		/// </summary>
		API_INTERFACE inline void disableAttributes()
		{
			for (sp_uint i = 0; i < attributes->length(); i++)
				glDisableVertexAttribArray(attributes->data()[i]->location);
		}
		

		/// <summary>
		/// Destruct the shader
		/// </summary>
		API_INTERFACE ~OpenGLShader()
		{
			if (program != NULL)
			{
				glDeleteProgram(program);
				program = NULL;
			}

			if (attributes != NULL)
			{
				sp_mem_delete(attributes, SpArray<ShaderAttribute*>);
			}
		}
	};
}

#endif // OPENGL_SHADER_HEADER