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
				if(transitionTriggered)
				{
					auto& conditionValues = triggeredTransition->GetTransitionConditions();
					for (const auto& value : conditionValues)
					{
						std::string str{ value.first };
						auto& param = m_pAnimParams.at(str);
						if (param.isATrigger)
							param.condition = false;
					}
				}
				m_CurrentClip = m_Default;
			}
		}
	}
}

void dae::Animator::AddTransition(const std::shared_ptr<AnimationClip>& from, const std::string& name,
	const std::shared_ptr<AnimationClip>& to, bool isTrigger)
{
	bool isContainsFrom{}, isContainsTo{};
	for (const auto& clip : m_Clips)
	{
		if(isContainsFrom && isContainsTo)
			break;
		
		if (!isContainsFrom)
			isContainsFrom = (clip == from);
		if (!isContainsTo)
			isContainsTo = (clip == to);
	}

	if (!isContainsFrom)
		m_Clips.emplace_back(from);
	if (!isContainsTo)
		m_Clips.emplace_back(to);

	// Create new params and transition for from and to
	AnimParams params{ name,isTrigger,false };
	m_pAnimParams.try_emplace(name, params);

	AnimTransition transition{ to };
	auto& value = m_pAnimParams.at(name);
	transition.GetTransitionConditions().emplace_back(value.name, &value.condition);
	from->AddTransition(transition);
}

void dae::Animator::AddBlankTransition(const std::shared_ptr<AnimationClip>& from,
	const std::shared_ptr<AnimationClip>& to)
{
	bool isContainsFrom{}, isContainsTo{};
	for (const auto& clip : m_Clips)
	{
		if (isContainsFrom && isContainsTo)
			break;

		if (!isContainsFrom)
			isContainsFrom = (clip == from);
		if (!isContainsTo)
			isContainsTo = (clip == to);
	}

	if (!isContainsFrom)
		m_Clips.emplace_back(from);
	if (!isContainsTo)
		m_Clips.emplace_back(to);

	AnimTransition transition{ to };
	auto& value = m_pAnimParams.at("");
	transition.GetTransitionConditions().emplace_back(value.name, &value.condition);
	from->AddTransition(transition);
}

void dae::Animator::AddAnimationClip(std::shared_ptr<AnimationClip> clip)
{
	if (m_Clips.empty())
		m_Default = clip;
	m_Clips.emplace_back(std::move(clip));
}

void dae::Animator::SetDefaultAnimClip(std::weak_ptr<AnimationClip>&& clip)
{
	if(!clip.expired())
	{
		for (const auto& currClip : m_Clips)
		{
			if (clip.lock() == currClip)
			{
				m_Default = currClip;
				return;
			}
		}

		m_Clips.emplace_back(clip.lock());
		m_Default = std::move(clip);
	}

}

void dae::Animator::SetTrigger(const std::string& paramName)
{
	auto& value = m_pAnimParams.at(paramName);
	if (value.isATrigger)
		value.condition = true;
}

void dae::Animator::SetBool(const std::string& paramName, bool state)
{
	auto& value = m_pAnimParams.at(paramName);
	if (!value.isATrigger)
		value.condition = state;
}
