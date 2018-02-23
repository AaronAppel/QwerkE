#include "stdafx.h"

Factory::Factory(ResourceManager* resources) // TODO: Get Resource Manager in constructor?
{
	m_pResources = resources;
}

Factory::~Factory()
{
}

void Factory::ClearResources()
{
	m_pResources->DeleteAllResources(); // TODO: Shouldn't have access to resources through factory
}
