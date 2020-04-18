#ifndef RENDERER_DATA_HEADER
#define RENDERER_DATA_HEADER

#include "SpectrumRendering.h"

namespace NAMESPACE_RENDERING
{
	class RenderData
	{
	public:
		Mat4f projectionMatrix;
		Mat4f viewMatrix;

	};
}

#endif // RENDERER_DATA_HEADER