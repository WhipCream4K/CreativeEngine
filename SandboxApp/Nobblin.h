#pragma once
#include <GameObject.h>

namespace dae
{
	class NavAgent;
}
class PlayerDigger;
class Nobblin : public dae::GameObject
{
protected:
	void Awake() override;
	void Start() override;
	void Update() override;
private:
	std::vector<std::shared_ptr<dae::Sprite>> m_pWalkingSprites;
	std::shared_ptr<dae::Sprite> m_pDeadSprite;

	std::weak_ptr<dae::NavAgent> m_pRefNavAgent;
	std::weak_ptr<PlayerDigger> m_pRefMainPlayer;
};

