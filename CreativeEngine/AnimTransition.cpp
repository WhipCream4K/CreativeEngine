#include "pch.h"
#include "AnimTransition.h"
#include "AnimationClip.h"

//dae::AnimTransition::AnimTransition(std::vector<ConditionFunc>&& conditions, std::vector<ActionFunc>&& actions,
//	std::weak_ptr<AnimationClip> targetState)
//		: FSMTransition(std::move(conditions),std::move(actions),std::move(targetState))
//{
//}
//
//dae::AnimTransition::AnimTransition()
//	: FSMTransition()
//{
//}

dae::AnimTransition::AnimTransition(std::weak_ptr<AnimationClip>&& targetState)
	: m_TargetClip(std::move(targetState))
{
}

dae::AnimTransition::AnimTransition(std::vector<ConditionFunc>&& conditions, std::weak_ptr<AnimationClip>&& targetState)
	: m_Conditions{ std::move(conditions) }
	, m_TargetClip{ std::move(targetState) }
{
}

bool dae::AnimTransition::IsTriggered() const
{
	return std::all_of(m_States.begin(), m_States.end(), [](const auto& state) { return state->second; });
}


//bool dae::AnimTransition::IsTriggered() const
//{
//	bool isTrue = false;
//
//	if (!m_Conditions.empty())
//	{
//		for (const auto& condition : m_Conditions)
//		{
//			isTrue = condition->Invoke();
//		}
//	}
//	else
//		isTrue = true;
//
//	return isTrue;
//}
