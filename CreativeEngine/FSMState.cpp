#include "pch.h"
#include "FSMState.h"

dae::FSMState::FSMState()
	: m_EntryActions{}
	, m_Actions{}
	, m_ExitActions{}
	, m_Transitions{}
{
}

void dae::FSMState::RunActions() const
{
	for (const auto& action : m_Actions)
	{
		action->Invoke();
	}
}

void dae::FSMState::RunEntryActions() const
{
	for (const auto& action : m_EntryActions)
	{
		action->Invoke();
	}
}

void dae::FSMState::RunExitActions() const
{
	for (const auto& action : m_ExitActions)
	{
		action->Invoke();
	}
}
