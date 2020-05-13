#ifndef DEFAULT_RENDERER_MANAGER_HEADER
#define DEFAULT_RENDERER_MANAGER_HEADER

#include "SpectrumRendering.h"
#include <CollisionResponse.h>
#include "GLConfig.h"
#include "SpEventDispatcher.h"
#include "LogGL.hpp"
#include "IRendererManager.h"

#include "SphereX.h"
#include "Cube.h"
#include <OBB.h>
#include "CubeList.h"

namespace NAMESPACE_RENDERING
{
	class DefaultRendererManager
		: public IRendererManager
	{
	public:

		API_INTERFACE DefaultRendererManager()
			: IRendererManager()
		{
		}

		API_INTERFACE virtual void init(Camera* camera) override;
		API_INTERFACE void resize(sp_float width, sp_float height) override;

		API_INTERFACE void preRender() override;
		API_INTERFACE void render() override;
		API_INTERFACE void postRender() override;

		API_INTERFACE ~DefaultRendererManager();
	};
}

#endif // DEFAULT_RENDERER_MANAGER_HEADER