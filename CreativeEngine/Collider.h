#pragma once

namespace dae
{
	class Collider;
	class Collider
	{
	public:

		Collider();
		constexpr auto IsEnableDebugDraw() const noexcept -> bool { return m_IsEnableDebugDraw; }
		constexpr auto IsTriggered() const noexcept -> bool { return m_IsTriggered; }

	protected:
		bool m_IsATrigger;
		bool m_IsTriggered;
	private:
		bool m_IsEnableDebugDraw;
	};
}


