#pragma once
#include <GameObject.h>
#include "EventDispatcher.h"

namespace dae
{
	class SpriteRenderer;
	class Animator;
	class Movement;
}

class Digger;
class PlayerDigger : public dae::GameObject
{
public:
	
	PlayerDigger();
	static dae::EventDispatcher<void> OnDeath;
	void SetPlayerStart(const glm::fvec3& position);
	void SetPlayerSpeed(const glm::fvec2& speed) { m_MovementSpeed = speed; }

protected:

	void Awake() override;
	void Update() override;
	void LateUpdate() override;
	
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
	
	glm::fvec2 m_MovementSpeed; // movement speed in pixels
	glm::fvec2 m_PlayerActualSize;
	const float m_MoveTimeLimitX;
	const float m_MoveTimeLimitY;
	bool m_PlayerFacingRight;
	bool m_PlayerFacingUp;
	float m_TimeCountX;
	float m_TimeCountY;
	float m_HorizontalScale;
	float m_VerticalScale;
	bool m_IsShellEmpty;
	bool m_HasMovedHorizontal;
	bool m_HasMovedVertical;

	void MoveHorizontal(float value);
	void MoveVertical(float value);
	void Shoot();
	bool IsTouchingRimPlayArea();
};

