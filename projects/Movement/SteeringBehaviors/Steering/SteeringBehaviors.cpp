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

// CONTEXT STEERING
//****
SteeringOutput ContextSteering::CalculateSteering(float deltaT, SteeringAgent* pAgent)
{

	Elite::Vector2 agentPos = pAgent->GetPosition();
	float agentDir = pAgent->GetOrientation();
	float maxLinSpeed = pAgent->GetMaxLinearSpeed();
	

	for (size_t i = 0; i < m_arrowCount; ++i)
	{
		float angle = (((2 * (float)M_PI) / m_arrowCount) * (int)i);
		Elite::Vector2 rightVector{ 1,0 };
		Elite::Vector2 currVector{};

		currVector.x = (rightVector.x * cos(agentDir + angle)) - (rightVector.y * sin(agentDir + angle));
		currVector.y = (rightVector.x * sin(agentDir + angle)) - (rightVector.y * cos(agentDir + angle));
		currVector = currVector.GetNormalized();
		currVector *= m_LookAheadRange;
		m_Directions[i] = currVector;
	}



	Elite::Vector2 toTarget{Elite::Vector2 {m_Target.Position - pAgent->GetPosition()}.GetNormalized()};

	for (size_t i = 0; i < m_arrowCount; i++)
	{
		float dotProdcut = m_Directions[i].GetNormalized().Dot(toTarget);
		if (0.f >= dotProdcut)
		{
			m_Interests[i] = 0;
		}
		else
		{
			m_Interests[i] = dotProdcut;
		}
	}

	for (size_t i = 0; i < m_arrowCount; i++)
	{
		bool isActive = false;
		
		for (size_t j = 0; j < m_pObstacles.size(); j++)
		{
			if (IsArrowIntersecting(agentPos, m_Directions[i], m_pObstacles[j]))
			{
				if (m_InterpolateDangerValues)
				{
					float distance = Elite::Distance(agentPos, m_pObstacles[j]->GetCenter());
					distance -= m_pObstacles[j]->GetRadius();
					
					m_Dangers[i] = (m_LookAheadRange - distance) / m_LookAheadRange;
				}
				else
				{
					m_Dangers[i] = 1;
				}
				m_Colors[i] = Elite::Color(m_Dangers[i], 1 - m_Dangers[i], 0);
				isActive = true;
			}
			else if (!isActive)
			{
				m_Colors[i] = Elite::Color(0, 1, 0);
				m_Dangers[i] = 0.f;
			}
		}

	}
	if (pAgent->CanRenderBehavior())
	{
		for (size_t i = 0; i < m_arrowCount; i++)
		{
			DEBUGRENDERER2D->DrawDirection(pAgent->GetPosition(), m_Directions[i] * m_Interests[i], (m_Directions[i] * m_Interests[i]).Magnitude(), m_Colors[i]);
		}
	}

	
	for (size_t i = 0; i < m_arrowCount; i++)
	{
		m_Interests[i] -= m_Dangers[i];
	}

	
	Elite::Vector2 finalDir{};
	for (size_t i = 0; i < m_arrowCount; i++)
	{
		finalDir += m_Directions[i] * m_Interests[i];
	}
	finalDir = finalDir.GetNormalized();
	finalDir *= maxLinSpeed;
	
	
	auto output = SteeringOutput();
	output.LinearVelocity = finalDir;
	output.AngularVelocity = 0.0f;
	output.IsValid = true;
	return output;
}

bool ContextSteering::IsArrowIntersecting(const Elite::Vector2& origin, const Elite::Vector2& dir, Obstacle* pObstacle)
{
	Elite::Vector2 closestPoint = ProjectOnLineSegment(origin, origin + dir, pObstacle->GetCenter());
	float distanceToCenter = Elite::Distance(pObstacle->GetCenter(), closestPoint);

	if (pObstacle->GetRadius() >= distanceToCenter)
	{
		return true;
	}
	return false;
}

void ContextSteering::SetSizeArray(int size)
{
	m_arrowCount = size;
	
	m_Directions.resize(m_arrowCount);
	m_Interests.resize(m_arrowCount);
	m_Dangers.resize(m_arrowCount);
	m_Colors.resize(m_arrowCount);
}

