#include "kDOP18List.h"

#define INDEXES_COUNT_PER_18DOP (96) /* (32 * 3) */
#define SIZE_PER_18DOP (SIZEOF_UINT * INDEXES_COUNT_PER_18DOP)
#define VERTEXES_PER_18DOP (INDEXES_COUNT_PER_18DOP * 3)

namespace NAMESPACE_RENDERING
{
	/*
	static sp_float points[VERTEXES_PER_18DOP] = {
		//front face
		0.0f, 0.0f, 0.0f,
		0.0f, 10.0f, 0.0f,

		10.0f, 10.0f, 0.0f,
		10.0f, 0.0f, 0.0f,

		0.0f, 10.0f, 0.0f,
		10.0f, 10.0f, 0.0f,

		0.0f, 0.0f, 0.0f,
		10.0f, 0.0f, 0.0f,

		//front-face and front-back-face connection
		0.0f, 0.0f, 0.0f,
		-3.0f, -3.0f, 3.0f,

		0.0f, 10.0f, 0.0f,
		-3.0f, 13.0f, 3.0f,

		10.0f, 0.0f, 0.0f,
		13.0f, -3.0f, 3.0f,

		10.0f, 10.0f, 0.0f,
		13.0f, 13.0f, 3.0f,

		//back face
		0.0f, 0.0f, 13.0f,
		0.0f, 10.0f, 13.0f,

		10.0f, 10.0f, 13.0f,
		10.0f, 0.0f, 13.0f,

		0.0f, 10.0f, 13.0f,
		10.0f, 10.0f, 13.0f,

		0.0f, 0.0f, 13.0f,
		10.0f, 0.0f, 13.0f,

		//back-face and back-back-face connection
		0.0f, 0.0f, 13.0f,
		-3.0f, -3.0f, 10.0f,

		0.0f, 10.0f, 13.0f,
		-3.0f, 13.0f, 10.0f,

		10.0f, 0.0f, 13.0f,
		13.0f, -3.0f, 10.0f,

		10.0f, 10.0f, 13.0f,
		13.0f, 13.0f, 10.0f,

		//left-front face
		16.0f, 10.0f, 6.0f,
		16.0f, 0.0f, 6.0f,

		16.0f, 10.0f, 7.0f,
		16.0f, 0.0f, 7.0f,

		16.0f, 10.0f, 6.0f,
		16.0f, 10.0f, 7.0f,

		16.0f, 0.0f, 6.0f,
		16.0f, 0.0f, 7.0f,

		//connection left-front face with back-faces
		16.0f, 0.0f, 6.0f,
		13.0f, -3.0f, 3.0f,

		16.0f, 10.0f, 6.0f,
		13.0f, 13.0f, 3.0f,

		16.0f, 10.0f, 7.0f,
		13.0f, 13.0f, 10.0f,

		16.0f, 0.0f, 7.0f,
		13.0f, -3.0f, 10.0f,

		//right-front face
		-6.0f, 10.0f, 6.0f,
		-6.0f, 0.0f, 6.0f,

		-6.0f, 10.0f, 7.0f,
		-6.0f, 0.0f, 7.0f,

		-6.0f, 10.0f, 6.0f,
		-6.0f, 10.0f, 7.0f,

		-6.0f, 0.0f, 6.0f,
		-6.0f, 0.0f, 7.0f,

		//connection right-front face with back-faces
		-6.0f, 0.0f, 6.0f,
		-3.0f, -3.0f, 3.0f,

		-6.0f, 10.0f, 6.0f,
		-3.0f, 13.0f, 3.0f,

		-6.0f, 10.0f, 7.0f,
		-3.0f, 13.0f, 10.0f,

		-6.0f, 0.0f, 7.0f,
		-3.0f, -3.0f, 10.0f,

		//up-front face
		10.0f, 16.0f, 6.0f,
		10.0f, 16.0f, 7.0f,

		0.0f, 16.0f, 6.0f,
		0.0f, 16.0f, 7.0f,

		10.0f, 16.0f, 6.0f,
		0.0f, 16.0f, 6.0f,

		10.0f, 16.0f, 7.0f,
		0.0f, 16.0f, 7.0f,

		//connection up-face with back-up-faces
		13.0f, 13.0f, 3.0f,
		10.0f, 16.0f, 6.0f,

		13.0f, 13.0f, 10.0f,
		10.0f, 16.0f, 7.0f,

		-3.0f, 13.0f, 10.0f,
		0.0f, 16.0f, 7.0f,

		-3.0f, 13.0f, 3.0f,
		0.0f, 16.0f, 6.0f,

		//down-front face
		10.0f, -6.0f, 6.0f,
		10.0f, -6.0f, 7.0f,

		0.0f, -6.0f, 6.0f,
		0.0f, -6.0f, 7.0f,

		10.0f, -6.0f, 6.0f,
		0.0f, -6.0f, 6.0f,

		10.0f, -6.0f, 7.0f,
		0.0f, -6.0f, 7.0f,

		//connection down-face with back-down-faces
		13.0f, -3.0f, 3.0f,
		10.0f, -6.0f, 6.0f,

		13.0f, -3.0f, 10.0f,
		10.0f, -6.0f, 7.0f,

		-3.0f, -3.0f, 10.0f,
		0.0f, -6.0f, 7.0f,

		-3.0f, -3.0f, 3.0f,
		0.0f, -6.0f, 6.0f
	};
	*/

	kDOP18List::kDOP18List()
		: GraphicObject3DList()
	{
	}

	void updatePoints(sp_float* points, sp_size& index, const Plane3D& plane1, const Plane3D& plane2, const Plane3D& plane3)
	{
		Line3D* line = plane1.findIntersection(plane2);
		Vec3f* point = line->findIntersectionOnRay(plane3);

		points[index++] = point->x;
		points[index++] = point->y;
		points[index++] = point->z;

		ALLOC_RELEASE(line);
	}

	void updatePoints(sp_float* points, sp_size& index,
		const Plane3D& plane1, const Plane3D& plane2,
		const Plane3D& plane3, const Plane3D& plane4)
	{
		Line3D* line = plane1.findIntersection(plane2);
		Vec3f* point1 = line->findIntersectionOnRay(plane3);
		Vec3f* point2 = line->findIntersectionOnRay(plane4);

		points[index++] = point1->x;
		points[index++] = point1->y;
		points[index++] = point1->z;
		points[index++] = point2->x;
		points[index++] = point2->y;
		points[index++] = point2->z;

		ALLOC_RELEASE(line);
	}

	void updatePoints(DOP18* dop, sp_float* points, sp_size pointIndex)
	{
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
	}

	void kDOP18List::init()
	{
		const static sp_uint indexesPerKdop[INDEXES_COUNT_PER_18DOP] = {
						0, 1, 2, 3, 0, 2, 1, 3, //front
						4, 5, 6, 7, 4, 6, 5, 7, // depth
						8, 9, 10, 11, 8, 10, 9, 11, // left
						12, 13, 14, 15, 12, 14, 13, 15, // right
						16, 17, 18, 19, 16, 18, 17, 19, // up
						20, 21, 22, 23, 20, 22, 21, 23, // down
						24, 0, 24, 8, 24, 16, // up-left
						25, 2, 25, 12, 25, 18,
						26, 4, 26, 10, 26, 17,
						27, 6, 27, 14, 27, 19,
						28, 1, 28, 9, 28, 20,
						29, 3, 29, 13, 29, 22,
						30, 5, 30, 11, 30, 21,
						31, 7, 31, 15, 31, 23
		};

		_indexes = sp_mem_new(SpArray<sp_uint>)(INDEXES_COUNT_PER_18DOP, INDEXES_COUNT_PER_18DOP);
		std::memcpy(_indexes->data(), indexesPerKdop, SIZE_PER_18DOP);

		sp_float* points = ALLOC_ARRAY(sp_float, INDEXES_COUNT_PER_18DOP);
		DOP18 dop;
		updatePoints(&dop, points, ZERO_SIZE);
		_buffer = sp_mem_new(OpenGLBuffer)(INDEXES_COUNT_PER_18DOP * SIZEOF_FLOAT, points);
		ALLOC_RELEASE(points);

		_transformsAsTexture = sp_mem_new(OpenGLTextureBuffer)();
		_transformsAsTexture->setBuffer(sizeof(Mat4f) * _length, _transforms, GL_DYNAMIC_DRAW);
	}

	void kDOP18List::render(const RenderData& renderData)
	{
		GraphicObject3DList<DOP18>::render(renderData);
	}

	void kDOP18List::dispose()
	{
		if (_indexes != nullptr)
		{
			sp_mem_delete(_indexes, SpArray<sp_uint>);
			_indexes = nullptr;
		}

		GraphicObject3DList<DOP18>::dispose();
	}
}

#undef FLOAT_COUNT_PER_18DOP
#undef VERTEXES_PER_18DOP
#undef FLOAT_SIZE_PER_18DOP