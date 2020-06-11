#ifndef SP_MATERIAL_HEADER
#define SP_MATERIAL_HEADER

#include "SpectrumRendering.h"

namespace NAMESPACE_RENDERING
{
	class SpMaterial
	{
	private:
		Vec3 _ambient;
		Vec3 _diffuse;
		Vec3 _specular;

		sp_float _shininessFactor = 1000.0f;

	public:

		API_INTERFACE SpMaterial()
		{
			_ambient = Vec3(1.0f, 0.0f, 0.0f);
			_diffuse = Vec3(1.0f, 1.0f, 1.0f);
			_specular = Vec3(1.0f, 1.0f, 1.0f);

			_shininessFactor = 1000.0f;
		}

		API_INTERFACE inline Vec3 ambient() const
		{
			return _ambient;
		}

		API_INTERFACE inline Vec3 diffuse() const
		{
			return _diffuse;
		}

		API_INTERFACE inline Vec3 specular() const
		{
			return _specular;
		}

		API_INTERFACE inline sp_float shininessFactor() const
		{
			return _shininessFactor;
		}

		// TODO: presets for material (wood, metal, wall, etc)
		//API_INTERFACE static SpMaterial preset(const sp_uint id) const {  }

	};
}

#endif // SP_MATERIAL_HEADER