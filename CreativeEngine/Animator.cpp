#include "pch.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "AnimTransition.h"

void dae::Animator::Update()
{
	const auto& transitions = m_CurrentClip.lock()->GetTransitions();
	bool transitionTriggered = false;
	const AnimTransition* triggeredTransition{};

	for (const auto& transition : transitions)
	{
		transitionTriggered = transition.IsTriggered();
		if (transitionTriggered)
		{
			triggeredTransition = &transition;
			break;
		}
	}

	if (transitionTriggered)
		m_CurrentClip = triggeredTransition->GetTargetState();

	m_CurrentClip.lock()->Update();
	//if (transitionTriggerd)
	//{
	//	const auto& targetState = triggeredTransition->GetTargetState();
	//	targetState->RunEntryActions();

	//	// switch
	//	m_CurrentState = targetState;
	//}
	//else
	//	currentState->RunActions();
}

void dae::Animator::AddAnimationClip(std::shared_ptr<AnimationClip> clip)
{
	if (m_Clips.empty())
		m_CurrentClip = clip;
	m_Clips.emplace_back(clip);
}
