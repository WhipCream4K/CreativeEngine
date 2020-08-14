#pragma once
#include "AnimationClip.h"
#include "AnimTransition.h"

namespace dae
{
	class Animator : public BaseComponent
	{
	public:

		struct AnimParams
		{
			std::string name{};
			bool isATrigger{};
			bool condition{};
		};

		void AddTransition(const std::shared_ptr<AnimationClip>& left,const std::string& name, const std::shared_ptr<AnimationClip>& right,bool isTrigger);
		void AddBlankTransition(const std::shared_ptr<AnimationClip>& left, const std::shared_ptr<AnimationClip>& right);
		void AddAnimationClip(std::shared_ptr<AnimationClip> clip);
		void SetDefaultAnimClip(std::weak_ptr<AnimationClip>&& clip) { m_Default = std::move(clip); }
		void SetTrigger(const std::string& paramName);
		void SetBool(const std::string& paramName, bool state);

	protected:
		void Update() override;
		void Awake() override;
	private:

		std::vector<std::shared_ptr<AnimationClip>> m_Clips;
		std::unordered_map<std::string, AnimParams> m_pAnimParams;
		std::weak_ptr<AnimationClip> m_CurrentClip;
		std::weak_ptr<AnimationClip> m_Default;
	};
	
}

