#pragma once
#include "AnimationClip.h"
#include "AnimTransition.h"

namespace dae
{	
	class Animator : public BaseComponent
	{
	public:
		// TODO: Animator throw exception when there's no animation clip
		void Update() override;
		void AddAnimationClip(std::shared_ptr<AnimationClip> clip);

	private:

		std::vector<std::shared_ptr<AnimationClip>> m_Clips;
		//const AnimationClip& m_CurrentClip;
		std::weak_ptr<AnimationClip> m_CurrentClip;
	};
	
}

