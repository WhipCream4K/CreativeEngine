#pragma once
#include "FSMTransition.h"

namespace dae
{
	class AnimationClip;
	class AnimTransition
	{
	public:

		AnimTransition(std::weak_ptr<AnimationClip>&& targetState);
		AnimTransition(std::vector<ConditionFunc>&& conditions, std::weak_ptr<AnimationClip>&& targetState);

		bool IsTriggered() const;

		std::vector<std::pair<std::string_view,bool*>>& GetTransitionConditions() { return m_States; }
		std::shared_ptr<AnimationClip> GetTargetState() const { return m_TargetClip.lock(); }

	private:

		std::vector<std::pair<std::string_view,bool*>> m_States;
		std::vector<ConditionFunc> m_Conditions;
		std::weak_ptr<AnimationClip> m_TargetClip;
		
	};
}

