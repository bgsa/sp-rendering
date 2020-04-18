#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "SpectrumRendering.h"
#include "RendererSettings.h"

#define MIN_FIELD_OF_VIEW  1.0f
#define MAX_FIELD_OF_VIEW 90.0f

namespace NAMESPACE_RENDERING
{
	class Camera
	{
	protected:
		Mat4f projectionMatrix;
		Mat4f viewMatrix;

		Vec3f position = Vec3f(0.0f);
		Vec3f target = Vec3f(0.0f);
		Vec3f up = Vec3f(0.0f);

		float fieldOfView = MAX_FIELD_OF_VIEW;
		float aspectRatio = 0.0f;
		float nearFrustum = 1.0f;
		float farFrustum = 1000.0f;

		Vec4f nearUpperLeft, nearLowerLeft, nearUpperRight, nearLowerRight;
		Vec4f farUpperLeft, farLowerLeft, farUpperRright, farLowerRight;

		void updateViewMatrix();

	public:
		void init(const Vec3f& position, const Vec3f& target, const Vec3f& up = Vec3f(0.0f, 1.0f, 0.0f));
		void initProjectionPerspective(const Vec3f& position, const Vec3f& target, float aspectRatio);

		float getFieldOfView();

		void zoom(float scale);
		void moveForward(float distance);
		void moveBackward(float distance);
		void moveLeft(float distance);
		void moveRight(float distance);
		void lookAtHorizontal(float angleInRadians);
		void lookAtVertical(float angleInRadians);

		Mat4f& getProjectionMatrix();
		Mat4f& getViewMatrix();
		Mat4f getHUDProjectionMatrix(float width, float height);

		void setProjectionPerspective(float fieldOfView, float aspect, float near, float far);
		void setProjectionOrthographic(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, GLfloat zMin, GLfloat zMax);

		void updateProjectionPerspectiveAspect(float aspectRatio);

		Vec3f getFromWorldToScreen(Vec3f& vertex, Mat4f& modelViewMatrix);
	};
}

#endif // CAMERA_HEADER