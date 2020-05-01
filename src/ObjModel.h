#ifndef OBJ_MODEL_HEADER
#define OBJ_MODEL_HEADER

#include "SpectrumRendering.h"
#include "Model.h"
#include "FileSystem.h"
#include "SpText.h"

namespace NAMESPACE_RENDERING
{
	class ObjModel
		: public Model
	{
#define SP_VERTEX_PREFIX  "v "
#define SP_NORMAL_PREFIX  "vn"
#define SP_TEXTURE_PREFIX "vt"
#define SP_FACE_PREFIX    "f "

	public:

		API_INTERFACE virtual void load(const SpString& filename)
		{
			assert(filename.length() != ZERO_UINT);

			SP_FILE file;
			file.open(filename, std::ios::in);
			const sp_size fileSize = file.length();
			sp_char* content = ALLOC_ARRAY(sp_char, fileSize);
			file.read(content, fileSize);
			file.close();

			SpText text = SpText(content);
			sp_uint vertexesLength = text.countLinesStartWith(SP_VERTEX_PREFIX);
			sp_uint normalsLength = text.countLinesStartWith(SP_NORMAL_PREFIX);
			sp_uint texturesCoordLength = text.countLinesStartWith(SP_TEXTURE_PREFIX);
			sp_uint facesLength = text.countLinesStartWith(SP_FACE_PREFIX);

			assert(vertexesLength == normalsLength);

			vertexes = sp_mem_new(SpArray<Vec3f>)(vertexesLength);
			normals = sp_mem_new(SpArray<Vec3f>)(normalsLength);
			textureCoordinates = sp_mem_new(SpArray<Vec2f>)(texturesCoordLength);

			for (sp_size i = ZERO_SIZE; i < text.length(); i++)
			{
				if (text[i]->startWith("v"))
				{
					const SpArray<SpString*> values = text[i]->split(' ');

					SpString* a = values[0];
					SpString* b = values[1];
					SpString* c = values[2];
					SpString* d = values[3];

					std::cout << a->toString() << END_OF_LINE;
					std::cout << b->toString() << END_OF_LINE;
					std::cout << c->toString() << END_OF_LINE;
					std::cout << d->toString() << END_OF_LINE;

					vertexes->add({
						values[1]->to<sp_float>(),
						values[2]->to<sp_float>(),
						values[3]->to<sp_float>()
						});
				}
				/*
				else if (text[i]->startWith("vn"))
				{
					SpArray<SpString> values = text[i]->split(' ');
					normals->add({
						values[1].to<sp_float>(),
						values[2].to<sp_float>(),
						values[3].to<sp_float>()
						});
				}
				else if (text[i]->startWith("vt"))
				{
					SpArray<SpString> values = text[i]->split(' ');
					textureCoordinates->add({
						values[1].to<sp_float>(),
						values[2].to<sp_float>()
						});
				}
				*/
			}

			ALLOC_RELEASE(content);
		}

		API_INTERFACE virtual const char* toString() override
		{
			return "OBJ Model";
		}

#undef SP_VERTEX_PREFIX
#undef SP_NORMAL_PREFIX
#undef SP_TEXTURE_PREFIX
#undef SP_FACE_PREFIX
	};
}

#endif // OBJ_MODEL_HEADER