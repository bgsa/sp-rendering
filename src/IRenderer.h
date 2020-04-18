#ifndef I_RENDERER_HEADER
#define I_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "RenderData.h"

namespace NAMESPACE_RENDERING
{
	template <class T>
	class IRenderer
	{
	public:

		virtual void setObjects(T* objects, size_t count) = 0;

		virtual void render(const RenderData& renderData) = 0;

	};
}

#endif // !I_RENDERER_HEADER