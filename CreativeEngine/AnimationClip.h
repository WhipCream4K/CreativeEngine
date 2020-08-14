#pragma once
#include "Sprite.h"
#include "CreativeStructs.h"
#include "SpriteRenderer.h"
#include "AnimTransition.h"
#include "FSMState.h"

namespace dae
{
	template<typename T>
	struct AnimFunc : IMuticastAction
	{
		AnimFunc(const std::function<void(std::weak_ptr<GameComponent<T>>)>& func, std::weak_ptr<GameComponent<T>> component)
			: m_Container{ func, component }
		{
		}

		void Invoke() override;
		MulticastContainer<void, std::weak_ptr<GameComponent<T>>> m_Container;
	};

	template <typename T>
	void AnimFunc<T>::Invoke()
	{
		std::apply(m_Container.func, m_Container.arguments);
	}

	class AnimTracker
	{
	public:

		AnimTracker(uint32_t keyFrameAmount)
			: m_AnimFunc{}
			, m_KeyFrameAmount{ keyFrameAmount }
			, m_CurrentKeyFrame{}
			, m_Interval{}
			, m_Timer{}
			, m_IsFinished()
		{
		}

		void Update(float deltaTime, uint32_t sampleRate);
		void SetExecuteFunction(ActionFunc func);

		ActionFunc m_AnimFunc;
		uint32_t m_KeyFrameAmount;
		uint32_t m_CurrentKeyFrame;
		float m_Interval;
		float m_Timer;
		bool m_IsFinished;
	};

	class AnimationClip
	{
	public:

		AnimationClip(std::weak_ptr<GameObject> pGameObject);

		static std::shared_ptr<AnimationClip> Create(std::weak_ptr<GameObject> pGameObject);

		// Only supports SpriteRenderer for now
		void AddProperty(std::weak_ptr<SpriteRenderer>&& component, const std::vector<std::shared_ptr<Sprite>>& sprites);

		// The amount of sample it takes in one second
		void SetSampleRate(uint32_t amount) { m_SampleRate = amount; }

		// Update all the object properties keyframe
		bool Update();

		void AddTransition(const AnimTransition& transition);
		//void AddTransition(const std::vector<std::string>& params, std::weak_ptr<AnimationClip>&& pPointToClip);
		//void AddBlankTransition(std::weak_ptr<AnimationClip>&& pPointToClip);

		std::vector<AnimTransition>& GetTransitions() { return m_Transitions; }

		bool IsFinishedPlaying();

	private:

		std::weak_ptr<GameObject> m_RefGameObject;
		std::vector<AnimTracker> m_ObjectProperties;
		std::vector<AnimTransition> m_Transitions;
		uint32_t m_SampleRate;
		
	};
}

