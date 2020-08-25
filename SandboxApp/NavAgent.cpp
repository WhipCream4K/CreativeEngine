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

// Big thanks to Sebastian Lague, a youtube who has a video about A star algorithm.
// It helps me made this https://www.youtube.com/watch?v=mZfyt03LDH4&t=1289s

WayPoints dae::NavAgent::RequestPath(const glm::fvec2& target)
{
	WayPoints wayPoints;
	wayPoints = std::async(std::launch::async, &NavAgent::CalculatePath, this, target);
	return wayPoints;
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
		m_pCellNodes[i] = std::make_shared<Node>(boundStart + offset, i % m_CellCol, i / m_CellCol, isCellUnWalkable[i]);

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
	if (m_HasInitCells && cellTypeArray)
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
	const glm::fvec4 white{ 1.0f,1.0f,1.0f,1.0f };
	const glm::fvec4 red{ 1.0f,0.0f,0.0f,1.0f };
	for (const auto& cell : m_pCellNodes)
	{
		Debug::DrawRect({ cell->position.x,cell->position.y,3.0f }, m_CellSize, cell->isWalkable ? white : red);
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
		m_pCellNodes[i] = std::make_shared<Node>(boundStart + offset, i % m_CellCol, i / m_CellCol, true);

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

std::shared_ptr<Node> dae::NavAgent::GetNodeFromWorldPos(const glm::fvec2& pos)
{
	float percentX{ (pos.x + m_MaxBound.x / 2.0f) / m_MaxBound.x };
	float percentY{ (m_MaxBound.y / 2.0f - pos.y) / m_MaxBound.y};
	percentX = Clamp(percentX, 0.0f, 1.0f);
	percentY = Clamp(percentY, 0.0f, 1.0f);

	int x{ int(float(m_CellCol) * percentX) };
	int y{ int(float(m_CellRow ) * percentY) };
	const uint32_t index{ uint32_t(y * int(m_CellCol) + x) };
	return m_pCellNodes[index];
}

std::vector<std::shared_ptr<Node>> dae::NavAgent::GetNeighbours(const std::shared_ptr<Node>& node)
{
	std::vector<std::shared_ptr<Node>> neighbours{};
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			if (x == 0 && y == 0)
				continue;

			int checkX = node->cellPosX + x;
			int checkY = node->cellPosY + y;

			if (checkX >= 0 && checkX < int(m_CellCol) && checkY >= 0 && checkY < int(m_CellRow))
			{
				const uint32_t index{ uint32_t(checkY * int(m_CellCol) + checkX) };
				neighbours.emplace_back(m_pCellNodes[index]);
			}
		}
	}

	return neighbours;
}

int dae::NavAgent::GetManhattanDistance(const std::shared_ptr<Node>& from, const std::shared_ptr<Node>& to)
{
	return abs(from->cellPosX - to->cellPosX) + abs(from->cellPosY - to->cellPosY);
}

std::forward_list<glm::fvec2> dae::NavAgent::CalculatePath(const glm::fvec2& target)
{
	// Initialize first node and end node
	const glm::fvec2 currPos{ GetGameObject()->GetTransform()->GetPosition() };
	const auto startNode{ GetNodeFromWorldPos(currPos) };
	const auto endNode{ GetNodeFromWorldPos(target) };

	std::vector<std::shared_ptr<Node>> openSet{};
	std::vector<std::shared_ptr<Node>> closeSet{};

	openSet.emplace_back(startNode);

	// Start A star algorithm
	while (!openSet.empty())
	{
		std::shared_ptr<Node> currentNode{ *std::min_element(openSet.begin(),openSet.end(),[](const std::shared_ptr<Node>& left,const std::shared_ptr<Node>& right)
		{
			const int leftFCost{left->fCost()};
			const int rightFCost{ right->fCost() };

			if (leftFCost == rightFCost)
				return left->hCost < right->hCost;

			return leftFCost < rightFCost;
		}) };

		openSet.erase(std::remove(openSet.begin(), openSet.end(), currentNode), openSet.end());

		closeSet.emplace_back(currentNode);

		if (currentNode == endNode)
		{
			// construct way points
			std::forward_list<glm::fvec2> path{};
			while (currentNode != startNode)
			{
				path.push_front(currentNode->position);
				currentNode = currentNode->pParent.lock();
			}

			//path.reverse();

			return path;
		}

		auto neighbours{ GetNeighbours(currentNode) };
		for (auto& neighbour : neighbours)
		{
			auto it = std::find(closeSet.begin(), closeSet.end(), neighbour);
			if (!neighbour->isWalkable || it != closeSet.end())
				continue;

			const int pathToNeighbour{ currentNode->gCost + GetManhattanDistance(currentNode,neighbour) };
			auto itContainsInOpenSet = std::find(openSet.begin(), openSet.end(), neighbour);
			if (pathToNeighbour < neighbour->gCost || itContainsInOpenSet == openSet.end())
			{
				neighbour->gCost = pathToNeighbour;
				neighbour->hCost = GetManhattanDistance(neighbour, endNode);
				neighbour->pParent = currentNode;
				openSet.emplace_back(neighbour);
			}
		}
	}

	return {};
}
