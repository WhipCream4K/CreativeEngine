#pragma once

#include "Collider.h"

namespace dae
{
	class BoxCollider2D : public Collider
	{
	public:
		
		constexpr auto SetIsTrigger(bool value) noexcept -> void { m_IsTriggered = value; }
		constexpr auto SetCenter(const glm::fvec2& center) noexcept -> void { m_Center = center; }
		constexpr auto SetSize(const glm::fvec2& size) noexcept -> void { m_Size = size; }
		constexpr auto GetSize() const noexcept -> const glm::fvec2& { return m_Size; }
		glm::fvec2 GetCenter() const;

		void Resolve(const std::vector<std::weak_ptr<Collider>>& colliders) override;
		void Resolve(const std::shared_ptr<Collider>& collider) override;
		
	protected:
		
		void Render() const override;

	private:
		
		glm::fvec2 m_Center;
		glm::fvec2 m_Size;
	};
}

