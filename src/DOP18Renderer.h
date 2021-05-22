#ifndef DOP18_RENDERER_HEADER
#define DOP18_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "BoundingVolumeRenderer.h"
#include "DOP18.h"
#include "OpenGLBuffer.h"
#include "OpenGLShader.h"

namespace NAMESPACE_RENDERING
{
	#define INDEXES_COUNT_PER_18DOP (96)
	#define SIZE_PER_18DOP (sizeof(sp_uint) * INDEXES_COUNT_PER_18DOP)
	#define VERTEXES_PER_18DOP (INDEXES_COUNT_PER_18DOP * 3)  /* (32 * 3) */

	class DOP18Renderer
		: public BoundingVolumeRenderer
	{
	private:
		OpenGLBuffer* vertexBuffer;
		OpenGLBuffer* indecesBuffer;
		OpenGLShader* shader;

		GLint projectionMatrixLocation;
		GLint viewMatrixLocation;
		GLint positionAttribute = -1;

		void updatePoints(sp_float* points, sp_size& index, const Plane& plane1, const Plane& plane2, const Plane& plane3)
		{
			Line3D line;
			plane1.intersection(plane2, &line);

			Vec3 point;
			line.intersectionOnRay(plane3, &point);

			points[index++] = point.x;
			points[index++] = point.y;
			points[index++] = point.z;
		}

		void updatePoints(sp_float* points, sp_size& index,
			const Plane& plane1, const Plane& plane2,
			const Plane& plane3, const Plane& plane4)
		{
			Line3D line;
			plane1.intersection(plane2, &line);

			Vec3 point1;
			line.intersectionOnRay(plane3, &point1);

			Vec3 point2;
			line.intersectionOnRay(plane4, &point2);

			points[index++] = point1.x;
			points[index++] = point1.y;
			points[index++] = point1.z;
			points[index++] = point2.x;
			points[index++] = point2.y;
			points[index++] = point2.z;
		}

		void updatePoints(DOP18* dop, sp_float* points, sp_size pointIndex)
		{
			Plane* planes = dop->planes();

			Plane leftPlane = planes[0];
			Plane rightPlane = planes[1];
			Plane upPlane = planes[2];
			Plane downPlane = planes[3];
			Plane frontPlane = planes[4];
			Plane depthPlane = planes[5];

			Plane upLeftPlane = planes[6];
			Plane downRightPlane = planes[7];
			Plane upRightPlane = planes[8];
			Plane downLeftPlane = planes[9];
			Plane upFrontPlane = planes[10];
			Plane downDepthPlane = planes[11];
			Plane upDepthPlane = planes[12];
			Plane downFrontPlane = planes[13];
			Plane leftDepthPlane = planes[14];
			Plane rightFrontPlane = planes[15];
			Plane rightDepthPlane = planes[16];
			Plane leftFrontPlane = planes[17];

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

			sp_mem_delete(planes, Plane);
		}

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

		API_INTERFACE void init()
		{
			shader = sp_mem_new(OpenGLShader)();
			shader
				->buildFromFile(GL_VERTEX_SHADER, "resources\\shaders\\opengl\\dop-18\\shader.vs")
				->buildFromFile(GL_FRAGMENT_SHADER, "resources\\shaders\\opengl\\dop-18\\shader.fs")
				->link();

			projectionMatrixLocation = shader->getUniform("projectionMatrix");
			viewMatrixLocation = shader->getUniform("viewMatrix");
			positionAttribute = shader->getAttribute("Position");
			setUpPositionAttribute();

			const sp_uint IndexesPerKdop[INDEXES_COUNT_PER_18DOP] = {
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

			vertexBuffer = sp_mem_new(OpenGLBuffer)();
			indecesBuffer = sp_mem_new(OpenGLBuffer)(INDEXES_COUNT_PER_18DOP * sizeof(sp_float), IndexesPerKdop, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
		}

		API_INTERFACE void update() override { }

		API_INTERFACE void render(const RenderData& renderData, BoundingVolume* boundingVolume) override 
		{
			indecesBuffer->use();

			sp_float points[INDEXES_COUNT_PER_18DOP];
			DOP18* dop = (DOP18*)boundingVolume;
			updatePoints(dop, points, ZERO_SIZE);
			vertexBuffer
				->use()
				->updateData(INDEXES_COUNT_PER_18DOP * sizeof(sp_float), points, GL_DYNAMIC_DRAW);
			
			glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

			shader
				->enable()
				->enableAttributes()
				->setUniform<Mat4>(projectionMatrixLocation, renderData.projectionMatrix)
				->setUniform<Mat4>(viewMatrixLocation, renderData.viewMatrix);

			glDrawElements(GL_LINES, INDEXES_COUNT_PER_18DOP, GL_UNSIGNED_INT, NULL);

			shader->disable();
		}

		API_INTERFACE void dispose() override { }

		API_INTERFACE const sp_char* toString() override
		{
			return "18-DOP Bounding Volume Renderer";
		}

	};

	#undef FLOAT_COUNT_PER_18DOP
	#undef VERTEXES_PER_18DOP
	#undef FLOAT_SIZE_PER_18DOP
}

#endif // DOP18_RENDERER_HEADER