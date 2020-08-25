#pragma once
#include <GameObject.h>
#include "EventDispatcher.h"

namespace dae
{
	class SpriteRenderer;
	class Animator;
	class Movement;
	class AnimationClip;
}

class Digger;
class PlayerDigger : public dae::GameObject
{
public:

	enum class PlayerDirection
	{
		Right,
		Left,
		Down,
		Up
	};
	
	PlayerDigger();
	static dae::EventDispatcher<int> OnDeath;
	void SetPlayerStart(const glm::fvec3& position);
	void SetPlayerSpeed(const glm::fvec2& speed) { m_MovementSpeed = speed; }
	const glm::fvec2& GetActualSize() const { return m_PlayerActualSize; }
	glm::fvec2 GetPointForward();
	glm::fvec2 GetForward();
	void CallDeath();

protected:

	void Awake() override;
	void Update() override;
	void LateUpdate() override;
	void OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& otherColliders) override;
	
private:

	
	// Normal Digger
	std::vector<std::shared_ptr<dae::Sprite>> m_pWalkStraight;
	std::vector<std::shared_ptr<dae::Sprite>> m_pWalkUp;
	std::vector<std::shared_ptr<dae::Sprite>> m_pWalkDown;

	// Empty Shell
	std::vector<std::shared_ptr<dae::Sprite>> m_pWalkStraightX;
	std::vector<std::shared_ptr<dae::Sprite>> m_pWalkUpX;
	std::vector<std::shared_ptr<dae::Sprite>> m_PWalkDownX;

	// Dead Sprite
	std::vector<std::shared_ptr<dae::Sprite>> m_pDeadSprite;

	std::weak_ptr<dae::SpriteRenderer> m_pSpriteRenderer;
	std::weak_ptr<dae::Animator> m_pAnimator;
	std::weak_ptr<dae::Movement> m_pMovement;

	std::weak_ptr<Digger> m_pRefDiggerScene;
	std::weak_ptr<dae::Transform> m_pRefTransform;
	std::weak_ptr<dae::AnimationClip> m_pRefDeadAnimClip;
	std::weak_ptr<dae::AnimationClip> m_pRefShellEmptyIdleAnimClip;
	std::weak_ptr<dae::AnimationClip> m_pRefMainIdleAnimClip;
	
	glm::fvec2 m_MovementSpeed; // movement speed in pixels
	glm::fvec2 m_PlayerActualSize;
	const float m_MoveTimeLimitX;
	const float m_MoveTimeLimitY;
	PlayerDirection m_PlayerFacing;
	float m_TimeCountX;
	float m_TimeCountY;
	float m_HorizontalScale;
	float m_VerticalScale;
	float m_ReloadTime;
	float m_ReloadTimeCount;
	bool m_PlayerFacingRight;
	bool m_PlayerFacingUp;
	bool m_ShellEmpty;
	bool m_HasMovedHorizontal;
	bool m_HasMovedVertical;
	bool m_IsDead;

	void MoveHorizontal(float value);
	void MoveVertical(float value);
	void Shoot();
	void PlayDeath(int something);
	bool IsTouchingRimPlayArea();
	void ClampIfGold();
};

