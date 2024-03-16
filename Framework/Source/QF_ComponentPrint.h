#pragma once

#include <string>

static u16 counter = 0;

class ComponentPrint
{
public:
	ComponentPrint() : m_Position(counter++) { }

	std::string Content() { return m_Content; }
	u16 Position () { return m_Position; }

private:
	std::string m_Content = "empty";
	u16 m_Position = 0;
};
