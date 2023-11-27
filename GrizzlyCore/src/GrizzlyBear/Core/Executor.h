#pragma once

#include "BaseDefines.h"

#include <mutex>

#include <queue>

#define GRIZZLY_EXEC_OK 0
#define GRIZZLY_EXEC_SUSPENDED 1

namespace GrizzlyBear {

	class Executable
	{
		friend class Executor;

	public:
		virtual int run(int argc, char* argv[]) = 0;
		bool is_paused();
		bool should_pause();

		void interrupt_point();

		void pause();
		void resume();

		void suspend();

		const std::vector<Gstring> read_command();
		bool has_command();
		void push_command(const std::vector<Gstring>& cmd);

		Gstring read_result();
		bool has_result();
		void push_result(const Gstring& result);

	private:
		std::mutex m_ControlLock;
		bool m_ShouldClose = false;
		bool m_Paused = false;
		bool m_InIntSection = false;
		std::condition_variable m_Pause;

		std::mutex m_CommandQueueLock;
		std::queue<std::vector<Gstring>> m_CommandQueue;
		std::mutex m_ResultQueueLock;
		std::queue<Gstring> m_ResultQueue;
	};

	class Executor
	{
	public:
		template<typename T, typename... Args>
		explicit Executor(Tag<T> type, Args... args) requires std::is_base_of_v<Executable, T>
		{
			m_Executable = std::make_unique<T>(std::forward(args)...);
		}

		template<typename T>
		explicit Executor(T& exec) requires std::is_base_of_v<Executable, T>
		{
			m_Executable = new T(std::move(exec));
		}

		void start();

		void pause();
		void resume();

		bool is_alive();
		bool is_paused();
		bool should_pause();

		const std::vector<Gstring> read_command();
		bool has_command();
		void push_command(const std::vector<Gstring>& cmd);

		Gstring read_result();
		bool has_result();
		void push_result(const Gstring& result);

		void suspend(bool wait = false);
	private:
		int run(int argc, char* argv[]);

	private:
		std::future<int> m_ExecutableFuture;
		std::unique_ptr<Executable> m_Executable;
	};

}
