#pragma once

#include <functional>

#define THORN_BIND_FNC(fn)  [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Thorn
{
	template <typename... Args>
	class Event
	{
	private:
		using Func = std::function<bool(Args...)>;

	public:
		void operator()(Args... args)
		{
			for (auto& callback : m_Callbacks)
			{
				if (callback(args...))
					break;
			}
		}

		void operator +=(const Func& callback)
		{
			m_Callbacks.push_back(callback);
		}

	private:
		std::vector<Func> m_Callbacks;
	};
}