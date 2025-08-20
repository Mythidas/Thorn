#pragma once

#include <functional>

#define THORN_BIND_FNC(fn)  [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Thorn
{
	template <typename... Args>
	class EventListener
	{
	private:
		using Func = std::function<bool(Args...)>;

	public:
		EventListener() {}
		EventListener(const Func& callback)
			: Callback(callback)
		{
		}

		bool operator()(Args... args) const
		{
			if (Callback)
				return Callback(args...);

			return false;
		}

		uint32_t ID;
		Func Callback;
	};

	template <typename... Args>
	class Event
	{
	public:
		void operator()(Args... args) const
		{
			for (const EventListener<Args...>& callback : m_Callbacks)
			{
				if (callback(args...))
					break;
			}
		}

		void operator +=(EventListener<Args...>& listener)
		{
			listener.ID = m_Counter++;
			m_Callbacks.push_back(listener);
		}

		void operator-=(const EventListener<Args...>& listener)
		{
			auto it = std::find_if(m_Callbacks.begin(), m_Callbacks.end(),
				[&](auto& l) { return l.ID == listener.ID; });
			if (it != m_Callbacks.end())
				m_Callbacks.erase(it);
		}

	private:
		std::vector<EventListener<Args...>> m_Callbacks;
		uint32_t m_Counter = 0;
	};
}