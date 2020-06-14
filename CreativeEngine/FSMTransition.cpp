#include "pch.h"
#include "FSMTransition.h"

dae::FSMTransition::FSMTransition()
	: m_Actions{}
	, m_Conditions{}
	, m_TargetState{}
{
}

dae::FSMTransition::FSMTransition(std::vector<ConditionFunc>&& conditions, std::vector<ActionFunc>&& actions,
	std::weak_ptr<FSMState> targetState)
	: m_Actions{ std::move(actions) }
	, m_Conditions{ std::move(conditions) }
	, m_TargetState{ std::move(targetState) }
{
}

bool dae::FSMTransition::IsTriggerd() const
{
	bool isTrue = false;

	if (!m_Conditions.empty())
	{
		for (const auto& condition : m_Conditions)
		{
			isTrue = condition->Invoke();
		}
	}
	else
		isTrue = true;

	return isTrue;
}

void dae::FSMTransition::RunActions() const
{
	for (const auto& action : m_Actions)
	{
		action->Invoke();
	}
}
