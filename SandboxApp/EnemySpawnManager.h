#pragma once
#include <GameObject.h>
class EnemySpawnManager : public dae::GameObject
{
public:
	
	EnemySpawnManager();
	void SetSpawnPoint(const glm::fvec3& point);
	void SetSpawnLimit(uint32_t amount) { m_SpawnLimit = amount; }
	void SetSpawnInterval(float interval) { m_SpawnInterval = interval; }
	
protected:
	
	void Awake() override;
	void Update() override;
	
private:
	glm::fvec3 m_SpawnPoint;
	float m_TimeCount;
	float m_SpawnInterval;
	uint32_t m_SpawnLimit;
};

