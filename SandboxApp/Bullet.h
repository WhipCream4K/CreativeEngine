#pragma once
#include <GameObject.h>

namespace dae
{
	class Animator;
	class AnimationClip;
	class Movement;
}

class Digger;
class Bullet : public dae::GameObject
{
public:
	void SetLaunchDirection(const glm::fvec2& direction);
protected:
	
	void Awake() override;
	void Update() override;
	void OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& otherColliders) override;

private:
	std::vector<std::shared_ptr<dae::Sprite>> m_pBulletBeforeImpactSprites;
	std::vector<std::shared_ptr<dae::Sprite>> m_pBulletAfterImpactSprites;

	std::weak_ptr<Digger> m_pRefMainScene;
	std::weak_ptr<dae::Movement> m_pRefMovementComponent;
	std::weak_ptr<dae::AnimationClip> m_pRefBulletAfterImpactClip;
	std::weak_ptr<dae::Animator> m_pRefAnimator;
	glm::fvec2 m_LaunchDirection;
	bool m_IsImpact{};
};

