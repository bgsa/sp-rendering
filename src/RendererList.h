#ifndef RENDERER_LIST_HEADER
#define RENDERER_LIST_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3DList.h"
#include "IGraphicObjectRenderer.h"
#include "OpenGLShader.h"
#include "SpPhysicSimulator.h"

namespace NAMESPACE_RENDERING
{
	class RendererList
		: public IGraphicObjectRenderer
	{
	private:
		OpenGLShader* shader;
		GLuint programShader;
		GLuint vertexBufferObject;
		GLuint modelBufferObject;

		GLint projectionMatrixLocation;
		GLint viewMatrixLocation;

		GLint positionAttribute = -1;
		GLint transformLocation = -1;

		GraphicObject3DList* _list;

		inline void setUpPositionAttribute()
		{
			glVertexAttribPointer(positionAttribute,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				0);

			glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
		}

	public:

		API_INTERFACE void setList(GraphicObject3DList* list)
		{
			this->_list = list;
		}

		API_INTERFACE void init()
		{
			shader = sp_mem_new(OpenGLShader)();
			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\renderer-list\\shader.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\renderer-list\\shader.fs")
				->link();

			projectionMatrixLocation = shader->getUniform("projectionMatrix");
			viewMatrixLocation = shader->getUniform("viewMatrix");
			transformLocation = shader->getUniform("transformMatrix");

			positionAttribute = shader->getAttribute("Position");
			
			setUpPositionAttribute();
		}

		API_INTERFACE void render(const RenderData& renderData) override
		{
			_list->buffer()->use();
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

			shader
				->enable()
				->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
				->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix);

			SpPhysicSimulator::instance()->transformsGPU()->use();

			//glDrawElementsInstanced(GL_LINES, _list->indexesBuffer()->length(), GL_UNSIGNED_INT, _list->indexes()->data(), _list->length());

			shader->disable();
		}

		API_INTERFACE virtual void update() override
		{
		}

		API_INTERFACE void dispose() override
		{
			if (shader != nullptr)
			{
				sp_mem_delete(shader, OpenGLShader);
				shader = nullptr;
			}
		}

		API_INTERFACE const sp_char* toString() override
		{
			return "k-DOPs 18 Renderer";
		}

	};
}

#endif // RENDERER_LIST_HEADER