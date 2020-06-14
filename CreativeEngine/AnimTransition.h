#pragma once
#include "FSMTransition.h"

namespace dae
{
	class AnimationClip;
	class AnimTransition
	{
	public:
		AnimTransition(std::vector<ConditionFunc>&& conditions, std::weak_ptr<AnimationClip>&& targetState);

		bool IsTriggered() const;
		std::shared_ptr<AnimationClip> GetTargetState() const { return m_TargetClip.lock(); }

	private:
		
		std::vector<ConditionFunc> m_Conditions;
		std::weak_ptr<AnimationClip> m_TargetClip;
	};
}

