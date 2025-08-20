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
		m_ImGUI = ImGUI::Create(m_Window->GetNativeWindow());
		
		Renderer::_Init();
		Renderer::Resize({ windowSpecs.Width, windowSpecs.Height });

		m_WindowCloseListener = EventListener<>(THORN_BIND_FNC(_OnWindowClose));
		m_WindowResizeListener = EventListener<int, int>(THORN_BIND_FNC(_OnWindowResize));

		Window::OnWindowClose += m_WindowCloseListener;
		Window::OnWindowResize += m_WindowResizeListener;
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

#ifdef THORN_DEBUG
			m_ImGUILayer->BeginUI();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnRenderUI();
			}
			m_ImGUILayer->EndUI();
#endif

			m_LayerStack.Flush();
		}
	}

	bool Application::_OnWindowClose()
	{
		m_Running = false;
		return false;
	}

	bool Application::_OnWindowResize(int width, int height)
	{
		Renderer::Resize({ width, height });
		return false;
	}
}