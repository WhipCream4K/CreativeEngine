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
		auto SetSprite(std::weak_ptr<Sprite>&& sprite) noexcept -> void;
		//void SetSprite(Sprite&& sprite) { m_Sprite = std::move(sprite); }
		//void SetSprite(const Sprite& sprite) { m_Sprite = sprite; }
		auto SetPivotPoint(const glm::fvec2& point) noexcept -> void;
		auto SetFlipX(bool value) noexcept -> void;
		auto SetFlipY(bool value) noexcept -> void;

	protected:
		void Render() const override;
	private:
		// I decided to keep sprite on the stack because for now Sprite doesn't have an owner
		// and if I were to store it in the heap then I need to create some sort of container
		// which doesn't make sense to me (it's a pod anyway)
		std::weak_ptr<Sprite> m_pSprite;
		//std::weak_ptr<Sprite> m_pSprite;
	};
}


