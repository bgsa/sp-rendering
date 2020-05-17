#include "Rock.h"
#include "RockRenderer.h"

namespace NAMESPACE_RENDERING
{
	void Rock::render(const RenderData& renderData)
	{
		shader->use();
		buffer->use();
		texture->use();

		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0
		);
		glVertexAttribPointer(textureAttribute,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(sizeOfVertexes + sizeOfNormals)
		);

		shader->enableAttributes();

		shader
			->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
			->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix)
			->setUniform<Mat4>(transformMatrixLocation, transform.toMat4());

		glDrawElements(GL_TRIANGLES, model.sizeOfFaces(), GL_UNSIGNED_INT, model.faces->data());
		//renderer->render(renderData);

		shader->disableAttributes();
	}

	void Rock::init()
	{
		model.load("resources\\models\\MyRock.OBJ");

		//sizeOfVertexes = model.sizeOfVertexes();
		//sizeOfNormals = model.sizeOfNormals();
		sizeOfVertexes = model.vertexes->length();
		sizeOfNormals = model.normals->length();

		const sp_size sizeAllBuffers = model.sizeOfAllBuffers();
		sp_char* tempCpuBuffer = ALLOC_ARRAY(sp_char, sizeAllBuffers);
		model.allBuffers(tempCpuBuffer);
		buffer = sp_mem_new(OpenGLBuffer)(sizeAllBuffers, tempCpuBuffer);
		ALLOC_RELEASE(tempCpuBuffer);

		shader = sp_mem_new(OpenGLShader)();
		shader
			->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\rock\\shader.vs")
			->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\rock\\shader.fs")
			->link();

		projectionMatrixLocation = shader->getUniform("projectionMatrix");
		viewMatrixLocation = shader->getUniform("viewMatrix");
		transformMatrixLocation = shader->getUniform("modelMatrix");

		positionAttribute = shader->getAttribute("Position");
		textureAttribute = shader->getAttribute("TexCoord");

		texture = sp_mem_new(OpenGLTexture)();
		ImageBMP* image = ImageBMP::load("resources\\models\\FreeRock\\FreeRock.bmp");
		texture->init()
			->use()
			->setProperty(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
			->setProperty(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
			->setData(image->data(), SpSize<sp_int>(image->width(), image->height()), image->getColorFormat());
		sp_mem_delete(image, ImageBMP);

		_boundingVolume = sp_mem_new(DOP18)();
		_boundingVolume->scale({ 2.8f, 2.9f, 2.6f });
		_boundingVolume->translate(0.2f, 1.0f, 1.3f);
	}

	Rock* Rock::translate(const Vec3& translation)
	{
		GraphicObject3D::translate(translation);

		boundingVolume()->translate(translation);

		return this;
	}

	Rock* Rock::scale(const Vec3& scaleVector)
	{
		GraphicObject3D::scale(scaleVector);

		boundingVolume()->scale(scaleVector);

		return this;
	}

	void Rock::dispose()
	{
		GraphicObject3D::dispose();

		if (buffer != NULL)
		{
			sp_mem_delete(buffer, OpenGLBuffer);
			buffer = NULL;
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
	}

	Rock::~Rock()
	{
		dispose();
	}

}