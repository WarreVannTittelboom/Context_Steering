#pragma once
#include "../SteeringHelpers.h"
#include "FlockingSteeringBehaviors.h"

class ISteeringBehavior;
class SteeringAgent;
class BlendedSteering;
class PrioritySteering;
class CellSpace;

//#define USE_SPACE_PARTITIONING

class Flock
{

public:
	Flock(
		int flockSize = 50,
		float worldSize = 100.f,
		SteeringAgent* pAgentToEvade = nullptr,
		bool trimWorld = false);

	~Flock();

	void Update(float deltaT);
	void UpdateAndRenderUI();
	void Render(float deltaT);

	//#ifndef  USE_SPACE_PARTITIONING
	void RegisterNeighbors(SteeringAgent* pAgent);
	//#endif
	int GetNrOfNeighbors() const { return m_NrOfNeighbors; }
	const vector<SteeringAgent*>& GetNeighbors() const { return m_Neighbors; }


	Elite::Vector2 GetAverageNeighborPos() const;
	Elite::Vector2 GetAverageNeighborVelocity() const;
	SteeringAgent* GetAgentToEvade() { return m_pAgentToEvade; }
	void SetSeekTarget(TargetData target);
	void SetWorldTrimSize(float size) { m_WorldSize = size; }

private:
	//Datamembers
	int m_FlockSize = 100;
	Wandering* wander = new Wandering();
	vector<SteeringAgent*> m_Agents;
	//#ifdef USE_SPACE_PARTITIONING
	CellSpace* m_pCellSpace = nullptr;
	vector<Elite::Vector2*> m_OldPos;
	//#endif


	bool m_SpacePart = true;
	bool m_TrimWorld = false;
	float m_WorldSize = 0.f;
	float m_NeighborhoodRadius = 10.f;
	int m_NrOfNeighbors = 0;
	vector<SteeringAgent*> m_Neighbors;
	SteeringAgent* m_pAgentToEvade = nullptr;

	//Steering Behaviors

	Separation* m_pSeparationBehavior = nullptr;
	Cohesion* m_pCohesionBehavior = nullptr;
	VelocityMatch* m_pVelMatchBehavior = nullptr;
	Seek* m_pSeekBehavior = nullptr;
	Wandering* m_pWanderBehavior = nullptr;
	EvadePrio* m_pEvadeBehavior = nullptr;
	BlendedSteering* m_pBlendedSteering = nullptr;
	PrioritySteering* m_pPrioritySteering = nullptr;



	float* GetWeight(ISteeringBehavior* pBehaviour);

private:
	Flock(const Flock& other);
	Flock& operator=(const Flock& other);
};