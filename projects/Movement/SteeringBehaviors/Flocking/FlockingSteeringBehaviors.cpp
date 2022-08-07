#include "stdafx.h"
#include "FlockingSteeringBehaviors.h"
#include "TheFlock.h"
#include "../SteeringAgent.h"
#include "../SteeringHelpers.h"

//*******************
//COHESION (FLOCKING)
SteeringOutput Cohesion::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	m_Target = m_pFlock->GetAverageNeighborPos();
	//seek m_Target
	SteeringOutput steering = {};

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();

	return steering;
}


//*********************
//SEPARATION (FLOCKING)
SteeringOutput Separation::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	// Move away from neighbors with a magnitude that’s inversely proportional to the distance to that neighbor.The closer a neighbor is, the more impact it should have on the separation.
	vector <SteeringAgent*> neigbours = m_pFlock->GetNeighbors();
	int NrOfNeighbors = m_pFlock->GetNrOfNeighbors();
	Elite::Vector2 fleePoint;
	for (int i = 0; NrOfNeighbors > i; i++)
	{
		Elite::Vector2 distance{ neigbours[i]->GetPosition() - pAgent->GetPosition() };
		float length = Elite::Distance(pAgent->GetPosition(), neigbours[i]->GetPosition());
		if (10 > length)
		{
			fleePoint += distance * (10 - length);

		}
	}
	fleePoint /= NrOfNeighbors;
	fleePoint *= -1;
	m_Target.Position = fleePoint;
	//flee the fleePoint 
	steering.LinearVelocity = m_Target.Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawCircle(pAgent->GetPosition(), 10.f, Elite::Color{ 1,1,1,1 }, 0.9f);
		std::vector<Elite::Vector2> pos;
		pos.push_back({ pAgent->GetPosition().x - 10, pAgent->GetPosition().y - 10 });
		pos.push_back({ pAgent->GetPosition().x - 10, pAgent->GetPosition().y + 10 });
		pos.push_back({ pAgent->GetPosition().x + 10, pAgent->GetPosition().y + 10 });
		pos.push_back({ pAgent->GetPosition().x + 10, pAgent->GetPosition().y - 10 });
		Elite::Polygon* rect = new Elite::Polygon(pos);
		DEBUGRENDERER2D->DrawPolygon(rect, Elite::Color({ 0,0,0,1 }), 0.9f);
		delete rect;
		rect = nullptr;

		for (int i = 0; NrOfNeighbors > i; i++)
		{
			neigbours[i]->SetBodyColor({ 0,1,0,1 });
		}

	}
	return steering;
}

//*************************
//VELOCITY MATCH (FLOCKING)

SteeringOutput VelocityMatch::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	steering.LinearVelocity = m_pFlock->GetAverageNeighborVelocity();
	return steering;
}

SteeringOutput EvadePrio::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	steering.LinearVelocity = pAgent->GetPosition() - m_Target.Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();


	steering.IsValid = false;
	float Distance = Elite::Distance(m_pFlock->GetAgentToEvade()->GetPosition(), pAgent->GetPosition());
	if (30 > Distance) { steering.IsValid = true; }
	return steering;
}