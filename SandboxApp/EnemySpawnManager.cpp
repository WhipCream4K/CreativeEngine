#include "pch.h"
#include "EnemySpawnManager.h"


#include "Digger.h"
#include "Nobblin.h"

EnemySpawnManager::EnemySpawnManager()
	: m_SpawnPoint()
	, m_TimeCount()
	, m_SpawnInterval()
	, m_SpawnLimit()
	, m_CurrentSpawnCount()
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
	if(m_CurrentSpawnCount > m_SpawnLimit)
		return;
	
	auto scene{ GetScene() };
	const float deltaSeconds{ scene->GetSceneContext().pGameTime->GetDeltaSeconds() };
	m_TimeCount += deltaSeconds;
	const float firstEnemySpawnTime{ 1.0f };

	if (m_CurrentSpawnCount == 0)
	{
		if (m_TimeCount >= firstEnemySpawnTime)
		{
			m_TimeCount -= firstEnemySpawnTime;
			scene->Instantiate<Nobblin>(m_SpawnPoint,{},Digger::ObjectRelativeScaling);
			m_CurrentSpawnCount++;
		}
	}

	if (m_TimeCount >= m_SpawnInterval)
	{
		m_TimeCount -= m_SpawnInterval;
		scene->Instantiate<Nobblin>(m_SpawnPoint,{},Digger::ObjectRelativeScaling);
	}
}
