#pragma once

#include <print>
#include <format>
#include <chrono>
#include <iomanip>

namespace Thorn {
	constexpr auto c_RESET = "\033[0m";

	constexpr auto c_GRAY = "\033[90m";
	constexpr auto c_GREEN = "\033[32m";
	constexpr auto c_AMBER = "\033[33m";
	constexpr auto c_RED = "\033[31m";

	class Log {
	public:
		template <typename ...Types>
		static void Trace(std::string_view fmt, Types&&... args);
		template <typename ...Types>
		static void Info(std::string_view fmt, Types&&... args);
		template <typename ...Types>
		static void Warn(std::string_view fmt, Types&&... args);
		template <typename ...Types>
		static void Error(std::string_view fmt, Types&&... args);

	private:
		template<typename ...Types>
		static void _Print(const char* color, std::string_view fmt, Types&&... args);

		static std::string _GetTime();
	};

	template<typename ...Types>
	inline void Log::Trace(std::string_view fmt, Types&&... args)
	{
		return _Print(c_GRAY, fmt, std::forward<Types>(args)...);
	}

	template<typename ...Types>
	inline void Log::Info(std::string_view fmt, Types&&... args)
	{
		return _Print(c_GREEN, fmt, std::forward<Types>(args)...);
	}

	template<typename ...Types>
	inline void Log::Warn(std::string_view fmt, Types&&... args)
	{
		return _Print(c_AMBER, fmt, std::forward<Types>(args)...);
	}

	template<typename ...Types>
	inline void Log::Error(std::string_view fmt, Types&&... args)
	{
		return _Print(c_RED, fmt, std::forward<Types>(args)...);
	}

	template<typename ...Types>
	inline void Log::_Print(const char* color, std::string_view fmt, Types&&... args)
	{
		std::string leader = std::format("{}[{}]: ", color, _GetTime());
		std::println("{}{}{}", leader, std::vformat(fmt, std::make_format_args(std::forward<Types>(args)...)), c_RESET);
	}

	inline std::string Log::_GetTime()
	{
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);

		std::tm tm{};
		localtime_s(&tm, &t);

		std::ostringstream oss;
		oss << std::put_time(&tm, "%T");
		return oss.str();
	}
}