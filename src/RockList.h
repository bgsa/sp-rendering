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
#include "SpInertiaTensor.h"

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
		sp_int transformOffsetLocation;

		Vec3 modelInitialPosition;

		void initIndexBuffer(const ObjModel* model)
		{
			facesLength = model->faces->length();
			_indexesBuffer = sp_mem_new(OpenGLBuffer)(facesLength * 3 * SIZEOF_UINT, model->faces->data(), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
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

			const Mat3 tensor = SpInertiaTensor::sphere(2.0f, 1.0f / physicProperties(0u)->massInverse());

			for (sp_uint i = 0; i < length(); i++)
				physicProperties(i)->inertialTensor(tensor);
		}
		
		void initObject(sp_uint index)
		{
			SpPhysicProperties* physic = physicProperties(0u);
			DOP18* bvs = (DOP18*)boundingVolumes(0u);

			modelInitialPosition = Vec3(0.0f, 1.1f, 1.3f);

			physic[index].position(modelInitialPosition);
			physic[index].mass(8.0f);

			bvs[index].scale({ 2.8f, 3.0f, 3.0f });
			bvs[index].min[DOP18_AXIS_UP_DEPTH] += 1.0f;
			bvs[index].max[DOP18_AXIS_UP_DEPTH] -= 1.0f;
		}

	public:

		API_INTERFACE RockList(const sp_uint length)
			: SpPhysicObjectList::SpPhysicObjectList(length)
		{
			shader = nullptr;

			for (sp_uint i = 0; i < length; i++)
				initObject(i);
		}

		API_INTERFACE inline void translate(const sp_uint index, const Vec3& translation) override
		{
			transforms(index)->translate(translation);
			boundingVolumes(index)->translate(translation);
			physicProperties(index)->position(translation);
		}

		API_INTERFACE inline void rotate(const sp_uint index, const Quat& rotation)
		{
			Quat newOrientation = transforms(index)->orientation * rotation;

			transforms(index)->orientation = newOrientation;
			physicProperties(index)->orientation(newOrientation);
		}

		API_INTERFACE inline void scale(const sp_uint index, const Vec3& factors) override
		{
			transforms(index)->scale(factors);
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
			transformOffsetLocation = shader->getUniform("transformOffset");
			
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
				->setUniform<sp_float>(shininessFactorLocation, 1000.0f)
				->setUniform<sp_uint>(transformOffsetLocation, physicIndex);

			_buffer->use();
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionAttribute);
			glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexesLength * VEC3_LENGTH * SIZEOF_FLOAT));
			glEnableVertexAttribArray(normalAttribute);

			SpPhysicSimulator::instance()->transformsGPU()->use();
			_indexesBuffer->use();
			
			glDrawElementsInstanced(GL_TRIANGLES, facesLength * THREE_SIZE, GL_UNSIGNED_INT, NULL, length());
			
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