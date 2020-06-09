#ifndef SP_GRAPHIC_OBJECT_MANAGER_HEADER
#define SP_GRAPHIC_OBJECT_MANAGER_HEADER

#include "SpectrumRendering.h"
#include "SpPhysicSimulator.h"
#include "SpPhysicSyncronizer.h"
#include "SpArray.h"
#include "SpTransform.h"

namespace NAMESPACE_RENDERING
{
	class SpGraphicObjectManager :
		public SpPhysicSyncronizer
	{
	private:
		sp_uint _allocatedObjects;
		sp_uint _maxLength;
		SpArray<SpTransform>* _transforms;

		SpGraphicObjectManager()
		{
		}

	public:
		
		API_INTERFACE void init(const sp_uint length)
		{
			_allocatedObjects = ZERO_UINT;
			_maxLength = length;
			_transforms = sp_mem_new(SpArray<SpTransform>)(_maxLength, _maxLength);

			SpPhysicSimulator::instance()->syncronizer = this;
		}

		API_INTERFACE static SpGraphicObjectManager* instance()
		{
			static SpGraphicObjectManager* _instance = sp_mem_new(SpGraphicObjectManager)();
			return _instance;
		}

		API_INTERFACE inline sp_uint maxObjectsLength()
		{
			return _maxLength;
		}

		API_INTERFACE inline SpTransform* transforms(const sp_uint index) const
		{
			return &_transforms->data()[index];
		}

		API_INTERFACE inline sp_uint alloc(const sp_uint length)
		{
			sp_assert(length + _allocatedObjects < _maxLength, "InvalidArgumentException");

			sp_uint index = _allocatedObjects;

			_allocatedObjects += length;

			return index;
		}

		API_INTERFACE void sync(const sp_uint objectIndex, const Vec3& translation) override
		{
			_transforms->data()[objectIndex].translate(translation);
		}

	};
}

#endif SP_GRAPHIC_OBJECT_MANAGER_HEADER