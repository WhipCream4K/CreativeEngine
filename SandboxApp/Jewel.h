#pragma once
#include <GameObject.h>
class PlayerDigger;
class PickUps;
class Jewel : public dae::GameObject
{
protected:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& others) override;
private:
	std::weak_ptr<PlayerDigger> m_pRefPlayer;
	std::weak_ptr<PickUps> m_pRefPickUps;
	std::shared_ptr<dae::Sprite> m_pJewelSprite;
};