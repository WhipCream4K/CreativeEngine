#pragma once
#include <GameObject.h>

class PlayerDigger;
class PickUps;
class Gold : public dae::GameObject
{
public:
protected:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& colliders) override;
private:
	std::weak_ptr<PlayerDigger> m_pRefMainPlayer;
	std::weak_ptr<PickUps> m_pRefPickUps;
	std::vector<std::shared_ptr<dae::Sprite>> m_pMoneySprites;
	std::vector<std::shared_ptr<dae::Sprite>> m_pMoneyDestroyedSprites;
	bool m_IsPickable{};
};

