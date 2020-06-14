#pragma once
#include "CreativeTypeName.h"

namespace dae
{
	class FSMTransition;
	class FSMState
	{
	public:
		
		FSMState();
		~FSMState() = default;
		void RunActions() const;
		void RunEntryActions() const;
		void RunExitActions() const;

		const std::vector<std::shared_ptr<FSMTransition>>& GetTransition() const { return m_Transitions; }
		
		void SetEntryActions(std::vector<ActionFunc>&& entryActions) { m_EntryActions = std::move(entryActions); }
		void SetActions(std::vector<ActionFunc>&& actions) { m_Actions = std::move(actions); }
		void SetExitActions(std::vector<ActionFunc>&& exitActions) { m_ExitActions = std::move(exitActions); }
		void SetTransitions(std::vector<std::shared_ptr<FSMTransition>>&& transitions) { m_Transitions = std::move(transitions); }

		virtual void Update() {}
		
	protected:

		std::vector<ActionFunc> m_EntryActions;
		std::vector<ActionFunc> m_Actions;
		std::vector<ActionFunc> m_ExitActions;
		std::vector<std::shared_ptr<FSMTransition>> m_Transitions;
	};
}

