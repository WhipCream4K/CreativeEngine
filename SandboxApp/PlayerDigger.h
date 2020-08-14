#pragma once
#include <GameObject.h>

class PlayerDigger :
    public dae::GameObject
{
public:
	PlayerDigger();
protected:
	void Awake() override;
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
	
	bool m_IsShellEmpty;
};

