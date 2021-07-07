#ifndef OPENGL_SHADER_HEADER
#define OPENGL_SHADER_HEADER

#include "SpectrumRendering.h"
#include <SpVector.h>
#include <SpArray.h>
#include "FileSystem.h"
#include "SpLogger.h"
#include "SpShader.h"

namespace NAMESPACE_RENDERING
{
#define SP_DEFAULT_SHADER_VERSION "#version 300 es\n\n"

	class OpenGLShader
		: public SpShader
	{
	private:

		void loadAttributes()
		{
			sp_int count;
			glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);

			_attributes = sp_mem_new(SpArray<SpShaderAttribute*>)(count);

			for (sp_int i = ZERO_INT; i < count; i++)
			{
				GLint arraySize = 0;
				GLenum type = 0;
				GLsizei actualLength = 0;
				sp_char* name = (sp_char*) ALLOC_ARRAY(sp_char, SP_DEFAULT_STRING_LENGTH);

				glGetActiveAttrib(program, (GLuint)i, SP_DEFAULT_STRING_SIZE, &actualLength, &arraySize, &type, name);

				SpShaderAttribute* attr = sp_mem_new(SpShaderAttribute)();
				attr->name(name);
				attr->type = type;

				if (std::strcmp(name, "gl_InstanceID") != ZERO_INT) // ignore fake attribute of GLSL
					attr->location = attribute(attr->name());

				_attributes->add(attr);

				ALLOC_RELEASE(name);
			}
		}

		void loadUniforms()
		{
			sp_int count;
			glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);

			_uniforms = sp_mem_new(SpArray<SpShaderUniform*>)(count);

			for (sp_int i = ZERO_INT; i < count; i++)
			{
				GLint arraySize = 0;
				GLenum type = 0;
				GLsizei actualLength = 0;
				sp_char* name = (sp_char*)ALLOC_ARRAY(sp_char, SP_DEFAULT_STRING_LENGTH);

				glGetActiveUniform(program, (GLuint)i, SP_DEFAULT_STRING_SIZE, &actualLength, &arraySize, &type, name);

				SpShaderUniform* attr = sp_mem_new(SpShaderUniform)();
				attr->name(name);
				attr->type = type;
				attr->location = uniform(attr->name());

				_uniforms->add(attr);

				ALLOC_RELEASE(name);
			}
		}

	public:

		/// <summary>
		/// Create a new shader program for OpenGL API
		/// </summary>
		API_INTERFACE inline OpenGLShader()
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
		API_INTERFACE inline SpShader* enable() override
		{
			sp_opengl_check(glUseProgram(program));
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
		/// Get the uniform location from a shader
		/// </summary>
		API_INTERFACE inline sp_int uniform(const sp_char* name)
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
		API_INTERFACE inline sp_int attribute(const sp_char* name)
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
		/// Set the uniform with Mat3 value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">Mat3</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const Mat3& value) override
		{
			sp_opengl_check(glUniformMatrix3fv(id, ONE_INT, GL_FALSE, value));
			return this;
		}

		/// <summary>
		/// Set the uniform with Mat4 value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">Mat4</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const Mat4& value) override
		{
			sp_opengl_check(glUniformMatrix4fv(id, ONE_INT, GL_FALSE, value));
			return this;
		}

		/// <summary>
		/// Set the uniform with scalar int value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">Iouble</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const sp_int value) override
		{
			sp_opengl_check(glUniform1i(id, value));
			return this;
		}

		/// <summary>
		/// Set the uniform with scalar unsigned int value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">unsigned int</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const sp_uint value) override
		{
			sp_opengl_check(glUniform1ui(id, value));
			return this;
		}

#ifdef ENV_64BITS
		/// <summary>
		/// Set the uniform with scalar unsigned size value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">unsigned size</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const sp_size value) override
		{
			sp_opengl_check(glUniform1uiv(id, value));
			return this;
		}
#endif

		/// <summary>
		/// Set the uniform with scalar flaot value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">Float</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const sp_float value) override
		{
			sp_opengl_check(glUniform1f(id, value));
			return this;
		}

		/// <summary>
		/// Set the uniform with scalar double value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">Double</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const sp_double value) override
		{
			sp_opengl_check(glUniform1d(id, value));
			return this;
		}

		/// <summary>
		/// Eisable all attributes enabled. It is usually used at the begining of shader render
		/// </summary>
		API_INTERFACE OpenGLShader* enableAttributes()
		{
			for (sp_uint i = 0; i < _attributes->length(); i++)
				sp_opengl_check(glEnableVertexAttribArray(_attributes->data()[i]->location));

			return this;
		}

		/// <summary>
		/// Disable all attributes enabled. It is usually used at the end of shader render
		/// </summary>
		API_INTERFACE inline void disableAttributes()
		{
			for (sp_uint i = 0; i < _attributes->length(); i++)
				if (_attributes->data()[i]->type != 5124)
					sp_opengl_check(glDisableVertexAttribArray(_attributes->data()[i]->location));
		}

		/// <summary>
		/// Enable vertex attribute in shader
		/// </summary>
		/// <param name="index">Attribute index</param>
		/// <param name="size">Count of elements in attribute</param>
		/// <param name="type">Attribute Type</param>
		/// <param name="normalized">Is Normalized?</param>
		/// <param name="stride">Stride of elements</param>
		/// <param name="pointer">Pointer to start of element</param>
		/// <returns></returns>
		API_INTERFACE inline SpShader* enableVertexAttribute(const sp_uint index, sp_int size, sp_int type, sp_bool normalized, sp_size stride, const void* pointer) override
		{
			sp_opengl_check(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
			sp_opengl_check(glEnableVertexAttribArray(index));
			return this;
		}

		/// <summary>
		/// Disable a vertex attribute
		/// </summary>
		/// <param name="index">Attribute index</param>
		/// <returns></returns>
		API_INTERFACE inline SpShader* disableVertexAttribute(const sp_uint index) override
		{
			sp_opengl_check(glDisableVertexAttribArray(index));
			return this;
		}

		API_INTERFACE inline SpShader* drawElements(const sp_int primitiveTypeId, const sp_size indexesLength, const sp_int indexTypeId, const void* indexes = NULL) override
		{
			sp_opengl_check(glDrawElements(primitiveTypeId, indexesLength, indexTypeId, indexes));
			return this;
		}

		API_INTERFACE inline SpShader* drawElementsInstanced(const sp_int primitiveTypeId, const sp_size indexesLength, const sp_int indexTypeId, const void* indexes = NULL, const sp_size primitiveCount = 0) override
		{
			sp_opengl_check(glDrawElementsInstanced(primitiveTypeId, indexesLength, indexTypeId, indexes, primitiveCount));
			return this;
		}

		/// <summary>
		/// Disable the shader and all attributes enabled. It is usually used at the end of shader render
		/// </summary>
		API_INTERFACE inline void disable() override
		{
			disableAttributes();
			sp_opengl_check(glUseProgram(0));
		}

		/// <summary>
		/// Release all allocated resources
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void dispose() override
		{
			if (program != ZERO_UINT)
			{
				sp_opengl_check(glDeleteProgram(program));
				program = ZERO_UINT;
			}

			if (_attributes != NULL)
			{
				sp_mem_delete(_attributes, SpArray<SpShaderAttribute*>);
			}
		}

		~OpenGLShader()
		{
			dispose();
		}
	};

	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniformArray(const GLint id, const Mat4* listOfMat4, sp_size length)
	{
		glUniformMatrix4fv(id, (GLsizei)length, GL_FALSE, (sp_float*)listOfMat4);
		return this;
	}

	template <>
	API_INTERFACE inline OpenGLShader* OpenGLShader::setUniformArray(const GLint id, const sp_float* listOfFloat, sp_size length)
	{
		glUniform1fv(id, (GLsizei)length, listOfFloat);
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
