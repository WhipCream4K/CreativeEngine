#pragma once
#include <GameObject.h>

namespace dae
{
	class Animator;
	class AnimationClip;
	class Movement;
	class SpriteRenderer;
}

class PlayerDigger;
class PickUps;
class Digger;
class Gold : public dae::GameObject
{
public:

	enum class GoldState
	{
		CheckingForDrop,
		IsCountingDown,
		IsDropping,
		IsShattering
	};
	
protected:

	void Awake() override;
	void Start() override;
	void Update() override;
	void OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& colliders) override;

private:

	std::vector<std::shared_ptr<dae::Sprite>> m_pMoneyDestroyedSprites;
	std::vector<std::shared_ptr<dae::Sprite>> m_pMoneyBagShakingSprites;
	std::vector<std::shared_ptr<dae::Sprite>> m_pMoneyBagDropSprite;
	std::shared_ptr<dae::Sprite> m_pMoneyBagIdleSprite;

	std::weak_ptr<PlayerDigger> m_pRefMainPlayer;
	std::weak_ptr<PickUps> m_pRefPickUps;
	std::weak_ptr<Digger> m_pRefDiggerScene;
	std::weak_ptr<dae::Animator> m_pRefAnimator;
	std::weak_ptr<dae::AnimationClip> m_pRefMoneyBagShakingAnimClip;
	std::weak_ptr<dae::AnimationClip> m_pRefMoneyBagShatteredAnimClip;
	std::weak_ptr<dae::AnimationClip> m_pRefMoneyBagDropAnimClip;
	std::weak_ptr<dae::Movement> m_pRefMovement;
	std::weak_ptr<dae::SpriteRenderer> m_pRefSpriteRenderer;

	glm::fvec2 m_ActualSize;
	float m_TimeCount{};
	float m_TimeBeforeDrop{2.0f};
	GoldState m_State{ GoldState::CheckingForDrop };
	bool m_IsPickable{};
	bool m_ShouldSurvive{};
	bool m_IsPlayerBelowMe{};
	bool m_ReadyForDrop{};
	bool m_IsDropping{};

	void CountDownDropping();
	void PlayDropping();
	void CheckForBelowCellAndPlayer();
	void PlayShattering();
	void ClampToPlayArea();
	void ClampToTheGround();
};

