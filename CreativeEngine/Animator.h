#pragma once
#include "AnimationClip.h"
#include "AnimTransition.h"

namespace dae
{
	class Animator final : public BaseComponent
	{
	public:

		struct AnimParams
		{
			std::string name{};
			bool isATrigger{};
			bool condition{};
		};

		void AddTransition(const std::shared_ptr<AnimationClip>& from, const std::string& name, const std::shared_ptr<AnimationClip>& to, bool isTrigger);
		void AddBlankTransition(const std::shared_ptr<AnimationClip>& from, const std::shared_ptr<AnimationClip>& to);
		void AddAnimationClip(std::shared_ptr<AnimationClip> clip);
		void SetDefaultAnimClip(std::weak_ptr<AnimationClip>&& clip);
		void SetTrigger(const std::string& paramName);
		void SetBool(const std::string& paramName, bool state);
		void SetActive(bool value) { m_IsActive = value; }

	protected:
		void Update() override;
		void Awake() override;
	private:

		std::vector<std::shared_ptr<AnimationClip>> m_Clips;
		std::unordered_map<std::string, AnimParams> m_pAnimParams;
		std::weak_ptr<AnimationClip> m_CurrentClip;
		std::weak_ptr<AnimationClip> m_Default;
		bool m_IsActive{ true };
	};
	
}

