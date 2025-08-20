#pragma once

#include <chrono>

namespace Thorn
{
	class Time
	{
	private:
		friend class Application;
		typedef std::chrono::steady_clock::time_point TimePoint;

	public:
		static const float GetFPS() { return s_AverageFPS; }
		static const float GetMSPerFrame() { return 1.0f / s_AverageFPS * 1000.0f; }
		static const float GetDeltaTime() { return s_DeltaTime; }
		static const float GetFixedDeltaTime() { return s_FixedDeltaTime; }

	private:
		static void _Tick();
		static const float FIXED_DELTA_TIME;

	private:
		static TimePoint s_LastFrame;
		static TimePoint s_LastFixedFrame;
		static std::vector<float> s_FrameTimes;

		static float s_AverageFPS;
		static float s_DeltaTime;
		static float s_FixedDeltaTime;
	};
}