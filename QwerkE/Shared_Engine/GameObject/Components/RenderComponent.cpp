#include "stdafx.h"

RenderComponent::RenderComponent()
{
	m_Colour.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_pMesh = nullptr;
	m_pShader = nullptr;
	m_CurrentTexture = NULL;
	m_ComponentTag = Component_Render;
}

RenderComponent::~RenderComponent()
{
}