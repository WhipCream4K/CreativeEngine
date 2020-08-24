#pragma once
#include <BaseComponent.h>
#include "Digger.h"
#include <array>
#include <forward_list>

using WayPoints = std::future<std::forward_list<glm::fvec2>>;

constexpr uint32_t MAX_CELL_COUNT{ 150 };

struct Node
{
	Node(const glm::fvec2& pos, int posX, int posY, bool walkable)
		: position(pos)
		, pParent()
		, cellPosX(posX)
		, cellPosY(posY)
		, isWalkable(walkable)
	{
	}

	int fCost() const { return gCost + hCost; }

	glm::fvec2 position;
	std::weak_ptr<Node> pParent;
	int cellPosX{};
	int cellPosY{};
	int gCost{}; // distance from end node
	int hCost{}; // distance from start node
	bool isWalkable{};
};

namespace dae
{
	class NavAgent : public dae::BaseComponent
	{
	public:

		NavAgent();
		WayPoints RequestPath(const glm::fvec2& target);
		void CreateCustomCells(const bool* isCellUnWalkable, uint32_t col, uint32_t row);
		void SetMaxBound(const glm::fvec2& size) { m_MaxBound = size; }
		void SetCellDimension(const glm::fvec2& size) { m_CellSize = size; }
		void ResetCellsState(const bool* cellTypeArray, size_t count);
		void SetCellRowNColumn(uint32_t row, uint32_t col);

	protected:
		void Awake() override;
		void Render() const override;
	private:

		std::array<std::shared_ptr<Node>, MAX_CELL_COUNT> m_pCellNodes;
		glm::fvec2 m_MaxBound;
		glm::fvec2 m_CellSize;
		uint32_t m_CellRow;
		uint32_t m_CellCol;
		bool m_HasInitCells;
		void CreateCells();
		std::shared_ptr<Node> GetNodeFromWorldPos(const glm::fvec2& pos);
		std::vector<std::shared_ptr<Node>> GetNeighbours(const std::shared_ptr<Node>& node);
		int GetManhattanDistance(const std::shared_ptr<Node>& from, const std::shared_ptr<Node>& to);
		std::forward_list<glm::fvec2> CalculatePath(const glm::fvec2& target);
		//std::shared_ptr<Node> m_pCellNodes[];
	};
}


