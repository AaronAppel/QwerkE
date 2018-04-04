#include "Renderer.h"
#include "Graphics/FontRendering/FontTest.h"
#include "ServiceLocator.h"

Renderer::Renderer()
{
	LoadFonts();
}

Renderer::~Renderer()
{
}

void Renderer::DrawFont(const char* text)
{
	ResourceManager* resourceManager = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
	RenderText(resourceManager->GetShader("text"),
		std::string("Sample Text"),
		100, 100, // x, y
		1.0f, // scale
		glm::vec3(0,0,0));
}