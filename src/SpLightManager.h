#ifndef SP_LIGHT_MANAGER_HEADER
#define SP_LIGHT_MANAGER_HEADER

#include "SpectrumRendering.h"
#include "SpLightSource.h"

namespace NAMESPACE_RENDERING
{
	class SpLightManager
	{
	private:
		SpLightSource* _lights;
		sp_uint _lightLength;

	public:

		API_INTERFACE static SpLightManager* instance()
		{
			static SpLightManager* _instance = sp_mem_new(SpLightManager)();
			return _instance;
		}

		API_INTERFACE inline void init(const sp_uint length = ONE_UINT)
		{
			_lights = sp_mem_new_array(SpLightSource, length);
			_lightLength = length;
		}

		API_INTERFACE inline SpLightSource* lights(const sp_uint index = ZERO_UINT)
		{
			return &_lights[index];
		}
		
		API_INTERFACE inline sp_uint length()
		{
			return _lightLength;
		}

	};
}

#endif // SP_LIGHT_MANAGER_HEADER