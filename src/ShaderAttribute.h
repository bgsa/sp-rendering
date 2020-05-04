#ifndef SHADER_ATTRIBUTE_HEADER
#define SHADER_ATTRIBUTE_HEADER

#include "SpectrumRendering.h"
#include "SpString.h"

namespace NAMESPACE_RENDERING
{
	class ShaderAttribute
	{
	public:
		sp_int location;
		sp_int type;
		SpString* name;

		ShaderAttribute()
		{
		}

		ShaderAttribute(sp_int location, sp_int type, SpString* name)
		{
			this->location = location;
			this->type = type;
			this->name = name;
		}
	};
}

#endif // SHADER_ATTRIBUTE_HEADER