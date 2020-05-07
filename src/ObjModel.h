#ifndef OBJ_MODEL_HEADER
#define OBJ_MODEL_HEADER

#include "SpectrumRendering.h"
#include "Model.h"
#include "FileSystem.h"
#include "SpText.h"
#include "SpArrayOfString.h"

#define SP_OBJECT_PREFIX  "o "
#define SP_VERTEX_PREFIX  "v "
#define SP_NORMAL_PREFIX  "vn"
#define SP_TEXTURE_PREFIX "vt"
#define SP_FACE_PREFIX    "f "

namespace NAMESPACE_RENDERING
{
	/// <summary>
	/// Represents a Wavefront file (.obj)
	/// </summary>
	class ObjModel
		: public Model
	{
	private:

		sp_uint countFaces(const SpText& text)
		{
			sp_uint result = ZERO_UINT;

			for (sp_uint i = ZERO_UINT; i < text.length(); i++)
				if (text[i]->startWith(SP_FACE_PREFIX))
				{
					sp_uint count = text[i]->count(' ');
					result += divideBy2(count);
				}

			return result;
		}

	public:

		/// <summary>
		/// Load a Wavefront file (.obj) from disk to memory
		/// </summary>
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
			sp_uint texturesCoordLength = text.countLinesStartWith(SP_TEXTURE_PREFIX);
			sp_uint facesLength = countFaces(text);

			vertexes = sp_mem_new(SpArray<Vec3f>)(vertexesLength);
			textureCoordinates = sp_mem_new(SpArray<Vec2f>)(texturesCoordLength);
			faces = sp_mem_new(SpArray<Vec3ui>)(facesLength);

			for (sp_uint i = ZERO_UINT; i < text.length(); i++)
			{
				if (text[i]->startWith(SP_OBJECT_PREFIX))
				{
					const SpArrayOfString* values = text[i]->split(' ');
					sp_char* content = values->data()[1].data();

					name = sp_mem_new(SpString)(content);

					sp_mem_delete(values, SpArrayOfString);
				}
				else if (text[i]->startWith(SP_VERTEX_PREFIX))
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
				else if (text[i]->startWith(SP_TEXTURE_PREFIX))
				{
					SpArrayOfString* values = text[i]->split(' ');
					SpString* numbers = values->data();

					textureCoordinates->add({
						numbers[1].to<sp_float>(),
						numbers[2].to<sp_float>()
					});

					sp_mem_delete(values, SpArrayOfString);
				}
				else if (text[i]->startWith(SP_FACE_PREFIX))
				{
					SpArrayOfString* values = text[i]->split(' ');
					SpArrayOfString* pair1 = values->data()[1].split('/');
					SpArrayOfString* pair2 = values->data()[2].split('/');
					SpArrayOfString* pair3 = values->data()[3].split('/');

					sp_uint index1 = pair1->data()[0].to<sp_uint>() - ONE_UINT;
					sp_uint index2 = pair2->data()[0].to<sp_uint>() - ONE_UINT;
					sp_uint index3 = pair3->data()[0].to<sp_uint>() - ONE_UINT;

					faces->add({ index1, index2, index3 });

					if (values->length() == 5u) // if face with 4 vertexes...
					{
						SpArrayOfString* pair4 = values->data()[4].split('/');
						sp_uint index4 = pair4->data()[0].to<sp_uint>() - ONE_UINT;

						faces->add({ index3, index4, index1 });

						sp_mem_delete(pair4, SpArrayOfString);
					}

					sp_mem_delete(pair3, SpArrayOfString);
					sp_mem_delete(pair2, SpArrayOfString);
					sp_mem_delete(pair1, SpArrayOfString);
					sp_mem_delete(values, SpArrayOfString);
				}
			}

			generateNormals();
		}

		API_INTERFACE virtual const char* toString() override
		{
			return "OBJ Model";
		}

	};
}

#undef SP_OBJECT_PREFIX
#undef SP_VERTEX_PREFIX
#undef SP_NORMAL_PREFIX
#undef SP_TEXTURE_PREFIX
#undef SP_FACE_PREFIX

#endif // OBJ_MODEL_HEADER