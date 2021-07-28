#ifndef SP_LIGHT_POINT_HEADER
#define SP_LIGHT_POINT_HEADER

#include "SpectrumRendering.h"
#include "SpColorRGB.h"

namespace NAMESPACE_RENDERING
{
	class SpLightPoint
	{
	private:
		Vec3 _position;
		sp_bool _enabled;
		SpColorRGB _color;

	public:

		API_INTERFACE SpLightPoint()
		{
			_position = Vec3Zeros;
			_color = SpColorRGB(1.0f, 1.0f, 1.0f);
			_enabled = true;
		}

		API_INTERFACE inline Vec3& position()
		{
			return _position;
		}

		API_INTERFACE void position(const Vec3& newPosition)
		{
			_position = newPosition;
		}

		API_INTERFACE inline SpColorRGB color()
		{
			return _color;
		}

		API_INTERFACE inline void color(const SpColorRGB& newColor)
		{
			_color = newColor;
		}

		API_INTERFACE inline sp_bool isEnabled()
		{
			return _enabled;
		}

		API_INTERFACE inline void enable()
		{
			_enabled = true;
		}

		API_INTERFACE inline void disable()
		{
			_enabled = false;
		}

	};
}

#endif // SP_LIGHT_POINT_HEADER