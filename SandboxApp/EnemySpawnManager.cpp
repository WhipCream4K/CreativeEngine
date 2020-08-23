#include "pch.h"
#include "EnemySpawnManager.h"

#include "Nobblin.h"

EnemySpawnManager::EnemySpawnManager()
	: m_SpawnPoint()
	, m_TimeCount()
	, m_SpawnInterval()
	, m_SpawnLimit()
{
}

void EnemySpawnManager::SetSpawnPoint(const glm::fvec3& point)
{
	m_SpawnPoint = point;
}

void EnemySpawnManager::Awake()
{

}

void EnemySpawnManager::Update()
{
	auto scene{ GetScene() };
	const float deltaSeconds{ scene->GetSceneContext().pGameTime->GetDeltaSeconds() };
	m_TimeCount += deltaSeconds;
	if(m_TimeCount >= m_SpawnInterval)
	{
		m_TimeCount -= m_SpawnInterval;
		scene->Instantiate<Nobblin>(m_SpawnPoint);
	}
}
