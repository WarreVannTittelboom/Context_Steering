/*=============================================================================*/
// Copyright 2021-2022 Elite Engine
// Authors: Matthieu Delaere, Thomas Goussaert
/*=============================================================================*/
// SteeringBehaviors.h: SteeringBehaviors interface and different implementations
/*=============================================================================*/
#ifndef ELITE_STEERINGBEHAVIORS
#define ELITE_STEERINGBEHAVIORS

//-----------------------------------------------------------------
// Includes & Forward Declarations
//-----------------------------------------------------------------
#include "../SteeringHelpers.h"
class SteeringAgent;
class Obstacle;

#pragma region **ISTEERINGBEHAVIOR** (BASE)
class ISteeringBehavior
{
public:
	ISteeringBehavior() = default;
	virtual ~ISteeringBehavior() = default;

	virtual SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) = 0;

	//Seek Functions
	void SetTarget(const TargetData& target) { m_Target = target; }

	template<class T, typename std::enable_if<std::is_base_of<ISteeringBehavior, T>::value>::type* = nullptr>
	T* As()
	{ return static_cast<T*>(this); }

protected:
	TargetData m_Target;
};
#pragma endregion

///////////////////////////////////////
//SEEK
//****
class Seek : public ISteeringBehavior
{
public:
	Seek() = default;
	virtual ~Seek() = default;

	//Seek Behaviour
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

/////////////////////////
//FLEE
//****
class Flee : public Seek
{
public:
	Flee() = default;
	virtual ~Flee() = default;

	//Seek Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

class Arrive : public Seek
{
public:
	Arrive() = default;
	virtual ~Arrive() = default;

	//Seek Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

class Wandering : public Seek
{
public:
	Wandering() = default;
	virtual ~Wandering() = default;

	//Seek Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;

	void SetWanderOffset(float offset) { m_OffsetDistance = offset; }
	void SetWanderRadius(float radius) { m_Radius = radius; }
	void SetMaxAngleChange(float rad) { m_MaxAngleChange = rad; }

protected:
	Elite::Vector2 m_Velocity{1.f,0.f};
	float m_OffsetDistance = 6.f;
	float m_Radius = 6.f;
	int m_MaxAngleChange = 45;
	float m_WanderAngle = Elite::ToRadians(rand() % 360);
	Elite::Vector2 WanderTarget{1,1};
	Elite::Vector2 CircleCenter{};
};

class Face : public Seek
{
public:
	Face() = default;
	virtual ~Face() = default;

	//Seek Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

class Pursuit : public Seek
{
public:
	Pursuit() = default;
	virtual ~Pursuit() = default;

	//Seek Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

class Evade : public Seek
{
public:
	Evade() = default;
	virtual ~Evade() = default;

	//Seek Behavior
	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
};

//////////////////////////////////////
//CONTEXT STEERING
//****
class ContextSteering : public Seek
{
public:
	ContextSteering() = default;
	virtual ~ContextSteering() = default;

	SteeringOutput CalculateSteering(float deltaT, SteeringAgent* pAgent) override;
	
	void SetSizeArray(int size);
	void AddObstacle(Obstacle* obstacle) { m_pObstacles.push_back(obstacle); };

private:
	bool IsArrowIntersecting(const Elite::Vector2& origin, const Elite::Vector2& dir, Obstacle* pObstacle);
	
	//Switch between smooth danger handling (interpolation) or normal danger handling (ignore intrest if any danger)
	bool m_InterpolateDangerValues = false;
	//legnth of arrows
	float m_LookAheadRange = 12.f;
	size_t m_arrowCount;
	std::vector<Obstacle*> m_pObstacles{};
	std::vector<Elite::Vector2> m_Directions;
	std::vector<float> m_Interests;
	std::vector<float> m_Dangers;
	std::vector<Elite::Color> m_Colors;
};

#endif


