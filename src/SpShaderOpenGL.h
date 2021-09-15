#ifndef SP_SHADER_OPENGL_HEADER
#define SP_SHADER_OPENGL_HEADER

#include "SpectrumRendering.h"
#include <SpVector.h>
#include <SpArray.h>
#include "FileSystem.h"
#include "SpLogger.h"
#include "SpShader.h"

namespace NAMESPACE_RENDERING
{
#define SP_DEFAULT_SHADER_VERSION "#version 300 es\n\n"

	class SpShaderOpenGL
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

		sp_size findIncludeIndex(sp_char* text, const sp_size textLength)
		{
			sp_size index = 0;

			for (sp_size i = 0; i < textLength; i++)
			{
				if (text[i] == '#')
				{
					while (text[i] != END_OF_LINE && text[i] != END_OF_STRING)
						i++;

					index = i;
				}
			}

			return index;
		}

		sp_char* includeFiles(sp_char* text, const sp_size textLength, const sp_char* rootDir, const sp_size rootDirLength)
		{
			const sp_size prefixLength = 10;
			const sp_char prefix[prefixLength] = { '#', 'i' , 'n', 'c', 'l' , 'u', 'd', 'e', ' ' , '"'};
			sp_size prefixIndex = 0;

			sp_size previousTextLength = textLength;
			sp_char* previousText = ALLOC_ARRAY(sp_char, textLength);
			std::memcpy(previousText, text, textLength);

			sp_size newTextLength = textLength;
			sp_char* newText = ALLOC_ARRAY(sp_char, textLength);;
			std::memcpy(newText, text, textLength);
		
			for (sp_size i = 0; i < newTextLength; i++)
			{
				if (newText[i] == prefix[prefixIndex])
				{
					if (prefixIndex + 1 == prefixLength) // include found
					{
						const sp_size includeBegin = i - prefixLength + 1;
						const sp_size filenameBegin = i + 1;

						while (newText[++i] != '"');
						const sp_size filenameEnd = i - 1;

						while (newText[++i] != END_OF_LINE_LF);
						const sp_size includeEnd = i;

						const sp_size filenameSize = filenameEnd - filenameBegin + 2;
						sp_char* filename = ALLOC_ARRAY(sp_char, filenameSize);
						std::memcpy(filename, &newText[filenameBegin], filenameEnd - filenameBegin + 1);
						filename[filenameEnd - filenameBegin + 1] = END_OF_STRING;

						sp_char fullFilename[500];
						std::memcpy(fullFilename, rootDir, rootDirLength);
						fullFilename[rootDirLength] = '/';
						std::memcpy(&fullFilename[rootDirLength + 1], filename, filenameSize);

						SP_FILE file;
						const sp_size includeContentSize = file.sizeOfFile(fullFilename) + 1;

						newTextLength = newTextLength + includeContentSize - (includeEnd - includeBegin);
						newText = ALLOC_ARRAY(sp_char, newTextLength);
						std::memcpy(newText, previousText, includeBegin);

						file.readTextFile(fullFilename, &newText[includeBegin]);

						std::memcpy(&newText[includeBegin + includeContentSize - 1], &previousText[includeEnd], previousTextLength - includeEnd);

						previousTextLength = newTextLength;
						previousText = ALLOC_ARRAY(sp_char, newTextLength);
						std::memcpy(previousText, newText, newTextLength);

						prefixIndex = 0;
					}
					else
						prefixIndex++;
				}
				else
					prefixIndex = 0;
			}

			return newText;
		}

	public:

		/// <summary>
		/// Create a new shader program for OpenGL API
		/// </summary>
		API_INTERFACE inline SpShaderOpenGL()
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
		API_INTERFACE SpShaderOpenGL* buildFromFile(GLenum type, const sp_char* filename, const sp_char* includeContent = nullptr)
		{
			SP_FILE file;
			sp_size textLength = file.sizeOfFile(filename) + 1;
			sp_char* text = ALLOC_ARRAY(sp_char, textLength);
			sp_char* pointer = text;

			file.readTextFile(filename, text);

			if (!strStartWith(text, textLength, "#version", 8))
			{
				const sp_size previousTextLength = textLength;

				const sp_size shaderVersionLength = std::strlen(SP_DEFAULT_SHADER_VERSION);
				textLength += shaderVersionLength;

				sp_char* newText = ALLOC_ARRAY(sp_char, textLength);

				strInsert(text, previousTextLength, SP_DEFAULT_SHADER_VERSION, shaderVersionLength, 0, newText);
				pointer = newText;
			}

			if (includeContent != nullptr)
			{
				const sp_size previousTextLength = textLength;
				const sp_size index = findIncludeIndex(pointer, textLength);

				const sp_size includeContentLength = std::strlen(includeContent);
				textLength += includeContentLength;

				sp_char* newText = ALLOC_ARRAY(sp_char, textLength);
				
				strInsert(text, previousTextLength, includeContent, includeContentLength, index, newText);
				pointer = newText;
			}

			sp_char rootDir[500];
			SpDirectory::directoryFromFile(filename, rootDir);
			const sp_size rootDirLength = std::strlen(rootDir);
			pointer = includeFiles(pointer, textLength, rootDir, rootDirLength);

			build(type, pointer);

			ALLOC_RELEASE(text);

			return this;
		}

		/// <summary>
		/// Build/Compile the shader source
		/// </summary>
		API_INTERFACE SpShaderOpenGL* build(GLenum type, const sp_char* source)
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
			return glGetUniformLocation(program, name);
		}

		/// <summary>
		/// Get the attribute location from a shader
		/// </summary>
		API_INTERFACE inline sp_int attribute(const sp_char* name)
		{
			return glGetAttribLocation(program, name);
		}

		/// <summary>
		/// Set the uniform value of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline SpShaderOpenGL* setUniform2(const GLint id, const T* values)
		{
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline SpShaderOpenGL* setUniform3(const GLint id, const T* values)
		{
			return this;
		}

		/// <summary>
		/// Set the uniform value of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline SpShaderOpenGL* setUniform4(const GLint id, const T* values)
		{
			return this;
		}

		/// <summary>
		/// Set the uniform array of shader
		/// </summary>
		template <typename T>
		API_INTERFACE inline SpShaderOpenGL* setUniformArray(const GLint id, const T* listOfT, sp_size length)
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
		/// Set the uniform with vec3 value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">Vec3</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const Vec3& value) override
		{
			sp_opengl_check(glUniform3fv(id, ONE_INT, (sp_float*)&value));
			return this;
		}

		/// <summary>
		/// Set the uniform with vec4 value
		/// </summary>
		/// <param name="id">Uniform Id</param>
		/// <param name="value">Vec4</param>
		/// <returns>Current shader</returns>
		API_INTERFACE inline SpShader* setUniform(const sp_int id, const Vec4& value) override
		{
			sp_opengl_check(glUniform4fv(id, ONE_INT, (sp_float*)&value));
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
			sp_opengl_check(glUniform1ui64ARB(id, value));
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
		API_INTERFACE SpShaderOpenGL* enableAttributes()
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
		API_INTERFACE inline SpShader* enableVertexAttribute(const sp_uint index, sp_int size, sp_int type, sp_bool normalize, sp_size stride, const void* pointer) override
		{
			sp_opengl_check(glVertexAttribPointer(index, size, type, normalize, stride, pointer));
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

		API_INTERFACE inline SpShader* drawArray(const sp_uint primitiveTypeId, const sp_int first, const sp_size count) override
		{
			sp_opengl_check(glDrawArrays(primitiveTypeId, first, count));
			return this;
		}

		API_INTERFACE inline SpShader* drawElements(const sp_uint primitiveTypeId, const sp_size indexesLength, const sp_int indexTypeId, const void* indexes = NULL) override
		{
			sp_opengl_check(glDrawElements(primitiveTypeId, indexesLength, indexTypeId, indexes));
			return this;
		}

		API_INTERFACE inline SpShader* drawElementsInstanced(const sp_uint primitiveTypeId, const sp_size indexesLength, const sp_int indexTypeId, const void* indexes = NULL, const sp_size primitiveCount = 0) override
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

		~SpShaderOpenGL()
		{
			dispose();
		}
	};

	template <>
	API_INTERFACE inline SpShaderOpenGL* SpShaderOpenGL::setUniformArray(const GLint id, const Mat4* listOfMat4, sp_size length)
	{
		glUniformMatrix4fv(id, (GLsizei)length, GL_FALSE, (sp_float*)listOfMat4);
		return this;
	}

	template <>
	API_INTERFACE inline SpShaderOpenGL* SpShaderOpenGL::setUniformArray(const GLint id, const sp_float* listOfFloat, sp_size length)
	{
		glUniform1fv(id, (GLsizei)length, listOfFloat);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader
	/// </summary>
	template <>
	API_INTERFACE inline SpShaderOpenGL* SpShaderOpenGL::setUniform2(const GLint id, const sp_float* values)
	{
		glUniform3fv(id, ONE_SIZE, values);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader
	/// </summary>
	template <>
	API_INTERFACE inline SpShaderOpenGL* SpShaderOpenGL::setUniform3(const GLint id, const sp_float* values)
	{
		glUniform3fv(id, ONE_SIZE, values);
		return this;
	}

	/// <summary>
	/// Set the uniform value of shader
	/// </summary>
	template <>
	API_INTERFACE inline SpShaderOpenGL* SpShaderOpenGL::setUniform4(const GLint id, const sp_float* values)
	{
		glUniform4fv(id, ONE_SIZE, values);
		return this;
	}

}

#endif // SP_SHADER_OPENGL_HEADER