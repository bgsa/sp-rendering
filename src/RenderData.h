#ifndef RENDERER_DATA_HEADER
#define RENDERER_DATA_HEADER

#include "SpectrumRendering.h"
#include "SpViewportData.h"
#include "SpVector.h"

namespace NAMESPACE_RENDERING
{
	class RenderData
	{
	public:
		Mat4f projectionMatrix;
		Mat4f viewMatrix;
		SpViewportData* viewport;

		SpVector<void*> customProperties = SpVector<void*>();
	};
}

#endif // RENDERER_DATA_HEADER