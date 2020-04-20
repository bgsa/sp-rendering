#ifndef I_RENDERER_HEADER
#define I_RENDERER_HEADER

#include "SpectrumRendering.h"
#include "RenderData.h"

namespace NAMESPACE_RENDERING
{
	template<class T>
	class IRenderer
	{
	public:

		API_INTERFACE virtual void setObjects(T* objects, sp_uint length) = 0;

		API_INTERFACE virtual void update() = 0;

		API_INTERFACE virtual void render(const RenderData& renderData) = 0;

	};
}

#endif // !I_RENDERER_HEADER