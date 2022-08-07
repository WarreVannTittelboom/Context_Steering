#include "stdafx.h"
#include "TheFlock.h"

#include "../SteeringAgent.h"
#include "../Steering/SteeringBehaviors.h"
#include "../CombinedSteering/CombinedSteeringBehaviors.h"
#include "../SpacePartitioning/SpacePartitioning.h"

//Constructor & Destructor
Flock::Flock(
	int flockSize,
	float worldSize,
	SteeringAgent* pAgentToEvade,
	bool trimWorld)

	: m_WorldSize{ worldSize }
	, m_FlockSize{ flockSize }
	, m_TrimWorld{ trimWorld }
	, m_pAgentToEvade{ pAgentToEvade }
	, m_NeighborhoodRadius{ 10 }
	, m_NrOfNeighbors{ 0 }
{
	m_Agents.resize(m_FlockSize);
	//#ifdef USE_SPACE_PARTITIONING
	m_OldPos.resize(m_FlockSize);
	// #endif 
		// TODO: initialize the flock and the memory pool
	for (int i = 0; m_FlockSize > i; ++i)
	{
		m_Agents[i] = new SteeringAgent();
		m_Agents[i]->SetAutoOrient(true);
		int x = rand() % int(m_WorldSize);
		int y = rand() % int(m_WorldSize);
		m_Agents[i]->SetPosition({ float(x),float(y) });
	}
	m_pAgentToEvade = new SteeringAgent();
	m_pSeparationBehavior = new Separation(this);
	m_pCohesionBehavior = new Cohesion(this);
	m_pVelMatchBehavior = new VelocityMatch(this);
	m_pSeekBehavior = new Seek();
	m_pWanderBehavior = new Wandering();
	m_pEvadeBehavior = new EvadePrio(this);
	m_pBlendedSteering = new BlendedSteering({ {m_pSeparationBehavior,0.2f}, {m_pCohesionBehavior,0.2f}, {m_pVelMatchBehavior,0.2f},  {m_pWanderBehavior,0.2f} , {m_pSeekBehavior,0.2f} });
	m_pPrioritySteering = new PrioritySteering({ m_pEvadeBehavior,m_pBlendedSteering });
	//#ifdef USE_SPACE_PARTITIONING
	m_pCellSpace = new CellSpace(m_WorldSize, m_WorldSize, 10, 10, m_FlockSize);

	for (int k = 0; m_FlockSize > k; ++k)
	{
		m_OldPos[k] = new Elite::Vector2(m_Agents[k]->GetPosition());
		m_pCellSpace->CellSpace::AddAgent(m_Agents[k]);
	}
	//#endif 
}

Flock::~Flock()
{
	// TODO: clean up any additional data

	SAFE_DELETE(m_pSeparationBehavior);
	SAFE_DELETE(m_pCohesionBehavior);
	SAFE_DELETE(m_pVelMatchBehavior);
	SAFE_DELETE(m_pSeekBehavior);
	SAFE_DELETE(m_pWanderBehavior);
	SAFE_DELETE(m_pEvadeBehavior);
	SAFE_DELETE(m_pBlendedSteering);
	SAFE_DELETE(m_pPrioritySteering);
	SAFE_DELETE(m_pAgentToEvade);

	for (auto pAgent : m_Agents)
	{
		SAFE_DELETE(pAgent);
	}
	m_Agents.clear();
	//#ifdef USE_SPACE_PARTITIONING
	SAFE_DELETE(m_pCellSpace);

	for (auto pOldPos : m_OldPos)
	{
		SAFE_DELETE(pOldPos);
	}
	m_OldPos.clear();
	//#endif

	delete wander;
	wander = nullptr;
}

void Flock::Update(float deltaT)
{
	m_pAgentToEvade->SetAutoOrient(true);
	m_pEvadeBehavior->SetTarget(m_pAgentToEvade->GetPosition());
	m_pAgentToEvade->SetSteeringBehavior(m_pWanderBehavior);
	m_pAgentToEvade->Update(deltaT);
	m_pAgentToEvade->TrimToWorld(Elite::Vector2(0, 0), Elite::Vector2(m_WorldSize, m_WorldSize));
	m_pAgentToEvade->Update(deltaT);
	for (int i = 0; m_FlockSize > i; ++i)
	{
		//if using spatial part
//#ifdef USE_SPACE_PARTITIONING
		if (m_SpacePart)
		{
			m_pCellSpace->UpdateAgentCell(m_Agents[i], *(m_OldPos[i]));
			m_pCellSpace->RegisterNeighbors(m_Agents[i], 10.f);
			m_NrOfNeighbors = m_pCellSpace->GetNrOfNeighbors();
			m_Neighbors = m_pCellSpace->GetNeighbors();
		}
		//#else
				// register its neighbors	(-> memory pool is filled with neighbors of the currently evaluated agent)
		else
		{
			RegisterNeighbors(m_Agents[i]);
		}
		//#endif
		m_Agents[i]->SetSteeringBehavior(m_pPrioritySteering);
		// update i(-> the behaviors can use the neighbors stored in the pool, next iteration they will be the next agent's neighbors)
		m_Agents[i]->Update(deltaT);
		// trim it to the world
		m_Agents[i]->TrimToWorld(Elite::Vector2(0, 0), Elite::Vector2(m_WorldSize, m_WorldSize));
		m_NrOfNeighbors = 0;
		m_Neighbors.clear();
	}
}

void Flock::Render(float deltaT)
{
	//render agents arrow and color

//	for (int i = 0; m_FlockSize > i; ++i)
//	{
//		m_Agents[i]->Render(deltaT);
//		m_Agents[i]->SetBodyColor({ 1,1,0,1 });
//		
//	}

	m_pAgentToEvade->Render(deltaT);
	m_pAgentToEvade->SetBodyColor({ 1,0,0,1 });


	//#ifdef USE_SPACE_PARTITIONING
	if (m_SpacePart)
	{
		m_pCellSpace->RenderNeighborhood(m_Agents[0]);
		m_pCellSpace->RenderCells();
	}
	//#endif
}

void Flock::UpdateAndRenderUI()
{
	//Setup
	int menuWidth = 235;
	int const width = DEBUGRENDERER2D->GetActiveCamera()->GetWidth();
	int const height = DEBUGRENDERER2D->GetActiveCamera()->GetHeight();
	bool windowActive = true;
	ImGui::SetNextWindowPos(ImVec2((float)width - menuWidth - 10, 10));
	ImGui::SetNextWindowSize(ImVec2((float)menuWidth, (float)height - 20));
	ImGui::Begin("Gameplay Programming", &windowActive, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::PushAllowKeyboardFocus(false);

	//Elements
	ImGui::Text("CONTROLS");
	ImGui::Indent();
	ImGui::Text("LMB: place target");
	ImGui::Text("RMB: move cam.");
	ImGui::Text("Scrollwheel: zoom cam.");
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("STATS");
	ImGui::Indent();
	ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
	ImGui::Unindent();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Flocking");
	ImGui::Spacing();

	ImGui::SliderFloat("Separation", &m_pBlendedSteering->GetWeightedBehaviorsRef()[0].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Cohesion", &m_pBlendedSteering->GetWeightedBehaviorsRef()[1].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("VelocityMatch", &m_pBlendedSteering->GetWeightedBehaviorsRef()[2].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Wander", &m_pBlendedSteering->GetWeightedBehaviorsRef()[3].weight, 0.f, 1.f, "%.2");
	ImGui::SliderFloat("Seek", &m_pBlendedSteering->GetWeightedBehaviorsRef()[4].weight, 0.f, 1.f, "%.2");

	bool isChecked = m_Agents[0]->CanRenderBehavior();
	ImGui::Checkbox("Render Debug", &isChecked);
	m_Agents[0]->SetRenderBehavior(isChecked);

	//check bool
	bool isChecked2 = m_SpacePart;
	ImGui::Checkbox("Spatial Partitioning", &isChecked2);
	if (isChecked2) { m_SpacePart = true; }
	if (!isChecked2) { m_SpacePart = false; }
	//set bool

	//End
	ImGui::PopAllowKeyboardFocus();
	ImGui::End();

}

//#ifndef USE_SPACE_PARTITIONING
void Flock::RegisterNeighbors(SteeringAgent* pAgent)
{
	//check if distance is smaller or equal to m_NeighborhoodRadius
	// register count of m_NrOfNeighbors
	m_NrOfNeighbors = 0;
	m_Neighbors.clear();
	for (int i = 0; m_FlockSize > i; ++i)
	{
		float distance = Elite::Distance(pAgent->GetPosition(), m_Agents[i]->GetPosition());
		if (m_NeighborhoodRadius >= distance) { m_Neighbors.push_back(m_Agents[i]); ++m_NrOfNeighbors; }

	}
}
//#endif

Elite::Vector2 Flock::GetAverageNeighborPos() const
{
	Elite::Vector2 total{};
	for (int i = 0; m_NrOfNeighbors > i; ++i)
	{
		total += m_Neighbors[i]->GetPosition();
	}
	Elite::Vector2 AveragePos = total / m_NrOfNeighbors;
	return AveragePos;
}

Elite::Vector2 Flock::GetAverageNeighborVelocity() const
{
	Elite::Vector2 total{};
	for (int i = 0; m_NrOfNeighbors > i; ++i)
	{
		total += m_Neighbors[i]->GetLinearVelocity();
	}
	Elite::Vector2 AverageVel = total / m_NrOfNeighbors;
	return AverageVel;
}

void Flock::SetSeekTarget(TargetData target)
{
	m_pSeekBehavior->SetTarget(target);
}

float* Flock::GetWeight(ISteeringBehavior* pBehavior)
{
	if (m_pBlendedSteering)
	{
		auto& weightedBehaviors = m_pBlendedSteering->GetWeightedBehaviorsRef();
		auto it = find_if(weightedBehaviors.begin(),
			weightedBehaviors.end(),
			[pBehavior](BlendedSteering::WeightedBehavior el)
			{
				return el.pBehavior == pBehavior;
			}
		);

		if (it != weightedBehaviors.end())
			return &it->weight;
	}

	return nullptr;
}
