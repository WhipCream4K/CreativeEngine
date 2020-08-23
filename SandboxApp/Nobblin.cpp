#include "pch.h"
#include "Nobblin.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "AnimationClip.h"
#include "Animator.h"
#include "NavAgent.h"

void Nobblin::Awake()
{
	using namespace dae;
	auto nobblinWalking = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Nobblin.tga", "Nobblin");
	m_pWalkingSprites = Sprite::CreateSpriteSheet(nobblinWalking, {}, { 32.0f,30.f }, 3, 0.0f);
	auto deadSprite = Sprite::CreateSpriteSheet(nobblinWalking, { 32.0f * 3.0f,0.0f }, { 32.0f,30.0f }, 1, 0.0f);
	m_pDeadSprite = std::move(deadSprite[0]);

	auto spriteRenderer = CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(m_pWalkingSprites[0], true);

	auto walkingClip = AnimationClip::Create(GetShared<Nobblin>());
	walkingClip->AddProperty(spriteRenderer, m_pWalkingSprites);
	walkingClip->SetSampleRate(10);

	auto animator = CreateComponent<Animator>();
	animator->SetDefaultAnimClip(walkingClip);

	auto navAgent = CreateComponent<NavAgent>();
	navAgent->SetCellDimension(Digger::CellSize);
	navAgent->SetCellRowNColumn(10, 15);
	navAgent->SetMaxBound(Digger::PlayArea);
	
	navAgent->ResetCellsState(nullptr, 150);

}

void Nobblin::Update()
{
	
}
