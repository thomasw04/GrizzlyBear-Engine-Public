#include <gbpch.h>
#include <stdexcept>

#include "Executor.h"

namespace GrizzlyBear {

	bool Executable::is_paused()
	{
		std::lock_guard<std::mutex> lock(m_ControlLock);
		return m_Paused && m_InIntSection;
	}

	bool Executable::should_pause()
	{
		std::lock_guard<std::mutex> lock(m_ControlLock);
		return m_Paused;
	}

	void Executable::interrupt_point()
	{
		std::unique_lock<std::mutex> lock(m_ControlLock);

		m_InIntSection = true;

		if (m_Paused)
		{
			m_Pause.wait(lock, [&] { return !m_Paused; });
		}

		m_InIntSection = false;

		if (m_ShouldClose)
		{
			lock.unlock();
			throw GRIZZLY_EXEC_SUSPENDED;
			return;
		}
	}

	void Executable::pause()
	{
		std::lock_guard<std::mutex> lock(m_ControlLock);
		m_Paused = true;
	}

	void Executable::resume()
	{
		{
			std::lock_guard<std::mutex> lock(m_ControlLock);
			m_Paused = false;
		}

		m_Pause.notify_one();
	}

	void Executable::suspend()
	{
		std::lock_guard<std::mutex> lock(m_ControlLock);
		m_ShouldClose = true;
	}

	const std::vector<Gstring> Executable::read_command()
	{
		std::lock_guard<std::mutex> lock(m_CommandQueueLock);
		std::vector<Gstring> cmd = m_CommandQueue.front();
		m_CommandQueue.pop();
		return cmd;
	}

	bool Executable::has_command()
	{
		return !m_CommandQueue.empty();
	}

	void Executable::push_command(const std::vector<Gstring>& cmd)
	{
		std::lock_guard<std::mutex> lock(m_CommandQueueLock);
		m_CommandQueue.push(cmd);
	}

	Gstring Executable::read_result()
	{
		std::lock_guard<std::mutex> lock(m_ResultQueueLock);
		Gstring result = m_ResultQueue.front();
		m_ResultQueue.pop();
		return result;
	}

	bool Executable::has_result()
	{
		return !m_ResultQueue.empty();
	}

	void Executable::push_result(const Gstring& result)
	{
		std::lock_guard<std::mutex> lock(m_ResultQueueLock);
		m_ResultQueue.push(result);
	}

	void Executor::start()
	{
		m_ExecutableFuture = std::async(std::launch::async, &Executor::run, &(*this), 0, nullptr);
	}

	void Executor::pause()
	{
		m_Executable->pause();
	}

	void Executor::resume()
	{
		m_Executable->resume();
	}

	bool Executor::is_alive()
	{
		auto status = m_ExecutableFuture.wait_for(std::chrono::milliseconds(0));

		if (status == std::future_status::ready)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	bool Executor::is_paused()
	{
		return m_Executable->is_paused();
	}

	bool Executor::should_pause()
	{
		return m_Executable->should_pause();
	}

	const std::vector<Gstring> Executor::read_command()
	{
		return m_Executable->read_command();
	}

	bool Executor::has_command()
	{
		return m_Executable->has_command();
	}

	void Executor::push_command(const std::vector<Gstring>& cmd)
	{
		m_Executable->push_command(cmd);
	}

	Gstring Executor::read_result()
	{
		return m_Executable->read_result();
	}

	bool Executor::has_result()
	{
		return m_Executable->has_result();
	}

	void Executor::push_result(const Gstring& result)
	{
		m_Executable->push_result(result);
	}

	void Executor::suspend(bool wait)
	{
		m_Executable->suspend();

		if (wait)
		{
			m_ExecutableFuture.wait();
		}
	}

	int Executor::run(int argc, char* argv[])
	{
		int errorCode = 0;

		try
		{
			errorCode = m_Executable->run(argc, argv);
		}
		catch (int error)
		{
			errorCode = error;
		}

		return errorCode;
	}

}