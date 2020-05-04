#include "Rock.h"
#include "RockRenderer.h"

namespace NAMESPACE_RENDERING
{
	void Rock::render(const RenderData& renderData)
	{
		shader->use();

		vertexBuffer->use();
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0);

		texture->use();
		glVertexAttribPointer(textureAttribute,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			0);

		shader->enableAttributes();

		modelView = Mat4f::createScale(7.0f, 7.0f, 7.0f);

		shader
			->setUniform<Mat4f>(projectionMatrixLocation, renderData.projectionMatrix)
			->setUniform<Mat4f>(viewMatrixLocation, renderData.viewMatrix)
			->setUniform<Mat4f>(modelViewLocation, modelView);


		glDrawElements(GL_TRIANGLES, model.sizeOfFaces(), GL_UNSIGNED_INT, model.faces->data());
		//renderer->render(renderData);

		shader->disableAttributes();
	}

	void Rock::init()
	{
		model.load("resources\\models\\rocks\\RockCustom.obj");
		vertexBuffer = sp_mem_new(OpenGLBuffer)(model.sizeOfVertexes(), model.vertexes->data());

		shader = sp_mem_new(OpenGLShader)();
		shader
			->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\rock\\shader.vs")
			->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\rock\\shader.fs")
			->link();

		projectionMatrixLocation = shader->getUniform("projectionMatrix");
		viewMatrixLocation = shader->getUniform("viewMatrix");
		modelViewLocation = shader->getUniform("modelMatrix");

		positionAttribute = shader->getAttribute("Position");
		textureAttribute = shader->getAttribute("TextureCoordinates");

		texture = sp_mem_new(OpenGLTexture)();
		ImageBMP* image = ImageBMP::load("resources\\models\\rocks\\RockCustom.bmp");
		texture->init()
			->use()
			->setProperty(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
			->setProperty(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
			->setData(image->data(), Vec2i(image->width(), image->height()));
		sp_mem_delete(image, ImageBMP);
	}

	void Rock::dispose()
	{
		if (vertexBuffer != NULL)
		{
			sp_mem_delete(vertexBuffer, OpenGLBuffer);
			vertexBuffer = NULL;
		}

		if (texture != NULL)
		{
			sp_mem_delete(texture, OpenGLTexture);
			texture = NULL;
		}

		if (shader != NULL)
		{
			sp_mem_delete(shader, OpenGLShader);
			shader = NULL;
		}

		if (renderer != NULL)
		{
			RockRenderer* r = (RockRenderer*)renderer;
			sp_mem_delete(r, RockRenderer);
		}
	}

	Rock::~Rock()
	{
		dispose();
	}

}