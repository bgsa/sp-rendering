#ifndef OPENGL_SHADER_HEADER
#define OPENGL_SHADER_HEADER

#include "SpectrumRendering.h"
#include <SpVector.h>
#include <SpArray.h>
#include "ShaderAttribute.h"
#include "ShaderUniform.h"
#include "FileSystem.h"
#include "SpLogger.h"

namespace NAMESPACE_RENDERING
{
#define SP_DEFAULT_SHADER_VERSION "#version 300 es\n\n"

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

				if (std::strcmp(name, "gl_InstanceID") != ZERO_INT) // ignore fake attribute of GLSL
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

			if (program == ZERO_UINT)
			{
				sp_log_error1s("glCreateProgram was not able to create a new shader program"); 
				sp_log_newline();
			}
		}

		/// <summary>
		/// Set the OpenGL state to use this program
		/// </summary>
		API_INTERFACE inline OpenGLShader* enable()
		{
			glUseProgram(program);
			return this;
		}

		/// <summary>
		/// Build/Compile the shader source from disk file
		/// </summary>
		API_INTERFACE OpenGLShader* buildFromFile(GLenum type, const sp_char* filename, const sp_char* includeContent = nullptr)
		{
			SP_FILE file;
			SpString* source = file.readTextFile(filename);

			if (!source->startWith("#version "))
				source->append(SP_DEFAULT_SHADER_VERSION);

			if (includeContent != nullptr)
			{
				source->resize(source->allocatedLength() + std::strlen(includeContent));
				source->append(includeContent, source->indexOf(END_OF_LINE) + 1);
			}

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

			if (shader == ZERO_UINT)
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
					sp_char* infoLog = ALLOC_ARRAY(sp_char, sizeof(sp_char) * infoLen);
					glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);

					sp_log_error1s(infoLog); sp_log_newline();
					sp_log_error1s("Shader Source: "); sp_log_newline();
					sp_log_error1s(source); sp_log_newline();

					ALLOC_RELEASE(infoLog);
				}

				glDeleteShader(shader);
				sp_assert(false, "BuildShaderException");
				return nullptr;
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
					sp_char *infoLog = ALLOC_ARRAY(sp_char, sizeof(sp_char) * infoLen);
					glGetProgramInfoLog(program, infoLen, nullptr, infoLog);

					sp_log_error1s(infoLog); sp_log_newline();

					ALLOC_RELEASE(infoLog);
				}

				glDeleteProgram(program);
				sp_assert(false, "LinkShaderException");
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
#ifdef DEBUG
			sp_log_info1s("Shader uniform not found: ");
			sp_log_info1s(name); sp_log_newline();
#endif
			return glGetUniformLocation(program, name);
		}

		/// <summary>
		/// Get the attribute location from a shader
		/// </summary>
		API_INTERFACE inline sp_int getAttribute(const sp_char* name)
		{
#ifdef DEBUG
			sp_log_info1s("Shader attribute not found: ");
			sp_log_info1s(name); sp_log_newline();
#endif	
			return glGetAttribLocation(program, name);
		}

		/// <summary>
		/// Set the uniform value of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline OpenGLShader* setUniform(const GLint id, const T& value)
		{
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline OpenGLShader* setUniform2(const GLint id, const T* values)
		{
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline OpenGLShader* setUniform3(const GLint id, const T* values)
		{
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline OpenGLShader* setUniform4(const GLint id, const T* values)
		{
			return this;
		}

		/// <summary>
		/// Set the uniform array of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline OpenGLShader* setUniformArray(const GLint id, const T* listOfT, sp_size length)
		{
			return this;
		}

		/// <summary>
		/// Eisable all attributes enabled. It is usually used at the begining of shader render
		/// </summary>
		API_INTERFACE OpenGLShader* enableAttributes()
		{
			for (sp_uint i = 0; i < attributes->length(); i++)
				glEnableVertexAttribArray(attributes->data()[i]->location);

			return this;
		}

		/// <summary>
		/// Disable all attributes enabled. It is usually used at the end of shader render
		/// </summary>
		API_INTERFACE inline void disableAttributes()
		{
			for (sp_uint i = 0; i < attributes->length(); i++)
				if (attributes->data()[i]->type != 5124)
					glDisableVertexAttribArray(attributes->data()[i]->location);
		}

		/// <summary>
		/// Disable the shader and all attributes enabled. It is usually used at the end of shader render
		/// </summary>
		API_INTERFACE inline void disable()
		{
			disableAttributes();
			glUseProgram(ZERO_UINT);
		}

		/// <summary>
		/// Destruct the shader
		/// </summary>
		API_INTERFACE ~OpenGLShader()
		{
			if (program != ZERO_UINT)
			{
				glDeleteProgram(program);
				program = ZERO_UINT;
			}

			if (attributes != NULL)
			{
				sp_mem_delete(attributes, SpArray<ShaderAttribute*>);
			}
		}
	};

	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniformArray(const GLint id, const Mat4* listOfMat4, sp_size length)
	{
		glUniformMatrix4fv(id, length, GL_FALSE, (sp_float*)listOfMat4);
		return this;
	}

	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniformArray(const GLint id, const sp_float* listOfFloat, sp_size length)
	{
		glUniform1fv(id, length, listOfFloat);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for Mat4 value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const Mat4& value)
	{
		glUniformMatrix4fv(id, ONE_INT, GL_FALSE, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for Mat3 value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const Mat3& value)
	{
		glUniformMatrix3fv(id, ONE_INT, GL_FALSE, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for Mat2 value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const Mat2& value)
	{
		glUniformMatrix3fv(id, ONE_INT, GL_FALSE, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for Mat2 value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const Vec4& value)
	{
		glUniform4fv(id, ONE_INT, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for Mat2 value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const Vec3& value)
	{
		glUniform3fv(id, ONE_INT, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for Mat2 value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const Vec2& value)
	{
		glUniform2fv(id, ONE_INT, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for a single float value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const sp_float& value)
	{
		glUniform1f(id, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for a single double value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const sp_double& value)
	{
		glUniform1d(id, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for a single unsigned int value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const sp_uint& value)
	{
		glUniform1ui(id, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader for a single int value
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform(const GLint id, const sp_int& value)
	{
		glUniform1i(id, value);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform2(const GLint id, const sp_float* values)
	{
		glUniform3fv(id, ONE_SIZE, values);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform3(const GLint id, const sp_float* values)
	{
		glUniform3fv(id, ONE_SIZE, values);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader
	/// </summary>
	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniform4(const GLint id, const sp_float* values)
	{
		glUniform4fv(id, ONE_SIZE, values);
		return this;
	}

}

#endif // OPENGL_SHADER_HEADER
