#ifndef ROCK_LIST_HEADER
#define ROCK_LIST_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3DList.h"
#include "DOP18.h"
#include "IGraphicObjectRenderer.h"
#include "ObjModel.h"
#include "OpenGLBuffer.h"
#include "SpShaderOpenGL.h"
#include "SpPhysicObjectList.h"
#include "SpLightManager.h"
#include "SpInertiaTensor.h"
#include "SpMesh.h"

namespace NAMESPACE_RENDERING
{
	class RockList
		: public GraphicObject3DList
		, public SpPhysicObjectList
	{
	private:
		SpShaderOpenGL* shader;
		sp_uint facesLength;
		sp_uint vertexesLength;
		sp_int normalAttribute;

		sp_int lightEnvironmentLocation;
		sp_int lightPositionLocation;
		sp_int lightColorLocation;
		sp_int shininessFactorLocation;
		sp_int transformOffsetLocation;

		void initIndexBuffer(const ObjModel* model)
		{
			facesLength = model->faces->length();
			_indexesBuffer = sp_mem_new(OpenGLBuffer)(facesLength * 3 * sizeof(sp_uint), model->faces->data(), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		}

		void initVertexBuffer(const ObjModel* model)
		{
			const sp_size allBuffersLen = model->allBuffersLength();
			sp_float* tempCpuBuffer = ALLOC_ARRAY(sp_float, allBuffersLen);
			model->allBuffers(tempCpuBuffer);
			_buffer = sp_mem_new(OpenGLBuffer)(allBuffersLen * sizeof(sp_float), tempCpuBuffer);
			ALLOC_RELEASE(tempCpuBuffer);
		}

		void initMesh(const ObjModel& model)
		{
			SpPoolMemoryAllocator::main()->enableMemoryAlignment();

			SpMesh* mesh = sp_mem_new(SpMesh)();
			mesh->vertexesMesh = sp_mem_new(SpArray<SpVertexMesh*>)(model.vertexes->length());
			mesh->faces = sp_mem_new(SpArray<SpFaceMesh*>)(model.faces->length());

			for (sp_uint i = 0; i < model.vertexes->length(); i++)
				mesh->vertexesMesh->add(sp_mem_new(SpVertexMesh)(mesh, i, model.vertexes->data()[i]));

			for (sp_uint i = 0; i < model.faces->length(); i++)
			{
				SpPoint3<sp_uint> indexes = model.faces->data()[i];
				mesh->faces->add(sp_mem_new(SpFaceMesh)(mesh, i, indexes.x, indexes.y, indexes.z));
			}

			mesh->init();

			SpWorldManagerInstance->current()->mesh(physicIndex, mesh);

			SpPoolMemoryAllocator::main()->disableMemoryAlignment();
		}

		void initBuffers()
		{
			ObjModel model;
			model.load("resources/models/rocks/obj.obj");

			initVertexBuffer(&model);
			initIndexBuffer(&model);
			initMesh(model);

			vertexesLength = model.vertexes->length();

			const Mat3 tensor = SpInertiaTensor::sphere(1.0f, 1.0f / rigidBody3D(0u)->massInverse());

			for (sp_uint i = 0; i < length(); i++)
				rigidBody3D(i)->inertialTensor(tensor);
		}
		
		inline void initObject(const sp_uint index)
		{
			rigidBody3D(index)->mass(1.1f);
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
			rigidBody3D(index)->currentState.translate(translation);
		}

		API_INTERFACE inline void rotate(const sp_uint index, const Quat& rotation)
		{
			Quat newOrientation;
			multiply(transforms(index)->orientation, rotation, newOrientation);

			transforms(index)->orientation = newOrientation;
			rigidBody3D(index)->currentState.orientation(newOrientation);
		}

		API_INTERFACE inline void scale(const sp_uint index, const Vec3& factors) override
		{
			transforms(index)->scale(factors);
			boundingVolumes(index)->scale(factors);
		}

		API_INTERFACE void init() override
		{
			initBuffers();

			shader = sp_mem_new(SpShaderOpenGL)();
			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources/shaders/opengl/rock/shader-list.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources/shaders/opengl/rock/shader-list.fs")
				->link();

			projectionMatrixLocation = shader->uniform("projectionMatrix");
			viewMatrixLocation = shader->uniform("viewMatrix");
			transformOffsetLocation = shader->uniform("transformOffset");

			lightEnvironmentLocation = shader->uniform("EnvironmentLightColor");
			lightColorLocation = shader->uniform("LightColor");
			lightPositionLocation = shader->uniform("LightPosition");
			shininessFactorLocation = shader->uniform("ShininessFactor");

			positionAttribute = shader->attribute("Position");
			normalAttribute = shader->attribute("Normal");
		}

		API_INTERFACE void render(const SpRenderData& renderData) override
		{
			shader->enable();
			shader->setUniform(projectionMatrixLocation, renderData.projectionMatrix);
			shader->setUniform(viewMatrixLocation, renderData.viewMatrix);
			shader->setUniform3<sp_float>(lightEnvironmentLocation, SpLightManager::instance()->environmentLight);
			shader->setUniform3<sp_float>(lightPositionLocation, SpLightManager::instance()->lights(0u)->position());
			shader->setUniform3<sp_float>(lightColorLocation, SpLightManager::instance()->lights(0u)->color());
			shader->setUniform(shininessFactorLocation, 1000.0f);
			shader->setUniform(transformOffsetLocation, physicIndex);

			_buffer->use();
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(positionAttribute);

			if (normalAttribute != -1)
			{
				glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, (void*)(vertexesLength * VEC3_LENGTH * sizeof(sp_float)));
				glEnableVertexAttribArray(normalAttribute);
			}

			SpWorldManagerInstance->current()->transformsGPU()->use();
			_indexesBuffer->use();
			
			glDrawElementsInstanced(GL_TRIANGLES, facesLength * THREE_SIZE, GL_UNSIGNED_INT, NULL, length());
			
			shader->disable();

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind
			glBindBuffer(GL_TEXTURE_BUFFER, 0); // Unbind
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
				sp_mem_delete(shader, SpShaderOpenGL);
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