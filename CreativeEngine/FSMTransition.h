#pragma once
#include "CreativeTypeName.h"

namespace dae
{
	class FSMState;
	class FSMTransition
	{
	public:

		FSMTransition();
		FSMTransition(std::vector<ConditionFunc>&& conditions, std::vector<ActionFunc>&& actions,std::weak_ptr<FSMState> targetState);
		bool IsTriggerd() const;
		std::shared_ptr<FSMState> GetTargetState() const { return m_TargetState.lock(); }
		void RunActions() const;
		
	protected:
		
		std::vector<ActionFunc> m_Actions;
		std::vector<ConditionFunc> m_Conditions;
		std::weak_ptr<FSMState> m_TargetState;
	};
}

