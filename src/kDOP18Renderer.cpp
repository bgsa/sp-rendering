#include "kDOP18Renderer.h"
#include <cstdlib>
#include <vector>
#include "Shader.h"

#define FLOAT_COUNT_PER_18DOP 96 /* (32 * 3) */
#define FLOAT_SIZE_PER_18DOP (SIZEOF_FLOAT * FLOAT_COUNT_PER_18DOP)
#define VERTEXES_PER_18DOP 32

namespace NAMESPACE_RENDERING
{
	void kDOP18Renderer::init()
	{
		const char* vertexShaderSource = "#version 300 es \n"
			"uniform mat4 projectionMatrix; \n"
			"uniform mat4 viewMatrix; \n"
			"uniform mat4 modelMatrix; \n"

			"in  vec3 Position; \n"

			"void main() \n"
			"{											\n"
			"	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(Position, 1.0); \n"
			"}											\n";

		const char* fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"

			"out vec4 FragColor; \n"

			"void main() \n"
			"{\n"
			"	FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
			"}";

		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		initVBO();
	}

	void kDOP18Renderer::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0);

		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	/*
	const int vertexCount = 96;
	float points[vertexCount * 3] = {

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

	void updatePoints(float* points, size_t& index, const Plane3D& plane1, const Plane3D& plane2, const Plane3D& plane3)
	{
		Line3D* line = plane1.findIntersection(plane2);
		Vec3f* point = line->findIntersectionOnRay(plane3);

		points[index++] = point->x;
		points[index++] = point->y;
		points[index++] = point->z;

		ALLOC_RELEASE(line);
	}
	void updatePoints(float* points, size_t& index,
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
	void updatePoints(DOP18* dop, float* points, size_t pointIndex)
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

	void kDOP18Renderer::setObjects(kDOP18* kdops, size_t kdopsCount)
	{
		this->kdops = kdops;
		this->kdopsCount = kdopsCount;

		float* points = ALLOC_ARRAY(float, FLOAT_COUNT_PER_18DOP * kdopsCount);
		size_t pointIndex = 0;

		for (size_t i = 0; i < kdopsCount; i++, pointIndex += FLOAT_COUNT_PER_18DOP)
			updatePoints((DOP18*)kdops[i].boundingVolume, points, pointIndex);

		glGenBuffers(1, &vertexBufferObject);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, FLOAT_SIZE_PER_18DOP * kdopsCount, points, GL_STATIC_DRAW);  //insere os dados no buffer para usar glDraw*

		ALLOC_RELEASE(points);
	}

	void kDOP18Renderer::initVBO()
	{
		projectionMatrixLocation = glGetUniformLocation(programShader, "projectionMatrix");
		viewMatrixLocation = glGetUniformLocation(programShader, "viewMatrix");
		modelMatrixLocation = glGetUniformLocation(programShader, "modelMatrix");

		positionAttribute = glGetAttribLocation(programShader, "Position");

		setUpPositionAttribute();
	}

	void kDOP18Renderer::render(const RenderData& renderData)
	{
		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

		setUpPositionAttribute();

		Mat4f modelView = Mat4f::identity();
		//kdops[0].modelView *= Mat4f::createRotate(degreesToRadians(2), 0.0f, 1.0f, 0.0f);

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, renderData.projectionMatrix);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, renderData.viewMatrix);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, modelView);


		GLuint indicesPerKdop[FLOAT_COUNT_PER_18DOP] = {
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

		GLuint* indices = ALLOC_ARRAY(GLuint, FLOAT_COUNT_PER_18DOP * kdopsCount);

		for (size_t i = 0; i < kdopsCount; i++)
		{
			ALLOC_COPY_TO(indicesPerKdop, &indices[i*FLOAT_COUNT_PER_18DOP], GLuint, FLOAT_COUNT_PER_18DOP);

			for (size_t j = 0; j < FLOAT_COUNT_PER_18DOP; j++)
				indicesPerKdop[j] += VERTEXES_PER_18DOP;
		}

		glDrawElements(GL_LINES, FLOAT_COUNT_PER_18DOP * kdopsCount, GL_UNSIGNED_INT, indices);

		ALLOC_RELEASE(indices);
	}
}

#undef FLOAT_COUNT_PER_18DOP
#undef VERTEXES_PER_18DOP
#undef FLOAT_SIZE_PER_18DOP