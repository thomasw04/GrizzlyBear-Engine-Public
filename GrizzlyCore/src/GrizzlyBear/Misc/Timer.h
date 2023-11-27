#pragma once

#include <chrono>
#include <iostream>

namespace GrizzlyBear {

	using TimeUnit = unsigned long long;
	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

	struct TimingFunctions
	{
		static bool can_run(TimePoint last, TimeUnit interval);
		static TimePoint now();
	};

	struct Timer
	{
		Timer(const char* name);
		~Timer();

		void stop();

		void print_absolute();
		void print_average();

		void round();

	private:
		const char* m_Name;
		TimePoint m_StartTimepoint;
		TimePoint m_RelativeStartTimepoint;
		bool m_Stopped;

		double m_Sum = 0;
		int m_Points = 0;
		double m_Absolute = 0;
	};
}
