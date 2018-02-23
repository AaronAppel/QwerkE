#ifndef __Box2DWorld_H__
#define __Box2DWorld_H__

class Box2DContactListener;

class Box2DWorld
{
public:
    Box2DWorld(b2Vec2 Gravity);
    ~Box2DWorld();

	void AddContactListener(Box2DContactListener* listener);
    b2Body* CreateBody(const b2BodyDef *BodyDef);
	void InitializeDebugDraw(ShaderProgram* debugdrawshader, mat4* pMatView, mat4* pMatProj);
    void Update();

	b2World* GetWorld() { return m_World; };
	
private:
	float m_UnusedTime;
	float m_TimeStep;
	int m_VelocityIterations;
	int m_PositionIterations;
    
	b2World *m_World;
	Box2DContactListener *m_ContactListener;
	Box2DDebugDraw* m_DebugDraw;
};

#endif //__Box2DWorld_H__
