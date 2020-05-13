#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "SpectrumRendering.h"
#include "SpViewportData.h"

#define SP_MIN_FIELD_OF_VIEW     1.0f
#define SP_DEFAULT_FIELD_OF_VIEW 60.0f
#define SP_MAX_FIELD_OF_VIEW     90.0f

namespace NAMESPACE_RENDERING
{
	class Camera
	{
	protected:
		Mat4f projectionMatrix;
		Mat4f viewMatrix;

		Vec3f position;
		Vec3f target;
		Vec3f _up;
		Vec3f _direction;
		Vec3f _right;
		
		sp_float fieldOfView;
		sp_float aspectRatio;
		sp_float nearFrustum;
		sp_float farFrustum ;

		Vec4f nearUpperLeft, nearLowerLeft, nearUpperRight, nearLowerRight;
		Vec4f farUpperLeft, farLowerLeft, farUpperRright, farLowerRight;

		void updateViewMatrix();

	public:

		sp_float invertY;
		sp_float velocity;
		
		API_INTERFACE inline void init(const Vec3f& position, const Vec3f& target, const Vec3f& up = Vec3f(ZERO_FLOAT, ONE_FLOAT, ZERO_FLOAT));
		API_INTERFACE inline void initProjectionPerspective(const Vec3f& position, const Vec3f& target, sp_float aspectRatio);

		API_INTERFACE inline sp_float getFieldOfView() const;

		API_INTERFACE virtual void zoom(sp_float scale) = 0;
		API_INTERFACE virtual void moveForward(sp_float distance) = 0;
		API_INTERFACE virtual void moveBackward(sp_float distance) = 0;
		API_INTERFACE virtual void moveLeft(sp_float distance) = 0;
		API_INTERFACE virtual void moveRight(sp_float distance) = 0;
		API_INTERFACE virtual void lookAtHorizontal(sp_float angleInRadians) = 0;
		API_INTERFACE virtual void lookAtVertical(sp_float angleInRadians) = 0;
		API_INTERFACE virtual void rotateX(sp_float angle) = 0;
		API_INTERFACE virtual void rotateY(sp_float angle) = 0;
		API_INTERFACE virtual void rotateZ(sp_float angle) = 0;

		API_INTERFACE inline Mat4f& getProjectionMatrix() noexcept;
		API_INTERFACE inline Mat4f& getViewMatrix() noexcept;
		API_INTERFACE inline Mat4f getHUDProjectionMatrix(sp_float width, sp_float height) const;

		API_INTERFACE void setProjectionPerspective(sp_float fieldOfView, sp_float aspect, sp_float near, sp_float far);
		API_INTERFACE void setProjectionOrthographic(sp_float xMin, sp_float xMax, sp_float yMin, sp_float yMax, sp_float zMin, sp_float zMax);

		API_INTERFACE inline void updateProjectionPerspectiveAspect(sp_float aspectRatio);

		API_INTERFACE Vec3f getFromWorldToScreen(const Vec3f& vertex, const Mat4f& modelViewMatrix, const SpViewportData* viewport);
	};
}

#endif // CAMERA_HEADER