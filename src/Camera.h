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
		Mat4 projectionMatrix;
		Mat4 viewMatrix;

		Vec3 position;
		Vec3 target;
		Vec3 _up;
		Vec3 _direction;
		Vec3 _right;
		
		sp_float fieldOfView;
		sp_float aspectRatio;
		sp_float nearFrustum;
		sp_float farFrustum ;

		Vec4 nearUpperLeft, nearLowerLeft, nearUpperRight, nearLowerRight;
		Vec4 farUpperLeft, farLowerLeft, farUpperRright, farLowerRight;

		void updateViewMatrix();

	public:

		sp_float invertY;
		sp_float velocity;
		
		API_INTERFACE inline void init(const Vec3& position, const Vec3& target, const Vec3& up = Vec3(ZERO_FLOAT, ONE_FLOAT, ZERO_FLOAT));
		API_INTERFACE inline void initProjectionPerspective(const Vec3& position, const Vec3& target, sp_float aspectRatio);

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

		API_INTERFACE inline Mat4& getProjectionMatrix() noexcept;
		API_INTERFACE inline Mat4& getViewMatrix() noexcept;
		API_INTERFACE inline Mat4 getHUDProjectionMatrix(sp_float width, sp_float height) const;

		API_INTERFACE void setProjectionPerspective(sp_float fieldOfView, sp_float aspect, sp_float near, sp_float far);
		API_INTERFACE void setProjectionOrthographic(sp_float xMin, sp_float xMax, sp_float yMin, sp_float yMax, sp_float zMin, sp_float zMax);

		API_INTERFACE inline void updateProjectionPerspectiveAspect(sp_float aspectRatio);

		API_INTERFACE Vec3 getFromWorldToScreen(const Vec3& vertex, const Mat4& modelViewMatrix, const SpViewportData* viewport);
	};
}

#endif // CAMERA_HEADER