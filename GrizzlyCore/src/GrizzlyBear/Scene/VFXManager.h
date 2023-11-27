#pragma once

#include <stdint.h>


namespace GrizzlyBear {

	struct VFXHandle
	{
		VFXHandle(uint64_t id)
			: m_ID(id) {}

		~VFXHandle() {}

		VFXHandle(const VFXHandle& entityHandle)
		{
			this->m_ID = entityHandle.m_ID;
		}

		VFXHandle& operator=(const VFXHandle& rhs)
		{
			this->m_ID = rhs.m_ID;

			return *this;
		}

		bool operator==(const VFXHandle& rhs) const
		{
			if (this->m_ID == rhs.m_ID)
			{
				return true;
			}

			return false;
		}

		uint32_t get_id() const { return m_ID; }

		static VFXHandle none()
		{
			return VFXHandle(0);
		}

	private:
		uint64_t m_ID;
	};

	class VFXManager
	{

	};


}
