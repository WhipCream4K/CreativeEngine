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
		PlayerStart,
		EnemySpawn
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
	static constexpr uint32_t CellCount{ 150 };
	bool IsInBetweenCellsX(const glm::fvec3& position);
	bool IsInBetweenCellsY(const glm::fvec3& position);
	
protected:

	void SceneInitialize() override;
	void SetUpInputMappingGroup() override;
	void Update() override;
	
private:
	
	std::unordered_map<uint32_t, std::shared_ptr<dae::Sprite>> m_pBigSprites;
	std::unordered_map<PathDirection, std::shared_ptr<dae::Sprite>> m_pWalkSprites;
	std::shared_ptr<dae::Text> m_pDiggerFont;
	std::shared_ptr<dae::Sprite> m_pBackgroundSprite;

	void SpawnPath(const glm::fvec3& position,const glm::fvec2& scale);
	char* m_pCellSemantics;
};

