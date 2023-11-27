#include <gbpch.h>

#include "Timer.h"

namespace GrizzlyBear {

	Timer::Timer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::steady_clock::now();
		m_RelativeStartTimepoint = m_StartTimepoint;
	}

	Timer::~Timer()
	{
		if (!m_Stopped)
		{
			stop();
		}

		print_absolute();
		print_average();
	}

	void Timer::stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();

		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();
		auto elapsedTimeRel = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_RelativeStartTimepoint).time_since_epoch();

		m_Stopped = true;

		m_Absolute = elapsedTime.count() / 1000.0;

		m_Sum += elapsedTimeRel.count() / 1000.0;
		m_Points++;
	}

	void Timer::print_absolute()
	{
		std::cout << m_Name << " passed with a absolute duration of " << m_Absolute << "ms" << std::endl;
	}

	void Timer::print_average()
	{
		std::cout << m_Name << " passed with a average duration of " << (m_Sum / m_Points) << "ms" << std::endl;
	}

	void Timer::round()
	{
		auto endTimepoint = std::chrono::steady_clock::now();

		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_RelativeStartTimepoint).time_since_epoch();
		m_RelativeStartTimepoint = endTimepoint;

		m_Sum += elapsedTime.count() / 1000.0;
		m_Points++;

		std::cout << "Round: " << (elapsedTime.count() / 1000.0) << std::endl;
	}

	bool TimingFunctions::can_run(TimePoint last, TimeUnit interval)
	{
		auto now = std::chrono::steady_clock::now();

		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(now).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(last).time_since_epoch();

		if (elapsedTime.count() / 1000.0 >= interval)
		{
			return true;
		}

		return false;
	}


	TimePoint TimingFunctions::now()
	{
		return std::chrono::steady_clock::now();
	}
}
