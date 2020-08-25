#pragma once
#include <GameObject.h>

#include "NavAgent.h"

namespace dae
{
	class NavAgent;
}
class PlayerDigger;
class Nobblin : public dae::GameObject
{
public:
	Nobblin();
protected:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& otherColliders) override;
private:
	std::vector<std::shared_ptr<dae::Sprite>> m_pWalkingSprites;
	std::shared_ptr<dae::Sprite> m_pDeadSprite;

	std::weak_ptr<dae::NavAgent> m_pRefNavAgent;
	std::weak_ptr<PlayerDigger> m_pRefMainPlayer;
	std::weak_ptr<Digger> m_pRefDiggerScene;

	std::array<bool, MAX_CELL_COUNT> GetNewCellUpdate();
	WayPoints m_FutureWayPoints;
	std::vector<glm::fvec2> m_WayPoints;
	float m_RequestUpdateInderval;
	float m_UpdateTimeCount;
	float m_TimeCountX;
	float m_TimeCountY;
	float m_MoveTimeLimitX;
	float m_MoveTimeLimitY;
	bool m_JustAskForPath;

	void MoveToPath();
	glm::fvec2 GetCurrentpath(const std::vector<glm::fvec2>& path);
};

