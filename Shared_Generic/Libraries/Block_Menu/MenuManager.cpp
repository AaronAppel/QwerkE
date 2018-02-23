#include "stdafx.h"

MenuManager::MenuManager()
{
}

MenuManager::~MenuManager()
{
	for (uint i = 0; i < m_Menus.size(); i++)
	{
		delete m_Menus.at(i);
	}
	m_Menus.clear();
}

void MenuManager::Update(vec2 mousePos)
{
	for (uint i = 0; i < m_Menus.size(); i++)
	{
		m_Menus.at(i)->Update(vec2(mousePos.x, mousePos.y));
	}
}

void MenuManager::Draw()
{
	for (uint i = 0; i < m_Menus.size(); i++)
	{
		m_Menus.at(i)->Draw();
	}
}

void MenuManager::Click(vec2 mousePos, int button)
{
	if (button == Button_MouseLeft) // 0
	{
		for (uint i = 0; i < m_Menus.size(); i++)
		{
			m_Menus.at(i)->LeftClick(vec2(mousePos.x, mousePos.y));
		}
	}
	else if (button == Button_MouseRight)
	{

	}
	else if (button == Button_MouseMiddle)
	{

	}
}