#include "pch.h"
#include "Box2DWorld.h"

Box2DWorld::Box2DWorld(b2Vec2 Gravity) :
	m_UnusedTime(0.0f),
	m_TimeStep(1 / 60.0f),
	m_VelocityIterations(8),
	m_PositionIterations(3),
	m_DebugDraw(nullptr)
{
	m_World = new b2World(Gravity);
	m_ContactListener = new Box2DContactListener();
}

Box2DWorld::~Box2DWorld()
{
	delete m_World;
	delete m_ContactListener;
	if (m_DebugDraw)
		delete m_DebugDraw;
}

void Box2DWorld::InitializeDebugDraw(ShaderProgram* debugdrawshader, mat4* pMatView, mat4* pMatProj)
{
	if (!m_DebugDraw)
	{
		m_DebugDraw = new Box2DDebugDraw(debugdrawshader, pMatView, pMatProj);
		m_World->SetDebugDraw(m_DebugDraw);

		uint32 flags = b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit | b2Draw::e_aabbBit | b2Draw::e_pairBit;
		m_DebugDraw->SetFlags(flags);
	}
}

void Box2DWorld::AddContactListener(Box2DContactListener* listener)
{
	m_World->SetContactListener((b2ContactListener*)(listener));
}

b2Body* Box2DWorld::CreateBody(const b2BodyDef *BodyDef)
{
    return m_World->CreateBody(BodyDef);
}

void Box2DWorld::Update()
{	
	// m_UnusedTime += deltaTime;

	m_World->Step(m_TimeStep, m_VelocityIterations, m_PositionIterations);
	m_World->SetContactListener(m_ContactListener);

	m_UnusedTime -= m_TimeStep;

	if (0) // too little time passed m_UnusedTime < value
	{
		m_World->ClearForces();
	}
}