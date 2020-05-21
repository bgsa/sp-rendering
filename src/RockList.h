#ifndef ROCK_LIST_HEADER
#define ROCK_LIST_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3DList.h"
#include "DOP18.h"
#include "IGraphicObjectRenderer.h"
#include "ObjModel.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"
#include "SpPhysicObjectList.h"

namespace NAMESPACE_RENDERING
{
	class RockList
		: public GraphicObject3DList
		, public SpPhysicObjectList
	{
	private:
		OpenGLShader* shader;
		sp_uint facesLength;

		DOP18* _boundingVolumes;

		RockList* translate(const Vec3& translation) override { return nullptr; }
		RockList* scale(const Vec3& scaleVector) override { return nullptr;  }
		
		void initIndexBuffer(const ObjModel* model)
		{
			facesLength = model->faces->length();
			_indexesBuffer = sp_mem_new(OpenGLBuffer)(facesLength * 3 * SIZEOF_UINT, model->faces->data(), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		}

		void initVertexBuffer(const ObjModel* model)
		{
			const sp_size sizeAllBuffers = model->sizeOfAllBuffers();
			sp_char* tempCpuBuffer = ALLOC_ARRAY(sp_char, sizeAllBuffers);
			model->allBuffers(tempCpuBuffer);
			_buffer = sp_mem_new(OpenGLBuffer)(sizeAllBuffers, tempCpuBuffer);
			ALLOC_RELEASE(tempCpuBuffer);
		}

		void initBuffers()
		{
			ObjModel model;
			model.load("resources\\models\\MyRock.OBJ");

			initVertexBuffer(&model);
			initIndexBuffer(&model);
		}

	public:

		API_INTERFACE BoundingVolume* boundingVolumes() const override
		{
			return _boundingVolumes;
		}

		API_INTERFACE void setLength(sp_uint length) override
		{
			GraphicObject3DList::setLength(length);
			_boundingVolumes = sp_mem_new_array(DOP18, length);

			for (sp_uint i = 0; i < length; i++)
			{
				_boundingVolumes[i].scale({ 2.8f, 3.0f, 3.0f });
				_boundingVolumes[i].translate(0.2f, 1.0f, 1.3f);
			}

		}

		API_INTERFACE sp_uint length() const override
		{
			return _length;
		}

		API_INTERFACE void translate(const sp_uint index, Vec3 translation) override
		{
			_transforms->data()[index].translate(translation);
			_boundingVolumes[index].translate(translation);
		}

		API_INTERFACE void scale(const sp_uint index, Vec3 factors) override
		{
			_transforms->data()[index].scale(factors);
			_boundingVolumes[index].scale(factors);
		}

		API_INTERFACE void init() override
		{
			initBuffers();

			SpString includeShader(100);
			includeShader.add("#define LIST_LENGTH ")->add(_length)->add(END_OF_LINE);
			
			shader = sp_mem_new(OpenGLShader)();
			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\rock\\shader-list.vs", includeShader)
				->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\rock\\shader-list.fs")
				->link();

			projectionMatrixLocation = shader->getUniform("projectionMatrix");
			viewMatrixLocation = shader->getUniform("viewMatrix");
			transformMatrixLocation = shader->getUniform("transformMatrix");

			positionAttribute = shader->getAttribute("Position");
		}

		API_INTERFACE void update(long long elapsedTime) override
		{
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			Mat4* transformsAsMat4 = ALLOC_NEW_ARRAY(Mat4, MAT4_LENGTH * _length);
			SpTransform* transforms = _transforms->data();

			for (sp_uint i = 0; i < _length; i++)
				std::memcpy(&transformsAsMat4[i], transforms[i].toMat4(), MAT4_SIZE);

			shader
				->enable()
				->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
				->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix)
				->setUniformArray<Mat4>(transformMatrixLocation, transformsAsMat4, _length);

			ALLOC_RELEASE(transformsAsMat4);
			
			_buffer->use();
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionAttribute);
			
			_indexesBuffer->use();

			glDrawElementsInstanced(GL_TRIANGLES, facesLength * THREE_SIZE, GL_UNSIGNED_INT, NULL, _length);

			shader->disable();

			glBindVertexArray(NULL);
			glBindBuffer(GL_ARRAY_BUFFER, NULL); // Unbind
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL); // Unbind
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Rock List";
		}

		API_INTERFACE inline void dispose() override
		{
			if (_boundingVolumes != nullptr)
			{
				sp_mem_release(_boundingVolumes);
				_boundingVolumes = nullptr;
			}

			if (_transforms != nullptr)
			{
				sp_mem_delete(_transforms, SpArray<SpTransform>);
				_transforms = nullptr;
			}

			if (_indexesBuffer != nullptr)
			{
				sp_mem_delete(_indexesBuffer, OpenGLBuffer);
				_indexesBuffer = nullptr;
			}

			if (_buffer != nullptr)
			{
				sp_mem_delete(_buffer, OpenGLBuffer);
				_buffer = nullptr;
			}

			if (shader != nullptr)
			{
				sp_mem_delete(shader, OpenGLShader);
				shader = nullptr;
			}
		}

		API_INTERFACE ~RockList()
		{
			dispose();
		}
		
	};
}

#endif // ROCK_LIST_HEADER