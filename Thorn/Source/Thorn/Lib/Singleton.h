#pragma once

#include <type_traits>

#include "Thorn/Lib/Log.h"

namespace Thorn
{
	template <typename T>
	class Singleton
	{
	public:
		Singleton()
		{
			if (s_Instance == nullptr)
			{
				s_Instance = static_cast<T*>(this);
			}
			else
			{
				Log::Error("Instance already exists!");
			}
		}

		static T& Get() { return *s_Instance; }

	private:
		static T* s_Instance;
	};

	template <typename T>
	inline T* Singleton<T>::s_Instance = nullptr;
}