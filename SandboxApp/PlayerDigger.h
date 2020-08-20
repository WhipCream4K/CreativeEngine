#pragma once
#include <GameObject.h>

namespace dae
{
	class SpriteRenderer;
	class Animator;
	class EventDispatcher;
	class DispatcherHandler;
	class Movement;
}

class PlayerDigger : public dae::GameObject
{
public:

	PlayerDigger();
	dae::DispatcherHandler* GetPlayerDeathDispatcher() const { return m_pDispatcherHandler; }

protected:

	void Awake() override;
	
private:

	dae::DispatcherHandler* m_pDispatcherHandler;
	
	
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
	bool m_IsShellEmpty;

	void MoveHorizontal(float value);
	void MoveVertical(float value);
};

