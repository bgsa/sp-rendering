#include "DOP18ListRenderer.h"

#define DOP18_LINES_LENGTH   (48)
#define DOP18_INDEXES_LENGTH (DOP18_LINES_LENGTH * 2)
#define DOP18_INDEXES_SIZE   (DOP18_INDEXES_LENGTH * SIZEOF_UINT)
#define DOP18_VERTEX_LENGTH  (32)
#define DOP18_VERTEX_SIZE    (DOP18_VERTEX_LENGTH * VEC3_SIZE)

namespace NAMESPACE_RENDERING
{
	DOP18ListRenderer::DOP18ListRenderer()
		: GraphicObject3DList()
	{
	}

	void updatePoints(Vec3* points, sp_size& pointIndex, const Plane3D& plane1, const Plane3D& plane2, const Plane3D& plane3)
	{
		Line3D* line = plane1.findIntersection(plane2);
		Vec3* point = line->findIntersectionOnRay(plane3);

		points[pointIndex].x = point->x;
		points[pointIndex].y = point->y;
		points[pointIndex].z = point->z;
		pointIndex++;

		ALLOC_RELEASE(line);
	}

	void updatePoints(Vec3* points, sp_size& pointIndex,
		const Plane3D& plane1, const Plane3D& plane2,
		const Plane3D& plane3, const Plane3D& plane4)
	{
		Line3D* line = plane1.findIntersection(plane2);
		Vec3* point1 = line->findIntersectionOnRay(plane3);
		Vec3* point2 = line->findIntersectionOnRay(plane4);

		points[pointIndex].x = point1->x;
		points[pointIndex].y = point1->y;
		points[pointIndex].z = point1->z;
		pointIndex++;

		points[pointIndex].x = point2->x;
		points[pointIndex].y = point2->y;
		points[pointIndex].z = point2->z;
		pointIndex++;

		ALLOC_RELEASE(line);
	}

	void updatePoints(DOP18* dop, Vec3* points)
	{
		sp_size pointIndex = 0;

		Plane3D* planes = dop->planes();

		Plane3D leftPlane = planes[0];
		Plane3D rightPlane = planes[1];
		Plane3D upPlane = planes[2];
		Plane3D downPlane = planes[3];
		Plane3D frontPlane = planes[4];
		Plane3D depthPlane = planes[5];

		Plane3D upLeftPlane = planes[6];
		Plane3D downRightPlane = planes[7];
		Plane3D upRightPlane = planes[8];
		Plane3D downLeftPlane = planes[9];
		Plane3D upFrontPlane = planes[10];
		Plane3D downDepthPlane = planes[11];
		Plane3D upDepthPlane = planes[12];
		Plane3D downFrontPlane = planes[13];
		Plane3D leftDepthPlane = planes[14];
		Plane3D rightFrontPlane = planes[15];
		Plane3D rightDepthPlane = planes[16];
		Plane3D leftFrontPlane = planes[17];

		// axis aligned
		updatePoints(points, pointIndex, frontPlane, leftFrontPlane, upFrontPlane, downFrontPlane); // left-front line and vertexes
		updatePoints(points, pointIndex, frontPlane, rightFrontPlane, upFrontPlane, downFrontPlane);  // right-front line and vertexes
		updatePoints(points, pointIndex, depthPlane, leftDepthPlane, upDepthPlane, downDepthPlane);  // left-depth line and vertexes
		updatePoints(points, pointIndex, depthPlane, rightDepthPlane, upDepthPlane, downDepthPlane); // right-depth line and vertexes
		updatePoints(points, pointIndex, leftPlane, leftFrontPlane, upLeftPlane, downLeftPlane);
		updatePoints(points, pointIndex, leftPlane, leftDepthPlane, upLeftPlane, downLeftPlane);
		updatePoints(points, pointIndex, rightPlane, rightFrontPlane, upRightPlane, downRightPlane);
		updatePoints(points, pointIndex, rightPlane, rightDepthPlane, upRightPlane, downRightPlane);
		//top
		updatePoints(points, pointIndex, upPlane, upLeftPlane, upFrontPlane, upDepthPlane); // left-top line and vertexes
		updatePoints(points, pointIndex, upPlane, upRightPlane, upFrontPlane, upDepthPlane); // right-top line and vertexes
		//bottom
		updatePoints(points, pointIndex, downPlane, downLeftPlane, downFrontPlane, downDepthPlane);
		updatePoints(points, pointIndex, downPlane, downRightPlane, downFrontPlane, downDepthPlane);
		//top-left-front vertex
		updatePoints(points, pointIndex, leftFrontPlane, upFrontPlane, upLeftPlane);
		//top-right-front vertex
		updatePoints(points, pointIndex, rightFrontPlane, upFrontPlane, upRightPlane);
		//top-left-depth vertex
		updatePoints(points, pointIndex, leftDepthPlane, upDepthPlane, upLeftPlane);
		//top-right-depth vertex
		updatePoints(points, pointIndex, rightDepthPlane, upDepthPlane, upRightPlane);
		//down-left-front vertex
		updatePoints(points, pointIndex, downFrontPlane, leftFrontPlane, downLeftPlane);
		//down-right-front vertex
		updatePoints(points, pointIndex, downFrontPlane, rightFrontPlane, downRightPlane);
		//down-left-depth vertex
		updatePoints(points, pointIndex, downDepthPlane, leftDepthPlane, downLeftPlane);
		//down-right-depth vertex
		updatePoints(points, pointIndex, downDepthPlane, rightDepthPlane, downRightPlane);

		sp_mem_delete(planes, Plane3D);
	}

	const static sp_uint indexesPerKdop[DOP18_INDEXES_LENGTH] = {
		0u, 1u, 2u, 3u, 0u, 2u, 1u, 3u, //front
		4u, 5u, 6u, 7u, 4u, 6u, 5u, 7u, // depth
		8u, 9u, 10u, 11u, 8u, 10u, 9u, 11u, // left
		12u, 13u, 14u, 15u, 12u, 14u, 13u, 15u, // right
		16u, 17u, 18u, 19u, 16u, 18u, 17u, 19u, // up
		20u, 21u, 22u, 23u, 20u, 22u, 21u, 23u, // down
		24u, 0u, 24u, 8u, 24u, 16u, // up-left
		25u, 2u, 25u, 12u, 25u, 18u,
		26u, 4u, 26u, 10u, 26u, 17u,
		27u, 6u, 27u, 14u, 27u, 19u,
		28u, 1u, 28u, 9u, 28u, 20u,
		29u, 3u, 29u, 13u, 29u, 22u,
		30u, 5u, 30u, 11u, 30u, 21u,
		31u, 7u, 31u, 15u, 31u, 23u
	};

	void DOP18ListRenderer::initIndexBuffer()
	{
		_indexesBuffer = sp_mem_new(OpenGLBuffer)(GL_ELEMENT_ARRAY_BUFFER);
	}

	void DOP18ListRenderer::initVertexBuffer()
	{
		_buffer = sp_mem_new(OpenGLBuffer)(GL_ARRAY_BUFFER);
	}

	//sp_bool inited = false;
	void DOP18ListRenderer::updateVertexBuffer(DOP18* listOfDOP18, sp_uint length)
	{
		//if (!inited)
		//{
		DOP18 dop;
		Vec3* points = ALLOC_ARRAY(Vec3, DOP18_VERTEX_LENGTH * length);
		Vec3* lines = ALLOC_ARRAY(Vec3, DOP18_INDEXES_LENGTH * length);

		sp_uint lineCount = 0u;
		for (sp_uint i = ZERO_UINT; i < length; i++)
		{
			updatePoints(&listOfDOP18[i], &points[i * DOP18_VERTEX_LENGTH]);

			for (sp_uint j = 0u; j < DOP18_INDEXES_LENGTH; j++)
				lines[lineCount++] = points[(i * DOP18_VERTEX_LENGTH) + indexesPerKdop[j]];
		}
			
		_buffer
			->use()
			->setData(DOP18_INDEXES_LENGTH * VEC3_SIZE * length, lines, GL_DYNAMIC_DRAW);

		ALLOC_RELEASE(points);

		//inited = true;
		//}
	}

	void DOP18ListRenderer::init()
	{
		initIndexBuffer();
		initVertexBuffer();

		shader = sp_mem_new(OpenGLShader)();
		shader
			->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\dop-18\\shader-list.vs")
			->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\dop-18\\shader-list.fs")
			->link();

		projectionMatrixLocation = shader->getUniform("projectionMatrix");
		viewMatrixLocation = shader->getUniform("viewMatrix");

		positionAttribute = shader->getAttribute("Position");
	}

	void DOP18ListRenderer::render(const RenderData& renderData, DOP18* listOfDOP18, sp_uint length)
	{
		shader
			->enable()
			->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
			->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix);

		updateVertexBuffer(listOfDOP18, length);
		_buffer->use();
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(positionAttribute);
		
		glDrawArrays(GL_LINES, 0, DOP18_INDEXES_LENGTH * length);

		shader->disable();
		_buffer->disable();
	}

	void DOP18ListRenderer::dispose()
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

		GraphicObject3DList::dispose();
	}
}

#undef DOP18_LINES_LENGTH
#undef DOP18_INDEXES_LENGTH
#undef DOP18_INDEXES_SIZE
#undef DOP18_VERTEX_LENGTH
#undef DOP18_VERTEX_SIZE