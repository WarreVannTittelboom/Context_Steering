/*=============================================================================*/
// Copyright 2020-2021 Elite Engine
/*=============================================================================*/
// StatesAndTransitions.h: Implementation of the state/transition classes
/*=============================================================================*/
#ifndef ELITE_APPLICATION_FSM_STATES_TRANSITIONS
#define ELITE_APPLICATION_FSM_STATES_TRANSITIONS

#include "projects/Shared/Agario/AgarioAgent.h"
#include "projects/Shared/Agario/AgarioFood.h"
#include "projects/Movement/SteeringBehaviors/Steering/SteeringBehaviors.h"
#include "framework/EliteAI/EliteData/EBlackboard.h"

//------------
//---STATES---
//------------
class WanderState : public Elite::FSMState
{
	virtual void OnEnter(Elite::Blackboard* pBlackboard) override
	{
		AgarioAgent* pAgent{ nullptr };
		bool sucsess = pBlackboard->GetData("Agent", pAgent);
		if (!sucsess) { return; }

		pAgent->SetToWander();
	}
private:
	float m_Timer{};
};

//-----------------
//---TRANSITIONS---
//-----------------

#endif