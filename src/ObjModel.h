#ifndef OBJ_MODEL_HEADER
#define OBJ_MODEL_HEADER

#include "SpectrumRendering.h"
#include "Model.h"
#include "FileSystem.h"
#include "SpText.h"
#include "SpArrayOfString.h"

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
			sp_assert(filename.length() != ZERO_UINT);

			SP_FILE file;
			file.open(filename, std::ios::in);
			const sp_size fileSize = file.length();
			sp_char* content = ALLOC_ARRAY(sp_char, fileSize);
			file.read(content, fileSize);
			file.close();

			SpText text = SpText(content);
			ALLOC_RELEASE(content);

			sp_uint vertexesLength = text.countLinesStartWith(SP_VERTEX_PREFIX);
			sp_uint normalsLength = text.countLinesStartWith(SP_NORMAL_PREFIX);
			sp_uint texturesCoordLength = text.countLinesStartWith(SP_TEXTURE_PREFIX);
			sp_uint facesLength = text.countLinesStartWith(SP_FACE_PREFIX);

			sp_assert(vertexesLength == normalsLength);

			vertexes = sp_mem_new(SpArray<Vec3f>)(vertexesLength);
			normals = sp_mem_new(SpArray<Vec3f>)(normalsLength);
			textureCoordinates = sp_mem_new(SpArray<Vec2f>)(texturesCoordLength);

			for (sp_size i = ZERO_SIZE; i < text.length(); i++)
			{
				if (text[i]->startWith("v "))
				{
					const SpArrayOfString* values = text[i]->split(' ');
					SpString* numbers = values->data();

					vertexes->add({
						numbers[1].to<sp_float>(),
						numbers[2].to<sp_float>(),
						numbers[3].to<sp_float>()
					});

					sp_mem_delete(values, SpArrayOfString);
				}
				else if (text[i]->startWith("vn"))
				{
					SpArrayOfString* values = text[i]->split(' ');
					SpString* numbers = values->data();

					normals->add({
						numbers[1].to<sp_float>(),
						numbers[2].to<sp_float>(),
						numbers[3].to<sp_float>()
					});

					sp_mem_delete(values, SpArrayOfString);
				}
				else if (text[i]->startWith("vt"))
				{
					SpArrayOfString* values = text[i]->split(' ');
					SpString* numbers = values->data();

					textureCoordinates->add({
						numbers[1].to<sp_float>(),
						numbers[2].to<sp_float>()
					});

					sp_mem_delete(values, SpArrayOfString);
				}
			}
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