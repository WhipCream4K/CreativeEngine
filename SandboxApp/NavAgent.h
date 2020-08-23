#pragma once
#include <BaseComponent.h>
#include "Digger.h"
#include <array>

using WayPoints = std::future<std::vector<glm::fvec2>>;

constexpr uint32_t MAX_CELL_COUNT{ 150 };

struct Node
{
	Node(const glm::fvec2& pos, bool walkable)
		: position(pos)
		, pParent()
		, isWalkable(walkable)
	{
	}

	glm::fvec2 position;
	std::weak_ptr<Node> pParent;
	bool isWalkable;
};

namespace dae
{
	class NavAgent : public dae::BaseComponent
	{
	public:

		NavAgent();
		WayPoints RequestPath(const glm::fvec2& target);
		void CreateCustomCells(const bool* isCellUnWalkable, uint32_t col,uint32_t row);
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
		//std::shared_ptr<Node> m_pCellNodes[];
	};
}


