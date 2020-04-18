#include "Camera.h"

namespace NAMESPACE_RENDERING
{
	void Camera::init(const Vec3f& position, const Vec3f& target, const Vec3f& up)
	{
		this->position = position;
		this->target = target;
		this->up = up;

		updateViewMatrix();
	}

	void Camera::updateViewMatrix()
	{
		Vec3f cameraDirection = (position - target).normalize();   //zAxis
		Vec3f cameraRight = up.cross(cameraDirection).normalize(); //xAxis
		Vec3f cameraUp = cameraDirection.cross(cameraRight);       //yAxis

		viewMatrix = {
			cameraRight[0], cameraUp[0], cameraDirection[0], 0.0f,
			cameraRight[1], cameraUp[1], cameraDirection[1], 0.0f,
			cameraRight[2], cameraUp[2], cameraDirection[2], 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		viewMatrix *= Mat4f::createTranslate(-position[0], -position[1], -position[2]);
	}

	void Camera::initProjectionPerspective(const Vec3f& position, const Vec3f& target, float aspectRatio)
	{
		init(position, target);
		updateProjectionPerspectiveAspect(aspectRatio);
	}

	void Camera::updateProjectionPerspectiveAspect(float aspectRatio)
	{
		this->aspectRatio = aspectRatio;
		setProjectionPerspective(fieldOfView, aspectRatio, nearFrustum, farFrustum);
	}

	float Camera::getFieldOfView()
	{
		return fieldOfView;
	}

	Mat4f& Camera::getProjectionMatrix()
	{
		return projectionMatrix;
	}

	Mat4f& Camera::getViewMatrix()
	{
		return viewMatrix;
	}

	void Camera::setProjectionPerspective(float fieldOfView, float aspectRatio, float near, float far)
	{
		this->fieldOfView = fieldOfView;
		this->aspectRatio = aspectRatio;
		this->nearFrustum = near;
		this->farFrustum = far;

		float xmin, xmax, ymin, ymax;       // Dimensions of near clipping plane
		float xFmin, xFmax, yFmin, yFmax;   // Dimensions of far  clipping plane

		// Do the Math for the near clipping plane
		ymax = near * tanf(float(fieldOfView * PI_DIV_360));
		ymin = -ymax;
		xmin = ymin * aspectRatio;
		xmax = -xmin;

		// Construct the projection matrix
		projectionMatrix = Mat4f::identity();
		projectionMatrix[0] = (2.0f * near) / (xmax - xmin);
		projectionMatrix[5] = (2.0f * near) / (ymax - ymin);
		projectionMatrix[8] = (xmax + xmin) / (xmax - xmin);
		projectionMatrix[9] = (ymax + ymin) / (ymax - ymin);
		projectionMatrix[10] = -((far + near) / (far - near));
		projectionMatrix[11] = -1.0f;
		projectionMatrix[14] = -((2.0f * far * near) / (far - near));
		projectionMatrix[15] = 0.0f;

		// Do the Math for the far clipping plane
		yFmax = far * tanf(float(fieldOfView * PI_DIV_360));
		yFmin = -yFmax;
		xFmin = yFmin * aspectRatio;
		xFmax = -xFmin;

		// Fill in values for untransformed Frustum corners
		// Near Upper Left
		nearUpperLeft[0] = xmin;
		nearUpperLeft[1] = ymax;
		nearUpperLeft[2] = -near;
		nearUpperLeft[3] = 1.0f;

		// Near Lower Left
		nearLowerLeft[0] = xmin;
		nearLowerLeft[1] = ymin;
		nearLowerLeft[2] = -near;
		nearLowerLeft[3] = 1.0f;

		// Near Upper Right
		nearUpperRight[0] = xmax;
		nearUpperRight[1] = ymax;
		nearUpperRight[2] = -near;
		nearUpperRight[3] = 1.0f;

		// Near Lower Right
		nearLowerRight[0] = xmax;
		nearLowerRight[1] = ymin;
		nearLowerRight[2] = -near;
		nearLowerRight[3] = 1.0f;

		// Far Upper Left
		farUpperLeft[0] = xFmin;
		farUpperLeft[1] = yFmax;
		farUpperLeft[2] = -far;
		farUpperLeft[3] = 1.0f;

		// Far Lower Left
		farLowerLeft[0] = xFmin;
		farLowerLeft[1] = yFmin;
		farLowerLeft[2] = -far;
		farLowerLeft[3] = 1.0f;

		// Far Upper Right
		farUpperRright[0] = xFmax;
		farUpperRright[1] = yFmax;
		farUpperRright[2] = -far;
		farUpperRright[3] = 1.0f;

		// Far Lower Right
		farLowerRight[0] = xFmax;
		farLowerRight[1] = yFmin;
		farLowerRight[2] = -far;
		farLowerRight[3] = 1.0f;
	}

	void Camera::setProjectionOrthographic(GLfloat xMin, GLfloat xMax, GLfloat yMin, GLfloat yMax, GLfloat zMin, GLfloat zMax)
	{
		projectionMatrix = Mat4f::createOrthographicMatrix(xMin, xMax, yMin, yMax, zMin, zMax);

		// Fill in values for untransformed Frustum corners	// Near Upper Left
		nearUpperLeft[0] = xMin;
		nearUpperLeft[1] = yMax;
		nearUpperLeft[2] = zMin;
		nearUpperLeft[3] = 1.0f;

		// Near Lower Left
		nearLowerLeft[0] = xMin;
		nearLowerLeft[1] = yMin;
		nearLowerLeft[2] = zMin;
		nearLowerLeft[3] = 1.0f;

		// Near Upper Right
		nearUpperRight[0] = xMax;
		nearUpperRight[1] = yMax;
		nearUpperRight[2] = zMin;
		nearUpperRight[3] = 1.0f;

		// Near Lower Right
		nearLowerRight[0] = xMax;
		nearLowerRight[1] = yMin;
		nearLowerRight[2] = zMin;
		nearLowerRight[3] = 1.0f;

		// Far Upper Left
		farUpperLeft[0] = xMin;
		farUpperLeft[1] = yMax;
		farUpperLeft[2] = zMax;
		farUpperLeft[3] = 1.0f;

		// Far Lower Left
		farLowerLeft[0] = xMin;
		farLowerLeft[1] = yMin;
		farLowerLeft[2] = zMax;
		farLowerLeft[3] = 1.0f;

		// Far Upper Right
		farUpperRright[0] = xMax;
		farUpperRright[1] = yMax;
		farUpperRright[2] = zMax;
		farUpperRright[3] = 1.0f;

		// Far Lower Right
		farLowerRight[0] = xMax;
		farLowerRight[1] = yMin;
		farLowerRight[2] = zMax;
		farLowerRight[3] = 1.0f;
	}

	Mat4f Camera::getHUDProjectionMatrix(float width, float height)
	{
		return Mat4f::createOrthographicMatrix(0.0f, width, 0.0f, height, -1.0f, 1.0f);
	}

	Vec3f Camera::getFromWorldToScreen(Vec3f& vertex, Mat4f& modelViewMatrix)
	{
		float halhWidth = RendererSettings::getInstance()->getWidth() / 2.0f;
		float halhHeight = RendererSettings::getInstance()->getHeight() / 2.0f;

		//Vec4f vertex4D = Vec4f(vertex, 1.0f) * modelViewMatrix * viewMatrix * projectionMatrix;
		Vec4f vertex4D = projectionMatrix * viewMatrix * modelViewMatrix * Vec4f(vertex, 1.0f);
		vertex4D /= vertex4D[3];

		Vec3f vertexOnDeviceSpace = vertex4D.toVec3();

		return Vec3f(
			(vertexOnDeviceSpace[0] + 1.0f) * halhWidth,
			(vertexOnDeviceSpace[1] + 1.0f) * halhHeight,
			vertex4D[3]
		);
	}

	void Camera::lookAtHorizontal(float angle)
	{
		Vec3f direction = target - position;

		target = Vec3f(
			position[0] + direction[0] * cos(angle) + direction[2] * sin(angle),
			target[1],
			position[2] + direction[2] * cos(angle) + (position[0] - target[0]) * sin(angle)
		);

		updateViewMatrix();
	}

	void Camera::lookAtVertical(float angle)
	{
		Vec3f direction = target - position;
		angle *= -1.0f;

		target = Vec3f(
			target[0],
			position[1] + direction[1] * cos(angle) + (position[2] - target[2]) * sin(angle),
			position[2] + direction[1] * sin(angle) + direction[2] * cos(angle)
		);

		updateViewMatrix();
	}

	void Camera::zoom(float scale)
	{
		float newFieldOfView = fieldOfView + (fieldOfView * scale);

		if (newFieldOfView <= MIN_FIELD_OF_VIEW || newFieldOfView >= MAX_FIELD_OF_VIEW)
			return;

		fieldOfView = newFieldOfView;
		setProjectionPerspective(fieldOfView, aspectRatio, nearFrustum, farFrustum);
	}

	void Camera::moveForward(float distance)
	{
		Vec3f directionToMove = (position - target).normalize();
		directionToMove *= distance;

		position -= directionToMove;
		target -= directionToMove;

		updateViewMatrix();
	}

	void Camera::moveBackward(float distance)
	{
		Vec3f directionToMove = (position - target).normalize();
		directionToMove *= distance;

		position += directionToMove;
		target += directionToMove;

		updateViewMatrix();
	}

	void Camera::moveLeft(float distance)
	{
		Vec3f cameraDirection = (position - target).normalize();
		Vec3f directionToMove = up.cross(cameraDirection).normalize();

		Vec3f distanceToMove = directionToMove * distance;

		position -= distanceToMove;
		target -= distanceToMove;

		updateViewMatrix();
	}

	void Camera::moveRight(float distance)
	{
		Vec3f cameraDirection = (position - target).normalize();
		Vec3f directionToMove = up.cross(cameraDirection).normalize();

		Vec3f distanceToMove = directionToMove * distance;

		position += distanceToMove;
		target += distanceToMove;

		updateViewMatrix();
	}
}