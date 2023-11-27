#pragma once

#include <boost/thread/thread.hpp>

//#define _SILENCE_CLANG_COROUTINE_MESSAGE
//#include <experimental/coroutine>

#include <coroutine>

#include <functional>
#include <vector>
#include <mutex>
#include <queue>
#include <deque>
#include <stack>
#include <unordered_map>

#include <GrizzlyBear/Core/BaseDefines.h>

#include "GrizzlyBear/Misc/Timer.h"

namespace GrizzlyBear {

	//Class declarations-------------------------------------------------------------------
	class Thread;
	class ThreadActions;
	class ThreadManager;
	class Context;
	struct RunnableHandle;
	struct Processor;
	struct Task;

	template<std::movable T>
	class ThreadState;

	using ThreadFunc = std::function<ThreadState<int>(ThreadActions)>;
	using TaskFunc = std::function<void()>;

	using RunHNDL = RunnableHandle;

	//Definitions--------------------------------------------------------------------------

	struct RunnableHandle
	{
		RunnableHandle(int id = 0);

		operator int() const;

		static RunnableHandle null();

	private:
		int m_ID;
	};

	//Thread queueing----------------------------------------------------------------------

	template<std::movable T>
	class ThreadState
	{
	public:

		struct promise_type;
		using Handle = std::coroutine_handle<promise_type>;

		struct promise_type
		{
			ThreadState<T> get_return_object();

			static std::suspend_always initial_suspend() noexcept;
			static std::suspend_always final_suspend() noexcept;

			std::suspend_always yield_value(T value) noexcept;
			void return_value(T value) noexcept;

			void unhandled_exception();

			std::optional<T> m_CurrentValue;
		};

		explicit ThreadState(const Handle handle);

		ThreadState() = default;

		~ThreadState();

		void resume();
		void reset() const;
		const T get_value();
		bool done();

	private:

		mutable Handle m_Task;
	};	

	class ThreadActions
	{
		friend class ThreadManager;

	public:
		ThreadActions(RunnableHandle identifier) : m_ID(identifier) {}

		int spawn_child(ThreadFunc childFunc);
		bool is_ready(RunnableHandle taskHandle);
		RunnableHandle get_identifier() const;

	private:
		RunnableHandle m_ID = 0;
	};

	class Thread
	{
		friend struct Processor;

	public:
		Thread(ThreadActions actions, ThreadState<int> state);
		Thread(ThreadActions actions, ThreadFunc func);
		~Thread();

		RunnableHandle get_identifier() const;
		void wait();

	private:

		void finish();

	private:

		ThreadActions m_Actions;

		std::mutex m_WaiterLock;
		bool m_Ready = false;
		std::condition_variable m_Waiter;

	public:
		ThreadState<int> m_Coroutine;
	};

	struct Processor
	{
		std::mutex m_ProcessorLock;
		std::deque<Ref<Thread>> m_Threads;
		boost::thread m_Processor;

		std::mutex m_ControlLock;
		RunnableHandle m_CurrentThreadID = 0;
		bool m_Sleeps = false;
		int m_OwnID = 0;

		Processor();
		
		void start();
		void run();

		void add_thread(Ref<Thread> thread, bool immediately = false);

		void sleep();
		void wake_up();
		void panic();
		bool try_steal();
	};

	//Task Scheduler---------------------------------------------------------------------

	struct Task
	{
		TaskFunc m_Func;
		TimeUnit m_Delay;
		TimeUnit m_Interval;

		TimePoint m_LastInterrupt;

		RunnableHandle m_Handle;
	};

	class Context
	{
	public:
		Context();
		~Context();

		RunnableHandle register_task(TimeUnit delay, TimeUnit interval, TaskFunc func);
		bool suspend_task(RunnableHandle task);

		void run();

	private:
		std::mutex m_ContextLock;
		std::vector<Task> m_Tasks;

		std::mutex m_ControlLock;
		int m_IdCounter = 0;

		boost::thread m_Processor;
		bool m_Running;
	};

	//ThreadManager----------------------------------------------------------------------

	class ThreadManager
	{
		friend class Thread;
		friend class ThreadActions;
		friend struct Processor;

	public:
		static ModuleInfo init();
		static void shutdown();
		static void panic();

		//Thread queueing
		static RunnableHandle queue_thread(ThreadFunc func, bool wait = false, bool immediately = false);
		static RunnableHandle queue_thread(Ref<Thread> thread, bool wait = false, bool immediately = false);

		static void wake_up_all();

		//Task scheduler
		static Ref<Context> create_context();

	private:

		static RunnableHandle generate_identifier();
		static void free_identifier(RunnableHandle handle);

		static void update_assigned_processor(RunnableHandle identifier, int newProcessor);

	public:
		//Processors
		static std::vector<Processor> s_Processors;

		//Lock + Queue
		static std::mutex s_StallQueueLock;
		static std::unordered_map<RunnableHandle, std::queue<Ref<Thread>>> s_Stalled;

		//Lock + ThreadIdentifier
		static std::mutex s_ThreadIdentifierLock;
		static std::unordered_map<RunnableHandle, RunnableHandle> s_ThreadIdentifier;

	private:

		//Lock free
		static bool s_Shutdown;

		//Lock + control variables
		static std::mutex s_ControlLock;
		static std::stack<RunnableHandle> s_IDStack;
	};
}

namespace std {

	template <>
	struct hash<GrizzlyBear::RunnableHandle>
	{
		size_t operator()(const GrizzlyBear::RunnableHandle& handle) const
		{
			return handle.operator int();
		}
	};

}
