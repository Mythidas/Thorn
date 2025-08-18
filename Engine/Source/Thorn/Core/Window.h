#pragma once

#include "Thorn/Core/Event.h"
#include "Thorn/Lib/Pointers.h"

#include <string>

namespace Thorn {
	struct WindowSpecs
	{
		std::string Title = "Thorn Engine";
		uint32_t Width = 800;
		uint32_t Height = 600;
	};
	
	class Window {
	public:
		static Event<> OnWindowClose; // No arguments
		static Event<int, int> OnWindowResize; // Width, Height
		static Event<int> OnKeyPressed; // Key
		static Event<int> OnKeyReleased; // Key
		static Event<double, double> OnMouseMoved; // Mouse X, Mouse Y
		static Event<float> OnMouseScrolled;
		static Event<int> OnMouseButtonPressed; // Button
		static Event<int> OnMouseButtonReleased; // Button

	public:
		THORN_MAKE_SCOPE(Window)
		~Window();

		void OnUpdate();

		uint32_t GetWidth() const { return m_Specs.Width; }
		uint32_t GetHeight() const { return m_Specs.Height; }
	private:
		Window(const WindowSpecs& specs);

		void* m_NativeWindow;
		WindowSpecs m_Specs;
	};
}
