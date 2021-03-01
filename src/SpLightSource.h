#ifndef SP_LIGHT_SOURCE_HEADER
#define SP_LIGHT_SOURCE_HEADER

#include "SpectrumRendering.h"
#include "ColorRGBA.h"

namespace NAMESPACE_RENDERING
{
	class SpLightSource
	{
	private:
		Vec3 _position;
		sp_bool _enabled;
		ColorRGB _color;

	public:

		API_INTERFACE SpLightSource()
		{
			_position = Vec3Zeros;
			_color = ColorRGB(1.0f, 1.0f, 1.0f);
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

		API_INTERFACE inline ColorRGB color()
		{
			return _color;
		}

		API_INTERFACE inline void color(const ColorRGB& newColor)
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

#endif // SP_LIGHT_SOURCE_HEADER