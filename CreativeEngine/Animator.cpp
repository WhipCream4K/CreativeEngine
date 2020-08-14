#include "pch.h"
#include "Animator.h"
#include "AnimationClip.h"
#include "AnimTransition.h"

void dae::Animator::Awake()
{
	auto& defaultVal = m_pAnimParams[""];
	defaultVal.isATrigger = false;
	defaultVal.condition = true;
	defaultVal.name = "";
	
	for (const auto& clip : m_Clips)
	{
		clip;
	}
}

void dae::Animator::Update()
{
	if (!m_Clips.empty())
	{
		if (!m_CurrentClip.lock())
			m_CurrentClip = m_Default;

		const auto sharedTemp = m_CurrentClip.lock();

		if (sharedTemp)
		{
			auto& transitions = sharedTemp->GetTransitions();
			bool transitionTriggered = false;
			AnimTransition* triggeredTransition{};

			for (auto& transition : transitions)
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

			if (m_CurrentClip.lock()->Update())
			{
				// reset
				auto& conditionValues = triggeredTransition->GetTransitionConditions();
				for (const auto& value : conditionValues)
				{
					std::string str{ value.first };
					auto& param = m_pAnimParams.at(str);
					if (param.isATrigger)
						param.condition = false;
				}
				m_CurrentClip = m_Default;
			}
		}
	}
}

void dae::Animator::AddTransition(const std::shared_ptr<AnimationClip>& left, const std::string& name,
	const std::shared_ptr<AnimationClip>& right, bool isTrigger)
{
	bool isContainsA{}, isContainsB{};
	for (const auto& clip : m_Clips)
	{
		isContainsA = (clip == left);
		isContainsB = (clip == right);
	}

	if (!isContainsA)
		m_Clips.emplace_back(left);
	if (!isContainsB)
		m_Clips.emplace_back(right);

	auto it = m_pAnimParams.find(name);
	if (it != m_pAnimParams.end())
	{
		// Create Only Transition for left and right
		AnimTransition transition{ right };
		auto& value = m_pAnimParams.at(name);
		transition.GetTransitionConditions().push_back(std::make_pair(value.name, &value.condition));
		left->AddTransition(transition);

	}
	else
	{
		// Create new params and transition for left and right
		AnimParams params{ name,isTrigger,false };
		m_pAnimParams.try_emplace(name, params);

		AnimTransition transition{ right };
		auto& value = m_pAnimParams.at(name);
		transition.GetTransitionConditions().push_back(std::make_pair(value.name, &value.condition));
		left->AddTransition(transition);
	}
}

void dae::Animator::AddBlankTransition(const std::shared_ptr<AnimationClip>& left,
	const std::shared_ptr<AnimationClip>& right)
{
	bool isContainsA{}, isContainsB{};
	for (const auto& clip : m_Clips)
	{
		isContainsA = (clip == left);
		isContainsB = (clip == right);
	}

	if (!isContainsA)
		m_Clips.emplace_back(left);
	if (!isContainsB)
		m_Clips.emplace_back(right);

	AnimTransition transition{ right };
	auto& value = m_pAnimParams.at("");
	transition.GetTransitionConditions().push_back(std::make_pair(value.name, &value.condition));
	left->AddTransition(transition);
}

void dae::Animator::AddAnimationClip(std::shared_ptr<AnimationClip> clip)
{
	if (m_Clips.empty())
		m_Default = clip;
	m_Clips.emplace_back(std::move(clip));
}

void dae::Animator::SetTrigger(const std::string& paramName)
{
	paramName;
}

void dae::Animator::SetBool(const std::string& paramName, bool state)
{
	paramName;
	state;
}
