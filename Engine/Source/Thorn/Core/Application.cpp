#include "Thorn/pch.h"
#include "Application.h"

#include "Thorn/Graphics/Camera.h"
#include "Thorn/Graphics/Transform.h"
#include "Thorn/Graphics/Renderer.h"

namespace Thorn
{
	Application::Application()
	{
		WindowSpecs windowSpecs{};
		m_Window = Window::Create(windowSpecs);
		
		Renderer::_Init();
	}

	void Application::Run()
	{
		Camera camera{};
		Transform transform{};

		camera.ClearColor = { 1.0f, 0.7f, 0.4f, 1.0f };

		while (true)
		{
			m_Window->OnUpdate();

			Renderer::BeginFrame(camera, transform);
			Renderer::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
			Renderer::EndFrame();
		}
	}
}