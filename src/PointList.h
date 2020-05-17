#ifndef POINT_LIST_HEADER
#define POINT_LIST_HEADER

#include "SpectrumRendering.h"
#include <vector>

namespace NAMESPACE_RENDERING
{
	class PointListStruct
	{
	private:
		std::vector<GLfloat> points;
		std::vector<GLfloat> colors;
		GLfloat defaultColor[4];

	public:

		GLfloat* toArray();
		size_t getArraySize();
		void addPoint(float x, float y, float z);
		void addPoint(float x, float y, float z, float redColorFactor, float greenColorFactor, float blueColorFactor, float alphaFactor);
		size_t getPointCount();
		void setDefaultColor(GLfloat* color4f);
	};

	class PointList
	{
	private:
		Mat4 modelView = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};

		GLuint programShader;
		PointListStruct attributes;

		GLuint pointVBO;

		GLint modelViewLocation;
		GLint projectionViewLocation;
		GLint positionAttribute;
		GLint colorAttribute;
		GLint pointSizeLocation;

		float pointSize = 3.0f;

		void initVBO();
		void setUpPositionAttribute();
		void setUpColorAttribute();

	public:
		~PointList();

		void init(PointListStruct attributes);

		void setPointSize(float newPointSize);

		void render(Mat4 projectionViewMatrix);
	};
}

#endif // POINT_LIST_HEADER