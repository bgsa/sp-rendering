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

		Vec3f position = Vec3f(ZERO_FLOAT);
		Vec3f target = Vec3f(ZERO_FLOAT);
		Vec3f up = Vec3f(ZERO_FLOAT);

		sp_float fieldOfView = MAX_FIELD_OF_VIEW;
		sp_float aspectRatio = ZERO_FLOAT;
		sp_float nearFrustum = ONE_FLOAT;
		sp_float farFrustum = 1000.0f;

		Vec4f nearUpperLeft, nearLowerLeft, nearUpperRight, nearLowerRight;
		Vec4f farUpperLeft, farLowerLeft, farUpperRright, farLowerRight;

		void updateViewMatrix();

	public:
		Mat4f transform;
		
		API_INTERFACE void init(const Vec3f& position, const Vec3f& target, const Vec3f& up = Vec3f(ZERO_FLOAT, ONE_FLOAT, ZERO_FLOAT));
		API_INTERFACE void initProjectionPerspective(const Vec3f& position, const Vec3f& target, sp_float aspectRatio);

		API_INTERFACE inline sp_float getFieldOfView() const;

		API_INTERFACE void zoom(sp_float scale);
		API_INTERFACE void moveForward(sp_float distance);
		API_INTERFACE void moveBackward(sp_float distance);
		API_INTERFACE void moveLeft(sp_float distance);
		API_INTERFACE void moveRight(sp_float distance);
		API_INTERFACE void lookAtHorizontal(sp_float angleInRadians);
		API_INTERFACE void lookAtVertical(sp_float angleInRadians);

		API_INTERFACE Mat4f& getProjectionMatrix();
		API_INTERFACE Mat4f& getViewMatrix();
		API_INTERFACE Mat4f getHUDProjectionMatrix(sp_float width, sp_float height);

		API_INTERFACE void setProjectionPerspective(sp_float fieldOfView, sp_float aspect, sp_float near, sp_float far);
		API_INTERFACE void setProjectionOrthographic(sp_float xMin, sp_float xMax, sp_float yMin, sp_float yMax, sp_float zMin, sp_float zMax);

		API_INTERFACE void updateProjectionPerspectiveAspect(sp_float aspectRatio);

		API_INTERFACE Vec3f getFromWorldToScreen(Vec3f& vertex, Mat4f& modelViewMatrix);
	};
}

#endif // CAMERA_HEADER