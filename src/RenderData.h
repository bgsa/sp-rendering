#ifndef RENDERER_DATA_HEADER
#define RENDERER_DATA_HEADER

#include "SpectrumRendering.h"
#include "SpViewportData.h"

namespace NAMESPACE_RENDERING
{
	class RenderData
	{
	public:
		Mat4f projectionMatrix;
		Mat4f viewMatrix;
		SpViewportData* viewport;

	};
}

#endif // RENDERER_DATA_HEADER