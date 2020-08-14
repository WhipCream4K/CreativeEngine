#include "pch.h"
#include "AnimationClip.h"

#include <utility>
#include "Scene.h"

void dae::AnimTracker::Update(float deltaTime, uint32_t sampleRate)
{
	m_Timer += deltaTime;
	m_Interval += deltaTime;

	const float interval{ 1.0f / sampleRate };
	const float maxTime{ interval * m_KeyFrameAmount };

	if (m_Timer >= maxTime)
	{
		m_Timer = 0.0f;
		m_Interval = 0.0f;
		m_CurrentKeyFrame = 0;

		m_IsFinished = true;
		//m_IsFinished = true;
		//
		//// Call function
		//m_AnimFunc->Invoke();
	}

	if (m_Interval >= interval)
	{
		m_IsFinished = false;
		m_Interval = 0.0f;
		m_CurrentKeyFrame = Clamp(++m_CurrentKeyFrame, 0U, m_KeyFrameAmount - 1);

		// Call function
		m_AnimFunc->Invoke();
	}
}

void dae::AnimTracker::SetExecuteFunction(ActionFunc func)
{
	m_AnimFunc = std::move(func);
}

dae::AnimationClip::AnimationClip(std::weak_ptr<GameObject> pGameObject)
	: m_RefGameObject{ std::move(pGameObject) }
	, m_SampleRate{ 60 }
{
}

std::shared_ptr<dae::AnimationClip> dae::AnimationClip::Create(std::weak_ptr<GameObject> pGameObject)
{
	return std::make_shared<AnimationClip>(std::move(pGameObject));
}

void dae::AnimationClip::AddProperty(std::weak_ptr<SpriteRenderer>&& spriteRenderer,
	const std::vector<std::shared_ptr<dae::Sprite>>& sprites)
{
	AnimTracker tracker{ uint32_t(sprites.size()) };
	m_ObjectProperties.emplace_back(tracker);
	auto& temp = m_ObjectProperties.back();
	const auto changeSprite = [&sprites, &temp](std::weak_ptr<SpriteRenderer> renderer)
	{
		renderer.lock()->SetSprite(sprites[temp.m_CurrentKeyFrame], false);
	};

	temp.SetExecuteFunction(std::make_shared<AnimFunc<SpriteRenderer>>(changeSprite, spriteRenderer));
}

bool dae::AnimationClip::Update()
{
	const float deltaTime{ m_RefGameObject.lock()->GetScene()->GetSceneContext().pGameTime->GetDeltaSeconds() };

	bool isFinished{ true };
	
	// Update Scene time
	for (auto& tracker : m_ObjectProperties)
	{
		tracker.Update(deltaTime, m_SampleRate);
		isFinished = tracker.m_IsFinished;
	}

	return isFinished;
}

void dae::AnimationClip::AddTransition(const AnimTransition& transition)
{
	m_Transitions.emplace_back(transition);
}

bool dae::AnimationClip::IsFinishedPlaying()
{
	bool isFinished{ true };

	for (const auto& property : m_ObjectProperties)
	{
		isFinished = property.m_IsFinished;
	}

	return isFinished;
}

