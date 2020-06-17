#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Sprite;
	class SpriteRenderer final : public BaseComponent
	{
	public:

		//void SetSprite(const Sprite& sprite) { m_Sprite = sprite; }
		////void SetSprite(std::weak_ptr<Sprite> sprite);
		//const Sprite& GetSprite() const { return m_Sprite; }
		//std::shared_ptr<Sprite> GetSprite() const { return m_pSprite.lock(); }

		// Getter
		auto GetSprite() const noexcept -> std::shared_ptr<Sprite> { return m_pSprite.lock(); }
		// Setter
		auto SetSprite(std::weak_ptr<Sprite>&& sprite,bool reset) noexcept -> void;
		//void SetSprite(Sprite&& sprite) { m_Sprite = std::move(sprite); }
		//void SetSprite(const Sprite& sprite) { m_Sprite = sprite; }
		auto SetPivotPoint(const glm::fvec2& point) noexcept -> void;
		auto SetFlipX(bool value) noexcept -> void;
		auto SetFlipY(bool value) noexcept -> void;

	protected:
		void Render() const override;
	private:

		std::weak_ptr<Sprite> m_pSprite;
		glm::fvec2 m_PivotPoint;
		bool m_FlipY;
		bool m_FlipX;
	};
}


