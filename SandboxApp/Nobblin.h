#pragma once
#include <GameObject.h>
class Nobblin : public dae::GameObject
{
protected:
	void Awake() override;
	void Update() override;
private:
	std::vector<std::shared_ptr<dae::Sprite>> m_pWalkingSprites;
	std::shared_ptr<dae::Sprite> m_pDeadSprite;
};

