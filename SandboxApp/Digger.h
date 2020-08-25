#pragma once
#include <Scene.h>
#include <unordered_map>

namespace dae
{
	class Text;
}

class PlayerDigger;
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
	static const glm::fvec2 ObjectRelativeScaling;
	static constexpr uint32_t MaxCellRow{ 10 };
	static constexpr uint32_t MaxCellCol{ 15 };
	static constexpr uint32_t CellCount{ 150 };
	static uint32_t PlayerLives;
	bool IsInBetweenCellsX(const glm::fvec3& position);
	bool IsInBetweenCellsY(const glm::fvec3& position);
	// Call to update the path for the player
	void UpdatePlayerMovement(const glm::fvec2& pos);
	const char* GetCurrentCellSemantics() const { return m_pCellSemantics; }
	BlockId GetBlockIdFromWorldPos(const glm::fvec3& position);
	uint32_t GetBlockIndexFromWorldPos(const glm::fvec3& position);
	glm::fvec3 GetWorldPosFromIndex(uint32_t index);
	static constexpr bool IsOutSidePlayArea(const glm::fvec3& pos);
	void SetBlockSemantic(uint32_t index, BlockId semantic);
	
protected:

	void SceneInitialize() override;
	void SetUpInputMappingGroup() override;
	void Update() override;
	
private:
	
	std::unordered_map<uint32_t, std::shared_ptr<dae::Sprite>> m_pBigSprites;
	std::unordered_map<PathDirection, std::shared_ptr<dae::Sprite>> m_pWalkSprites;
	std::shared_ptr<dae::Text> m_pDiggerFont;
	std::shared_ptr<dae::Sprite> m_pTitle;
	std::shared_ptr<dae::Sprite> m_pBackgroundSprite;

	std::weak_ptr<PlayerDigger> m_pRefMainPlayer;
	const glm::fvec3* m_pRefMainPlayerPos;
	
	char* m_pCellSemantics;
	bool m_HasTheGameStart;
	
	void SpawnPath(const glm::fvec3& position, const glm::fvec2& scale);
	void InstantiatePath(const glm::fvec3& position, const glm::fvec2& scale);

	void AddPathEdges();
};

constexpr bool Digger::IsOutSidePlayArea(const glm::fvec3& pos)
{
	float remapX{ pos.x + PlayArea.x / 2.0f };
	float remapY{ PlayArea.y / 2.0f - pos.y };
	return remapX > PlayArea.x || remapY > PlayArea.y || remapX < 0.0f || remapY < 0.0f;
}

