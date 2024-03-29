#ifndef SP_LIGHT_MANAGER_HEADER
#define SP_LIGHT_MANAGER_HEADER

#include "SpectrumRendering.h"
#include "SpLightPoint.h"

#define SP_LIGHT_MAX_SOURCE 10

namespace NAMESPACE_RENDERING
{
	class SpLightManager
	{
	private:
		sp_uint _lightLength;
		SpLightPoint _lights[SP_LIGHT_MAX_SOURCE];
		
		/// <summary>
		/// Default constructor
		/// </summary>
		SpLightManager()
		{
			_lightLength = ZERO_UINT;

			environmentLight.red = 0.3f;
			environmentLight.green = 0.3f;
			environmentLight.blue = 0.3f;
		}

	public:
		SpColorRGB environmentLight;

		/// <summary>
		/// Get the instance of the light manager
		/// </summary>
		/// <returns>Light Manager</returns>
		API_INTERFACE static SpLightManager* instance()
		{
			static SpLightManager* _instance = sp_mem_new(SpLightManager)();
			return _instance;
		}

		/// <summary>
		/// Add a light source
		/// </summary>
		/// <param name="color">Color</param>
		/// <param name="position">Position</param>
		/// <returns>void</returns>
		API_INTERFACE inline void addLight(const SpColorRGB& color, const Vec3& position)
		{
			sp_assert(_lightLength < SP_LIGHT_MAX_SOURCE, "InvalidOperationException");

			_lights[_lightLength].color(color);
			_lights[_lightLength].position(position);
			_lightLength++;
		}

		/// <summary>
		/// Get a light source
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		API_INTERFACE inline SpLightPoint* lights(const sp_uint index)
		{
			sp_assert(index < SP_LIGHT_MAX_SOURCE, "InvalidOperationException");

			return &_lights[index];
		}
		
		/// <summary>
		/// Length of the light source'
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_uint length() const
		{
			return _lightLength;
		}

	};
}

#endif // SP_LIGHT_MANAGER_HEADER