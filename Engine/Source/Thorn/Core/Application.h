#pragma once

#include "Thorn/Lib/Singleton.h"
#include "Thorn/Lib/Pointers.h"
#include "Window.h"

namespace Thorn {
	class Application : public Singleton<Application>
	{
	public:
		Application();

		void Run();

		Scope<Window>& GetWindow() const { return Get().m_Window; }
	private:
		Scope<Window> m_Window;
	};

	static Application* CreateApplication();
}