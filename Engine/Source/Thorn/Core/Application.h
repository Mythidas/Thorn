#pragma once

#include "Thorn/Lib/Singleton.h"
#include "Thorn/Lib/Pointers.h"
#include "Window.h"
#include "LayerStack.h"

namespace Thorn {
	class Application : public Singleton<Application>
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		inline void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
		inline void PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }

		Scope<Window>& GetWindow() { return m_Window; }

	private:
		Scope<Window> m_Window;
		LayerStack m_LayerStack;

		bool m_Running = false;

		EventListener<> m_WindowCloseListener;
		EventListener<int, int> m_WindowResizeListener;

		bool _OnWindowClose();
		bool _OnWindowResize(int width, int height);
	};

	static Application* CreateApplication();
}