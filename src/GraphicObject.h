#ifndef GRAPHIC_OBJECT_HEADER
#define GRAPHIC_OBJECT_HEADER

#include "SpectrumRendering.h"
#include <Object.h>
#include "RenderData.h"
#include "SpTransform.h"
#include "BoundingVolumeRenderer.h"

namespace NAMESPACE_RENDERING
{
	enum GraphicObjectType
	{
		Type2D = 2,
		Type3D = 3
	};

	class GraphicObject
		: public Object
	{
	protected:
		sp_uint programShader;
		sp_uint vertexBufferObject;

		sp_int projectionMatrixLocation;
		sp_int viewMatrixLocation;
		sp_int transformMatrixLocation;

		sp_int positionAttribute = -1;
		sp_int colorLocation = -1;

		SpTransform transform;
		BoundingVolumeRenderer* _boundingVolumeRenderer;

	public:
		
		API_INTERFACE GraphicObject()
		{
			_boundingVolumeRenderer = nullptr;

			projectionMatrixLocation = -1;
			viewMatrixLocation = -1;
			transformMatrixLocation = -1;

			positionAttribute = -1;
			colorLocation = -1;
		}

		API_INTERFACE BoundingVolumeRenderer* boundingVolumeRenderer()
		{
			return _boundingVolumeRenderer;
		}

		API_INTERFACE void setBoundingVolumeRenderer(BoundingVolumeRenderer* renderer)
		{
			this->_boundingVolumeRenderer = renderer;
		}

		API_INTERFACE virtual void init() = 0;

		API_INTERFACE virtual void render(const RenderData& renderData) = 0;

		API_INTERFACE inline virtual GraphicObject* translate(const Vec3& translation)
		{
			transform.translate(translation);
			return this;
		}

		API_INTERFACE inline Mat4 transformation() const
		{
			return transform.toMat4();
		}

		API_INTERFACE inline virtual GraphicObject* scale(const Vec3& scaleVector)
		{
			transform.scale(scaleVector);
			return this;
		}

		API_INTERFACE virtual GraphicObjectType type() = 0;

		API_INTERFACE virtual void dispose() override
		{
			if (programShader != 0)
			{
				glDeleteProgram(programShader);
			}

			if (vertexBufferObject != 0)
			{
				glDeleteBuffers(1, &vertexBufferObject);
			}
		}

		API_INTERFACE virtual ~GraphicObject()
		{
			dispose();
		}

	};
}

#endif // GRAPHIC_OBJECT_HEADER