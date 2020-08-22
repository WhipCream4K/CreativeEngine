#pragma once
#include <BaseComponent.h>

#include "EventDispatcher.h"


namespace dae
{
	class BoxCollider2D;
}

class PickUps : public dae::BaseComponent
{
public:

	PickUps();
	void SetBoundSize(const glm::fvec2& size) const;
	void SetGainScore(int value) { m_GainScore = value; }

	constexpr int GetGainScore() const noexcept { return m_GainScore; }
	static dae::EventDispatcher<int> OnScoreUpdate;

protected:
	void Awake() override;
	void Update() override;
private:
	std::weak_ptr<dae::BoxCollider2D> m_pCollider;
	int m_GainScore;
};

