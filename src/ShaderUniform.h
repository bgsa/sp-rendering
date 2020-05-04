#ifndef SHADER_UNIFORM_HEADER
#define SHADER_UNIFORM_HEADER

#include "SpectrumRendering.h"
#include "SpString.h"

namespace NAMESPACE_RENDERING
{
	class ShaderUniform
	{
	public:
		sp_int location;
		sp_int type;
		SpString* name;
	};
}

#endif // SHADER_ATTRIBUTE_HEADER