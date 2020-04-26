#ifndef ROCK_HEADER
#define ROCK_HEADER

#include "SpectrumRendering.h"
#include "GraphicObject3D.h"
#include "DOP18.h"
#include "IRenderer.h"
#include <FileSystem.h>

namespace NAMESPACE_RENDERING
{
	class Rock
		: public GraphicObject3D<Rock, DOP18>
	{
	public:

		API_INTERFACE void init() override
		{
			SP_FILE file;
			file.open("asd", std::ios::in);
			const sp_size fileSize = file.length();
			sp_char* content = ALLOC_ARRAY(sp_char, fileSize);
			file.read(content, fileSize);

			//content

			ALLOC_RELEASE(content);
		}

		API_INTERFACE void update(long long elapsedTime) override
		{
		}

		API_INTERFACE void render(const RenderData& renderData) override;

		API_INTERFACE inline const sp_char* toString() override
		{
			return "Rock";
		}
		
	};
}

#endif // ROCK_HEADER