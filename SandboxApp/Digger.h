#pragma once
#include <Scene.h>
#include <unordered_map>


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
	
protected:

	void SceneInitialize() override;
	void SetUpInputMappingGroup() override;
	void Update() override;
	
private:
	
	std::unordered_map<uint32_t, std::shared_ptr<dae::Sprite>> m_pBigSprites;
	std::unordered_map<PathDirection, std::shared_ptr<dae::Sprite>> m_pWalkSprites;
	std::shared_ptr<dae::Sprite> m_pBackgroundSprite;
	std::shared_ptr<dae::Sprite> m_pCherrySprite;

	static const uint32_t m_CellCount;

	char* m_pCellSemantics;
};

