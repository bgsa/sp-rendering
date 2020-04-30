#include "Rock.h"

namespace NAMESPACE_RENDERING
{
	void Rock::render(const RenderData& renderData)
	{
		renderer->render(renderData);
	}

	void Rock::init()
	{
		SP_FILE file;
		file.open("resources\\models\\rocks\\Rock 1.obj", std::ios::in);
		const sp_size fileSize = file.length();
		sp_char* content = ALLOC_ARRAY(sp_char, fileSize);
		file.read(content, fileSize);
		file.close();

		SpText text = SpText(content);
		sp_uint vertexesLength = text.countLinesStartWith('v');
		Vec3f* vertexes = sp_mem_new(Vec3f)[vertexesLength];

		for (sp_size i = 0; i < text.length(); i++)
		{
			SpArray<SpString> values = text[i]->split(' ');

			vertexes[i] = { 
				values[1].to<sp_float>(),
				values[2].to<sp_float>(),
				values[3].to<sp_float>()
			};
		}

		ALLOC_RELEASE(content);
	}

}