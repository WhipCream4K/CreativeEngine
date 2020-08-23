#include "pch.h"
#include "NavAgent.h"

dae::NavAgent::NavAgent()
	: m_pCellNodes()
	, m_MaxBound()
	, m_CellSize()
	, m_CellRow()
	, m_CellCol()
{
}

WayPoints dae::NavAgent::RequestPath(const glm::fvec2& target)
{
	// Initialize first node and end node
	const glm::fvec2 currPos{ GetGameObject()->GetTransform()->GetPosition() };
	target;
	WayPoints something{};
	return something;
}

void dae::NavAgent::CreateCustomCells(const bool* isCellUnWalkable, uint32_t col, uint32_t row)
{
	const glm::fvec2 offset{ m_CellSize.x / 2.0f, -m_CellSize.y / 2.0f };
	glm::fvec2 boundStart{ -m_MaxBound.x / 2.0f, m_MaxBound.y / 2.0f };
	int countCol{};

	m_CellCol = col;
	m_CellRow = row;
	
	for (uint32_t i = 0; i < col * row; ++i)
	{
		m_pCellNodes[i] = std::make_shared<Node>(boundStart + offset, isCellUnWalkable[i]);

		if (countCol % (m_CellCol - 1) == 0 && countCol != 0)
		{
			countCol = 0;
			boundStart.x = -m_MaxBound.x / 2.0f;
			boundStart.y -= m_CellSize.y;
		}
		else
		{
			boundStart.x += m_CellSize.x;
			++countCol;
		}
	}

	m_HasInitCells = true;
}

void dae::NavAgent::ResetCellsState(const bool* cellTypeArray, size_t count)
{
	if(m_HasInitCells && cellTypeArray)
	{
		for (size_t i = 0; i < count; ++i)
		{
			m_pCellNodes[i]->isWalkable = cellTypeArray[i];
		}
	}
}

void dae::NavAgent::SetCellRowNColumn(uint32_t row, uint32_t col)
{
	m_CellRow = row;
	m_CellCol = col;
}

void dae::NavAgent::Awake()
{
	// Initialize Blocks of node
	CreateCells();
}

void dae::NavAgent::Render() const
{
	for (const auto& cell : m_pCellNodes)
	{
		Debug::DrawRect({ cell->position.x,cell->position.y,3.0f }, m_CellSize,{1.0f,1.0f,1.0f,1.0f});
	}
}

void dae::NavAgent::CreateCells()
{
	if (m_HasInitCells)
		return;
	
	const glm::fvec2 offset{ m_CellSize.x / 2.0f, -m_CellSize.y / 2.0f };
	glm::fvec2 boundStart{ -m_MaxBound.x / 2.0f, m_MaxBound.y / 2.0f };
	int countCol{};

	for (uint32_t i = 0; i < m_CellCol * m_CellRow; ++i)
	{
		m_pCellNodes[i] = std::make_shared<Node>(boundStart + offset, true);

		if (countCol % (m_CellCol - 1) == 0 && countCol != 0)
		{
			countCol = 0;
			boundStart.x = -m_MaxBound.x / 2.0f;
			boundStart.y -= m_CellSize.y;
		}
		else
		{
			boundStart.x += m_CellSize.x;
			++countCol;
		}
	}

	m_HasInitCells = true;
}
