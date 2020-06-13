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
#include "SpLightManager.h"

namespace NAMESPACE_RENDERING
{
	class RockList
		: public GraphicObject3DList
		, public SpPhysicObjectList
	{
	private:
		OpenGLShader* shader;
		sp_uint facesLength;
		sp_uint vertexesLength;
		sp_int normalAttribute;

		sp_int lightPositionLocation;
		sp_int lightColorLocation;
		sp_int shininessFactorLocation;

		RockList* translate(const Vec3& translation) override { return nullptr; }
		RockList* scale(const Vec3& scaleVector) override { return nullptr;  }
		
		void initIndexBuffer(const ObjModel* model)
		{
			facesLength = model->faces->length();
			_indexesBuffer = sp_mem_new(OpenGLBuffer)(facesLength * 3 * SIZEOF_UINT, model->faces->data(), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		}

		void initTransformBuffer()
		{
			_transformsBuffer = sp_mem_new(OpenGLTextureBuffer)();

			Mat4* transformsAsMat4 = ALLOC_NEW_ARRAY(Mat4, MAT4_LENGTH * _length);
			SpTransform* transforms = GraphicObject3DList::transforms(0u);

			for (sp_uint i = 0; i < _length; i++)
				std::memcpy(&transformsAsMat4[i], transforms[i].toMat4(), MAT4_SIZE);

			_transformsBuffer
				->use()
				->setData(MAT4_SIZE * _length, transformsAsMat4, GL_DYNAMIC_DRAW);

			ALLOC_RELEASE(transformsAsMat4);
		}

		void initVertexBuffer(const ObjModel* model)
		{
			const sp_size allBuffersLen = model->allBuffersLength();
			sp_float* tempCpuBuffer = ALLOC_ARRAY(sp_float, allBuffersLen);
			model->allBuffers(tempCpuBuffer);
			_buffer = sp_mem_new(OpenGLBuffer)(allBuffersLen * SIZEOF_FLOAT, tempCpuBuffer);
			ALLOC_RELEASE(tempCpuBuffer);
		}

		void initBuffers()
		{
			ObjModel model;
			model.load("resources\\models\\MyRock.OBJ");

			vertexesLength = model.vertexes->length();

			initVertexBuffer(&model);
			initIndexBuffer(&model);
			initTransformBuffer();
		}

	public:

		API_INTERFACE RockList(const sp_uint length)
			: SpPhysicObjectList::SpPhysicObjectList(length)
		{
			GraphicObject3DList::setLength(length);

			SpPhysicProperties* physic = physicProperties(0u);

			DOP18* bvs = (DOP18*) boundingVolumes(0u);

			for (sp_uint i = 0; i < length; i++)
			{
				bvs[i].scale({ 2.8f, 3.0f, 3.0f });
				bvs[i].translate({ 0.2f, 1.1f, 1.3f });
				
				physic[i].position({ 0.2f, 1.0f, 1.3f });
				physic[i].mass(8.0f);
			}
		}

		API_INTERFACE inline sp_uint length() const override { return _length; }

		API_INTERFACE inline void translate(const sp_uint index, const Vec3& translation) override
		{
			GraphicObject3DList::transforms(index)->translate(translation);
			boundingVolumes(index)->translate(translation);
			physicProperties(index)->position(translation);
		}

		API_INTERFACE inline void scale(const sp_uint index, const Vec3& factors) override
		{
			GraphicObject3DList::transforms(index)->scale(factors);
			boundingVolumes(index)->scale(factors);
		}

		API_INTERFACE void init() override
		{
			initBuffers();

			shader = sp_mem_new(OpenGLShader)();
			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\rock\\shader-list.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\rock\\shader-list.fs")
				->link();

			projectionMatrixLocation = shader->getUniform("projectionMatrix");
			viewMatrixLocation = shader->getUniform("viewMatrix");
			transformMatrixLocation = shader->getUniform("transformMatrix");

			lightColorLocation = shader->getUniform("LightColor");
			lightPositionLocation = shader->getUniform("LightPosition");
			shininessFactorLocation = shader->getUniform("ShininessFactor");

			positionAttribute = shader->getAttribute("Position");
			normalAttribute = shader->getAttribute("Normal");
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			shader
				->enable()
				->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
				->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix)
				->setUniform3<sp_float>(lightPositionLocation, SpLightManager::instance()->lights()->position())
				->setUniform3<sp_float>(lightColorLocation, SpLightManager::instance()->lights()->color())
				->setUniform<sp_float>(shininessFactorLocation, 1000.0f);

			_buffer->use();
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionAttribute);
			glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexesLength * VEC3_LENGTH * SIZEOF_FLOAT));
			glEnableVertexAttribArray(normalAttribute);

			// update transfom matrixes data on GPU
			Mat4* transformsAsMat4 = ALLOC_NEW_ARRAY(Mat4, MAT4_LENGTH * _length);
			SpTransform* transforms = GraphicObject3DList::transforms(0u);
			for (sp_uint i = 0; i < _length; i++)
				std::memcpy(&transformsAsMat4[i], transforms[i].toMat4(), MAT4_SIZE);
			_transformsBuffer->use()->setData(MAT4_SIZE * _length, transformsAsMat4, GL_DYNAMIC_DRAW);
			ALLOC_RELEASE(transformsAsMat4);

			_transformsBuffer->use();
			
			_indexesBuffer->use();

			glDrawElementsInstanced(GL_TRIANGLES, facesLength * THREE_SIZE, GL_UNSIGNED_INT, NULL, _length);

			shader->disable();

			glBindVertexArray(NULL);
			glBindBuffer(GL_ARRAY_BUFFER, NULL); // Unbind
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL); // Unbind
			glBindBuffer(GL_TEXTURE_BUFFER, NULL); // Unbind
		}

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Rock List";
		}

		API_INTERFACE inline void dispose() override
		{
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

		~RockList()
		{
			dispose();
		}
		
	};
}

#endif // ROCK_LIST_HEADER