#pragma once

#include "Thorn/Lib/Pointers.h"
#include "Thorn/Math/Vector.h"

namespace Thorn
{
	class ImGUI
	{
	public:
		THORN_MAKE_REF(ImGUI)
		~ImGUI();

		void BeginUI();
		void EndUI(const UVector2& size);

	public:
		static void UseDarknessTheme();
		static void UseGrayTheme();

	private:
		ImGUI(void* nativeWindow);
	};
}