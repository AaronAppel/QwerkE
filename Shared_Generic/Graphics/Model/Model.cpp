#include "stdafx.h"

Model::Model()
{
}

Model::~Model()
{
	for (uint i = 0; i < m_HotMeshes.size(); i++)
	{
		delete m_HotMeshes.at(i);
	}
}

void Model::SetupMeshes(ShaderProgram* shader)
{
    for (uint i = 0; i < m_HotMeshes.size(); i++)
    {
        m_HotMeshes.at(i)->SetupShaderAttributes(shader);
    }
}

void Model::Draw()
{
	for (uint i = 0; i < m_HotMeshes.size(); i++)
	{
		m_HotMeshes.at(i)->Draw();
	}
}