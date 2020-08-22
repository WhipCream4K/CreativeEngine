#pragma once
#include "GameObject.h"

namespace dae
{
	class TextRenderer;
	class Text;
}
class ScoreManager : public dae::GameObject
{
public:
	ScoreManager();
	void UpdateScore(int score);
protected:
	void Awake() override;
	void Start() override;
private:
	std::weak_ptr<dae::TextRenderer> m_pTextRenderer;
	std::shared_ptr<dae::Text> m_pText;
	static constexpr int m_InitScoreShown{ 5 };
	int m_TotalScore;
};

