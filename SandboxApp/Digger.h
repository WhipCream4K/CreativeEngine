#pragma once
#include <Scene.h>
#include <unordered_map>

namespace dae
{
	class Text;
}

class Digger : public dae::Scene
{
public:

	Digger();
	
	enum class BlockId
	{
		None,
		Level,
		Gold,
		Jewel,
	};

	enum class PathDirection
	{
		None,
		Right,
		Left,
		Up,
		Down
	};

	~Digger();

	static const glm::fvec2 PlayArea;
	static const glm::fvec2 CellSize;
	static const uint32_t CellCount;
	
protected:

	void SceneInitialize() override;
	void SetUpInputMappingGroup() override;
	void Update() override;
	
private:
	
	std::unordered_map<uint32_t, std::shared_ptr<dae::Sprite>> m_pBigSprites;
	std::unordered_map<PathDirection, std::shared_ptr<dae::Sprite>> m_pWalkSprites;
	std::shared_ptr<dae::Text> m_pDiggerFont;
	std::shared_ptr<dae::Sprite> m_pBackgroundSprite;


	char* m_pCellSemantics;
};

