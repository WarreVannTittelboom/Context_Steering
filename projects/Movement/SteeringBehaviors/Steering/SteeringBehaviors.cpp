//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

//Includes
#include "SteeringBehaviors.h"
#include "../SteeringAgent.h"
#include "../Obstacle.h"
#include "framework\EliteMath\EMatrix2x3.h"

//SEEK
//****
SteeringOutput Seek::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0,1,0,1 });
	}
	return steering;
}

//FLEE
//****
SteeringOutput Flee::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = pAgent->GetPosition() - m_Target.Position;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0,1,0,1 });
		
	}



	return steering;
}

SteeringOutput Arrive::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};

	steering.LinearVelocity = m_Target.Position - pAgent->GetPosition();
	float distance = sqrtf(steering.LinearVelocity.x * steering.LinearVelocity.x + steering.LinearVelocity.y * steering.LinearVelocity.y);
	float SlowingDistance = pAgent->GetRadius() * 2;
	
		if (distance < SlowingDistance)
		{
			steering.LinearVelocity.Normalize();
			steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();
			steering.LinearVelocity *= (distance / SlowingDistance);
		}
	
	else
	{ 
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();
	}
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0,1,0,1 });
		
	}
	return steering;
}

SteeringOutput Wandering::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	

	

	

	Elite::Vector2 Dist = WanderTarget - pAgent->GetPosition();
	if (rand() % 2 == 1)
	{
		m_WanderAngle += Elite::ToRadians(rand() % m_MaxAngleChange);
	}
	else { m_WanderAngle -= Elite::ToRadians(rand() % m_MaxAngleChange); } 
	Elite::Vector2 offset = pAgent->GetLinearVelocity().GetNormalized();
	CircleCenter.x = pAgent->GetPosition().x + offset.x *  m_OffsetDistance;
	CircleCenter.y = pAgent->GetPosition().y + offset.y * m_OffsetDistance;
	
	
	WanderTarget.x = CircleCenter.x + m_Radius * sin(m_WanderAngle);
	WanderTarget.y = CircleCenter.y + m_Radius * cos(m_WanderAngle);

	steering.LinearVelocity = WanderTarget - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();
	


	if (pAgent->CanRenderBehavior())
	{
		
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0,1,0,1 });
		DEBUGRENDERER2D->DrawCircle(CircleCenter, m_Radius,Elite::Color{ 0,0,1,1 }, 0.9f);
	}
	return steering;
}

SteeringOutput Face::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	pAgent->SetAutoOrient(false);
	float RotationSpeed{ 1000.f };
	Elite::Vector2 toTarget = m_Target.Position - pAgent->GetPosition();
	float angletoTarget = atan2(toTarget.y, toTarget.x);
	float angle = angletoTarget - pAgent->GetOrientation() + E_PI_2;
	steering.AngularVelocity= RotationSpeed* deltaT * angle;
	steering.LinearVelocity = Elite::ZeroVector2;

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0,1,0,1 });

	}
	return steering;
}


SteeringOutput Pursuit::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	Elite::Vector2 distance = m_Target.Position - pAgent->GetPosition();
	int t = distance.Normalize() / pAgent->GetMaxAngularSpeed();
	Elite::Vector2 FuturePos = m_Target.Position + m_Target.LinearVelocity * t;
	steering.LinearVelocity = FuturePos - pAgent->GetPosition();
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();

	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0,1,0,1 });

	}
	return steering;
}

SteeringOutput Evade::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{
	SteeringOutput steering = {};
	Elite::Vector2 distance = m_Target.Position - pAgent->GetPosition();
	int t = distance.Normalize() / pAgent->GetMaxAngularSpeed();
	Elite::Vector2 FuturePos = m_Target.Position + m_Target.LinearVelocity * t;
	steering.LinearVelocity =  pAgent->GetPosition() - FuturePos;
	steering.LinearVelocity.Normalize();
	steering.LinearVelocity *= pAgent->GetMaxAngularSpeed();
	
	if (pAgent->CanRenderBehavior())
	{
		DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), steering.LinearVelocity, 5.f, Elite::Color{ 0,1,0,1 });

	}
	return steering;
}