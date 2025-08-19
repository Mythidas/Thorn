#include "Thorn/pch.h"
#include "Application.h"

#include "Thorn/Graphics/Camera.h"
#include "Thorn/Graphics/Transform.h"
#include "Thorn/Graphics/Renderer.h"
#include "Time.h"

namespace Thorn
{
	Application::Application()
	{
		WindowSpecs windowSpecs{};
		m_Window = Window::Create(windowSpecs);
		
		Renderer::_Init();

		m_WindowCloseListener = EventListener<>(THORN_BIND_FNC(_OnWindowClose));
		Window::OnWindowClose += m_WindowCloseListener;
	}

	Application::~Application()
	{
		m_LayerStack.Clear();
		Renderer::_Destroy();

		Log::Info("Application shutdown...");
	}

	void Application::Run()
	{
		m_Running = true;

		while (m_Running)
		{
			Time::_Tick();
			m_Window->OnUpdate();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			if (Time::GetFixedDeltaTime() > 0.0f)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnFixedUpdate();
				}
			}

			m_LayerStack.Flush();
		}
	}

	bool Application::_OnWindowClose()
	{
		m_Running = false;
		return false;
	}
}