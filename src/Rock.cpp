#include "Rock.h"
#include "RockRenderer.h"

namespace NAMESPACE_RENDERING
{
	void Rock::render(const RenderData& renderData)
	{
		shader->enable();
		indexesBuffer->use();
		buffer->use();

		shader
			->enableAttributes()
			->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
			->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix)
			->setUniform<Mat4>(transformMatrixLocation, transform.toMat4());

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

		texture->use();

		//glDrawElements(GL_TRIANGLES, model.sizeOfFaces(), GL_UNSIGNED_INT, model.faces->data());
		glDrawElements(GL_TRIANGLES, facesLength*3, GL_UNSIGNED_INT, NULL);
		//renderer->render(renderData);

		shader->disable();
	}

	void Rock::init()
	{
		ObjModel model;
		model.load("resources\\models\\MyRock.OBJ");

		//sizeOfVertexes = model.sizeOfVertexes();
		//sizeOfNormals = model.sizeOfNormals();
		sizeOfVertexes = model.vertexes->length();
		sizeOfNormals = model.normals->length();
		facesLength = model.faces->length();

		const sp_size sizeAllBuffers = model.sizeOfAllBuffers();
		sp_char* tempCpuBuffer = ALLOC_ARRAY(sp_char, sizeAllBuffers);
		model.allBuffers(tempCpuBuffer);
		buffer = sp_mem_new(OpenGLBuffer)(sizeAllBuffers, tempCpuBuffer);
		ALLOC_RELEASE(tempCpuBuffer);

		indexesBuffer = sp_mem_new(OpenGLBuffer)(facesLength * 3 * SIZEOF_UINT, model.faces->data(), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

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
		
		GraphicObject3D::scale(0.5f);
		_boundingVolume = sp_mem_new(DOP18)();
		_boundingVolume->scale({ 2.0f, 1.5f, 1.5f });
		_boundingVolume->translate(0.2f, 0.6f, 0.6f);
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