#include "pch.h"
#include "Box2DContactListener.h"

void Box2DContactListener::BeginContact(b2Contact* contact)
{
	b2WorldManifold* tempManifold = new b2WorldManifold();
	contact->GetWorldManifold(tempManifold);

	contact->GetFixtureA();
	contact->GetFixtureB();

	delete tempManifold;
}

void Box2DContactListener::EndContact(b2Contact* contact)
{
	B2_NOT_USED(contact);
}

void Box2DContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	B2_NOT_USED(contact);
	B2_NOT_USED(oldManifold);
}

void Box2DContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	B2_NOT_USED(contact);
	B2_NOT_USED(impulse);
}