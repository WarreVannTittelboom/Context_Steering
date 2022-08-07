#include "stdafx.h"
#include "SpacePartitioning.h"
#include "projects\Movement\SteeringBehaviors\SteeringAgent.h"

// --- Cell ---
// ------------
Cell::Cell(float left, float bottom, float width, float height)
{
	boundingBox.bottomLeft = { left, bottom };
	boundingBox.width = width;
	boundingBox.height = height;
	agentCount = 0;
}

std::vector<Elite::Vector2> Cell::GetRectPoints() const
{
	auto left = boundingBox.bottomLeft.x;
	auto bottom = boundingBox.bottomLeft.y;
	auto width = boundingBox.width;
	auto height = boundingBox.height;

	const std::vector<Elite::Vector2> rectPoints =
	{
		{ left , bottom  },
		{ left , bottom + height  },
		{ left + width , bottom + height },
		{ left + width , bottom  },
	};

	return rectPoints;
}

// --- Partitioned Space ---
// -------------------------
CellSpace::CellSpace(float width, float height, int rows, int cols, int maxEntities)
	: m_SpaceWidth(width)
	, m_SpaceHeight(height)
	, m_NrOfRows(rows)
	, m_NrOfCols(cols)
	, m_Neighbors(maxEntities)
	, m_NrOfNeighbors(0)
	, m_CellWidth(width / rows)
	, m_CellHeight(height / cols)
{
	int count{};
	for (int i = 0; m_NrOfRows > i; ++i)
	{
		for (int k = 0; m_NrOfCols > k; ++k)
		{
			m_Cells.push_back(Cell(k * m_CellWidth, i * m_CellHeight, m_CellWidth, m_CellHeight));
		}
	}
}

void CellSpace::AddAgent(SteeringAgent* agent)
{
	int index = CellSpace::PositionToIndex(agent->GetPosition());
	m_Cells[index].agents.push_back(agent);
	m_Cells[index].agentCount += 1;
	// add agent to cell with this index
}



void CellSpace::UpdateAgentCell(SteeringAgent* agent, Elite::Vector2& oldPos)
{
	int index = CellSpace::PositionToIndex(agent->GetPosition());
	int oldindex = CellSpace::PositionToIndex(oldPos);
	if (oldindex != index)
	{
		std::list<SteeringAgent*>::iterator findIter = std::find(m_Cells[oldindex].agents.begin(), m_Cells[oldindex].agents.end(), agent);
		m_Cells[oldindex].agents.erase(findIter);

		m_Cells[oldindex].agentCount -= 1;
		m_Cells[index].agents.push_back(agent);
		m_Cells[index].agentCount += 1;
		oldPos = agent->GetPosition();
	}
}

void CellSpace::RegisterNeighbors(SteeringAgent* agent, float queryRadius)
{
	m_NrOfNeighbors = 0;
	m_Neighbors.clear();
	int index = CellSpace::PositionToIndex(agent->GetPosition());
	for (int i = 0; m_Cells.size() > i; ++i)
	{
		if (i == index || i == index - 1 || i == index + 1 || i == index + m_NrOfCols || i == index + m_NrOfCols - 1 || i == index + m_NrOfCols + 1 || i == index - m_NrOfCols || i == index - m_NrOfCols - 1 || i == index - m_NrOfCols + 1)
		{




			for (SteeringAgent* n : m_Cells[i].agents)
			{
				float distance = Elite::Distance(n->GetPosition(), agent->GetPosition());
				if (queryRadius >= distance)
				{
					m_Neighbors.push_back(n);
					++m_NrOfNeighbors;
				}
			}

		}
	}

}

void CellSpace::RenderCells() const
{

	for (int i = 0; m_Cells.size() > i; ++i)
	{
		Elite::Polygon* rect = new Elite::Polygon(m_Cells[i].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect, Elite::Color({ 1,0,0,1 }), 0.9f);
		delete rect;
		rect = nullptr;
		int count = m_Cells[i].agentCount;
		std::string text = std::to_string(count);
		const char* c = text.c_str();
		std::vector<Elite::Vector2> pos = m_Cells[i].GetRectPoints();
		pos[0].y += m_CellHeight;
		DEBUGRENDERER2D->DrawString(pos[0], c);
	}

}

int CellSpace::PositionToIndex(const Elite::Vector2 pos) const
{
	int index{};

	for (int i = 0; pos.y > m_CellHeight + (m_CellHeight * i); ++i)
	{
		index += m_NrOfCols;
	}
	for (int k = 0; pos.x > m_CellWidth + (m_CellWidth * k); ++k)
	{
		++index;
	}


	if (index >= 99) { index = 99; }
	return index;
}
void CellSpace::RenderNeighborhood(SteeringAgent* agent)
{
	int index = CellSpace::PositionToIndex(agent->GetPosition());
	if (index <= 99 && index >= 0)
	{
		Elite::Polygon* rect1 = new Elite::Polygon(m_Cells[index].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect1, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect1;
		rect1 = nullptr;
	}
	if (index - 1 <= 99 && index - 1 >= 0) {
		Elite::Polygon* rect2 = new Elite::Polygon(m_Cells[index - 1].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect2, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect2;
		rect2 = nullptr;
	}
	if (index + 1 <= 99 && index + 1 >= 0) {
		Elite::Polygon* rect3 = new Elite::Polygon(m_Cells[index + 1].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect3, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect3;
		rect3 = nullptr;
	}
	if (index - m_NrOfCols <= 99 && index - m_NrOfCols >= 0) {
		Elite::Polygon* rect4 = new Elite::Polygon(m_Cells[index - m_NrOfCols].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect4, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect4;
		rect4 = nullptr;
	}
	if (index - m_NrOfCols - 1 <= 99 && index - m_NrOfCols - 1 >= 0) {
		Elite::Polygon* rect5 = new Elite::Polygon(m_Cells[index - m_NrOfCols - 1].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect5, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect5;
		rect5 = nullptr;
	}
	if (index - m_NrOfCols + 1 <= 99 && index - m_NrOfCols + 1 >= 0) {
		Elite::Polygon* rect6 = new Elite::Polygon(m_Cells[index - m_NrOfCols + 1].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect6, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect6;
		rect6 = nullptr;
	}
	if (index + m_NrOfCols <= 99 && index + m_NrOfCols >= 0) {
		Elite::Polygon* rect7 = new Elite::Polygon(m_Cells[index + m_NrOfCols].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect7, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect7;
		rect7 = nullptr;
	}
	if (index + m_NrOfCols - 1 <= 99 && index + m_NrOfCols - 1 >= 0) {
		Elite::Polygon* rect8 = new Elite::Polygon(m_Cells[index + m_NrOfCols - 1].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect8, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect8;
		rect8 = nullptr;

	}
	if (index + m_NrOfCols + 1 <= 99 && index + m_NrOfCols + 1 >= 0) {
		Elite::Polygon* rect9 = new Elite::Polygon(m_Cells[index + m_NrOfCols + 1].GetRectPoints());
		DEBUGRENDERER2D->DrawPolygon(rect9, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect9;
		rect9 = nullptr;
	}
}