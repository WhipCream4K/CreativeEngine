#pragma once
#include <GameObject.h>
class Jewel : public dae::GameObject
{
protected:
	void Awake() override;
	void Update() override;
private:
	std::shared_ptr<dae::Sprite> m_pJewelSprite;
};

