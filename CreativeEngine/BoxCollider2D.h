#pragma once

#include "BaseComponent.h"
#include "Collider.h"

namespace dae
{
	class BoxCollider2D : public BaseComponent, public Collider
	{
	public:
		
		constexpr auto SetIsTrigger(bool value) noexcept -> void { m_IsTriggered = value; }
		constexpr auto SetCenter(const glm::fvec2& center) noexcept -> void { m_Center = center; }
		constexpr auto SetSize(const glm::fvec2& size) noexcept -> void { m_Size = size; }

	protected:
		
		void Update() override;
		void Render() const override;

	private:
		glm::fvec2 m_Center;
		glm::fvec2 m_Size;
	};
}

