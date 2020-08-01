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
			->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix);
			//->setUniform<Mat4>(transformMatrixLocation, transform.toMat4()); // TODO: ENABLE !!!

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

		const sp_size allBuffersLength = model.allBuffersLength();
		sp_float* tempCpuBuffer = ALLOC_ARRAY(sp_float, allBuffersLength);
		model.allBuffers(tempCpuBuffer);
		buffer = sp_mem_new(OpenGLBuffer)(allBuffersLength * SIZEOF_FLOAT, tempCpuBuffer);
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
		texture->init(1u, SpSize<sp_int>(image->width(), image->height()))
			->use()
			->property(GL_TEXTURE_MIN_FILTER, GL_LINEAR)
			->property(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
			->updateData(image->data(), image->getColorFormat());
		sp_mem_delete(image, ImageBMP);
		
		_boundingVolume = sp_mem_new(DOP18)();
		_boundingVolume->scale({ 2.0f, 1.5f, 1.5f });
		_boundingVolume->translate(0.2f, 0.6f, 0.6f);
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