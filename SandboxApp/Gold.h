#pragma once
#include <GameObject.h>

class PlayerDigger;
class Gold : public dae::GameObject
{
protected:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& colliders) override;
private:
	std::weak_ptr<PlayerDigger> m_pRefMainPlayer;
	std::vector<std::shared_ptr<dae::Sprite>> m_pMoneySprites;
	std::vector<std::shared_ptr<dae::Sprite>> m_pMoneyDestroyedSprites;
};

