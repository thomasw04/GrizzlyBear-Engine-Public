#include <gbpch.h>

#include "Thread.h"

namespace GrizzlyBear {

	//-------------------------------------------------------------------------------------
	// * Name: Thread section
	// * Purpose: Queue threads which will be efficiently executed on the available processors.
	// * Version: 1.0
	// * Author: Thomas Wachter
	//-------------------------------------------------------------------------------------

	//Force template
	template class ThreadState<int>;

	//Task state + promise type------------------------------------------------------------------------

	template<std::movable T>
	ThreadState<T> ThreadState<T>::promise_type::get_return_object()
	{
		return ThreadState{ Handle::from_promise(*this) };
	}

	template<std::movable T>
	std::suspend_always ThreadState<T>::promise_type::initial_suspend() noexcept
	{
		return {};
	}

	template<std::movable T>
	std::suspend_always ThreadState<T>::promise_type::final_suspend() noexcept
	{
		return {};
	}

	template<std::movable T>
	std::suspend_always ThreadState<T>::promise_type::yield_value(T value) noexcept
	{
		m_CurrentValue = std::move(value);
		return {};
	}

	template<std::movable T>
	void ThreadState<T>::promise_type::return_value(T value) noexcept
	{
		m_CurrentValue = std::move(value);
	}

	template<std::movable T>
	void ThreadState<T>::promise_type::unhandled_exception()
	{
		throw;
	}

	template<std::movable T>
	ThreadState<T>::ThreadState(const Handle handle)
		: m_Task(handle)
	{
	}

	template<std::movable T>
	ThreadState<T>::~ThreadState()
	{
		if (m_Task) m_Task.destroy();
	}

	template<std::movable T>
	void ThreadState<T>::resume()
	{
		m_Task.resume();
	}

	template<std::movable T>
	void ThreadState<T>::reset() const
	{
		m_Task = nullptr;
	}

	template<std::movable T>
	const T ThreadState<T>::get_value()
	{
		return m_Task.promise().m_CurrentValue.value();
	}

	template<std::movable T>
	bool ThreadState<T>::done()
	{
		return m_Task.done();
	}

	//Thread Handle---------------------------------------------------------------------

	RunnableHandle::RunnableHandle(int id)
		: m_ID(id)
	{
	}

	RunnableHandle::operator int() const
	{
		return m_ID;
	}

	RunnableHandle RunnableHandle::null()
	{
		return RunnableHandle();
	}

	//----------------------------------------------------------------------------------

	//Thread actions--------------------------------------------------------------------

	/*
	 * \brief Called from an running coroutine in order to spawn child threads.
	 * \param func Function which will executed by the child thread.
	 * \return Returns the id of the newly created child thread.
	*/
	int ThreadActions::spawn_child(ThreadFunc childFunc)
	{
		ThreadManager::s_ThreadIdentifierLock.lock();

		int processor = ThreadManager::s_ThreadIdentifier[m_ID];

		Processor& p = ThreadManager::s_Processors[processor];

		ThreadManager::s_ThreadIdentifierLock.unlock();

		int id = 0;

		p.add_thread(CreateRef<Thread>(ThreadActions(id = ThreadManager::generate_identifier()), childFunc), true);

		ThreadManager::update_assigned_processor(id, processor);

		ThreadManager::wake_up_all();

		return id;
	}

	bool ThreadActions::is_ready(RunnableHandle taskHandle)
	{
		Processor& p1 = ThreadManager::s_Processors[ThreadManager::s_ThreadIdentifier[m_ID]];

		{
			std::scoped_lock<std::mutex> lock(p1.m_ProcessorLock);
			
			if (!p1.m_Threads.empty() && p1.m_Threads.back()->get_identifier() == taskHandle)
			{
				return false;
			}
		}

		for (Processor& p : ThreadManager::s_Processors)
		{
			{
				std::scoped_lock<std::mutex> lock(p.m_ControlLock);

				if (p.m_CurrentThreadID == taskHandle)
				{
					return false;
				}
			}
		}

		return true;
	}

	/*
	 * \brief Returns the id of the current thread.
	 * \return Id of the current thread.
	*/
	RunnableHandle ThreadActions::get_identifier() const
	{
		return m_ID;
	}

	//----------------------------------------------------------------------------------

	//Thread functions------------------------------------------------------------------

	Thread::Thread(ThreadActions actions, ThreadState<int> state)
		: m_Actions(actions), m_Coroutine(state)
	{
	}

	Thread::Thread(ThreadActions actions, ThreadFunc func)
		: m_Actions(actions), m_Coroutine(func(m_Actions))
	{

	}

	Thread::~Thread()
	{
		ThreadManager::free_identifier(get_identifier());
	}

	/*
	 * \brief Returns the id of the current thread.
	 * \return Id of the current thread.
	*/
	RunnableHandle Thread::get_identifier() const
	{
		return m_Actions.get_identifier();
	}

	/*
	* \brief Notifies all waiting threads that the task is done.
	*/
	void Thread::finish()
	{
		{
			std::unique_lock<std::mutex> lock(m_WaiterLock);
			m_Ready = true;
			m_Waiter.notify_all();
		}
	}

	/*
	* \brief Waits until the current thread finished execution. e.g. The finish method was executed.
	*/
	void Thread::wait()
	{
		{
			std::unique_lock<std::mutex> lock(m_WaiterLock);

			if (m_Ready) return;

			m_Waiter.wait(lock, [this] {return m_Ready; });
		}
	}

	//----------------------------------------------------------------------------------

	//Processor functions

	Processor::Processor()
	{
	}

	/*
	* \brief Internal function, to start the thread of an processor.
	*/
	void Processor::start()
	{
		m_Processor = boost::thread(&Processor::run, this);
	}

	/*
		TaskState<int> coroutine()
		{
			handle shader = spawn(shaderThread);

			//Load vertices

			while(!shaderLoaded)
			{
				yield_value(shader);
			}

			//Finish
		}
	*/

	/*
	* \brief Main loop of an processor, handles thread-stealing, thread-stalling and thread-executing.
	*/
	void Processor::run()
	{
		while (!ThreadManager::s_Shutdown)
		{
			m_ProcessorLock.lock(); 
			bool steal = m_Threads.empty();
			m_ProcessorLock.unlock(); 

			if (steal)
			{
				//Try to steal work
				if (!try_steal())
				{
					//If it doesnt work the processor waits for a new task request for the user
					sleep();

					//Shutdown
					if (ThreadManager::s_Shutdown) break;
				}
			}

			m_ProcessorLock.lock(); 

			if (m_Threads.empty())
			{
				m_ProcessorLock.unlock();
				continue;
			}
			
			Ref<Thread> thread = m_Threads.back();
			m_Threads.pop_back();

			m_ProcessorLock.unlock(); 

			{
				std::scoped_lock<std::mutex> lock(m_ControlLock);
				
				m_CurrentThreadID = thread->get_identifier();
			}

			thread->m_Coroutine.resume();

			int returnCode = thread->m_Coroutine.get_value();

			{
				std::scoped_lock<std::mutex> lock(ThreadManager::s_StallQueueLock);

				//Add the thread to the stalled queue if it was stalled
				if (!thread->m_Coroutine.done())
				{
					bool okay = false;

					{
						std::scoped_lock<std::mutex> lock(m_ProcessorLock);
						
						if (!m_Threads.empty() && m_Threads.back()->get_identifier() == returnCode)
						{
							okay = true;
						}
					}

					if (!okay)
					{
						for (Processor& p : ThreadManager::s_Processors)
						{
							{
								std::scoped_lock<std::mutex> lock(p.m_ControlLock);

								if (p.m_CurrentThreadID == returnCode)
								{
									okay = true;
									break;
								}
							}
						}
					}	

					if (okay)
					{
						ThreadManager::s_Stalled[returnCode].push(thread);
					}
					else
					{
						ThreadManager::queue_thread(thread, false, true);
					}
				}
				else
				{
					thread->finish();

					int stallCount = (int)ThreadManager::s_Stalled[thread->get_identifier()].size();

					while (stallCount > 0)
					{
						Ref<Thread> t = ThreadManager::s_Stalled[thread->get_identifier()].front();
						ThreadManager::s_Stalled[thread->get_identifier()].pop();

						stallCount--;

						ThreadManager::queue_thread(t, false, true);
					}
				}

				{
					std::scoped_lock<std::mutex> lock(m_ControlLock);

					m_CurrentThreadID = RunnableHandle::null();
				}
			}
		}
	}

	/*
	* \brief Adds a new thread to the deque of the processor.
	* \param thread The thread which should be added.
	* \param immediately If true the thread will be added to the back of the deque, if false it will be added to the front.
	*/
	void Processor::add_thread(Ref<Thread> thread, bool immediately)
	{
		//Lock our resource
		{
			std::scoped_lock<std::mutex> lock(m_ProcessorLock);

			if (immediately)
				m_Threads.push_back(thread);
			else
				m_Threads.push_front(thread);
		}
	}

	/*
	* \brief Sets the processor 31 years to sleep, if it is not asleep already.
	*/
	void Processor::sleep()
	{
		m_ControlLock.lock();

		if (!m_Sleeps)
		{
			//31 years...
			m_Sleeps = true;
			m_ControlLock.unlock();

			try {
				boost::this_thread::sleep_for(boost::chrono::seconds(1000000000));
			}
			catch (boost::thread_interrupted& e) { (void)e; }
		}
		else
		{
			m_ControlLock.unlock();
		}
	}

	/*
	* \brief Wakes the processor up, if it is sleeping.
	*/
	void Processor::wake_up()
	{
		m_ControlLock.lock();

		if (m_Sleeps)
		{
			try {
				m_Processor.interrupt();
			} catch(boost::thread_interrupted& e) { (void)e; }
			
			m_Sleeps = false;
		}

		m_ControlLock.unlock();
	}

	void Processor::panic()
	{
		m_ControlLock.lock();

		#ifdef GRIZZLY_PLATFORM_WINDOWS
			TerminateThread(m_Processor.native_handle(), 0);
		#else
			pthread_cancel(m_Processor.native_handle());
		#endif

		m_ControlLock.unlock();
	}
	/*
	* \brief Invoked when the deque of the processor is empty, this function tries to steal (only if existant) a thread from the top of the deque of an other processor.
	* \return Whether the steal was successful.
	*/
	bool Processor::try_steal()
	{
		//Steal work
		for (int i = 0; i < ThreadManager::s_Processors.size(); i++)
		{
			Processor& p = ThreadManager::s_Processors[i];

			if (&p == this) continue;

			{
				std::scoped_lock<std::mutex> lock(p.m_ProcessorLock);

				if (p.m_Threads.size() != 0)
				{
					m_Threads.push_back(p.m_Threads.front());

					ThreadManager::update_assigned_processor(p.m_Threads.front()->get_identifier(), m_OwnID);

					p.m_Threads.pop_front();

					return true;
				}
			}
		}

		return false;
	}

	//-----------------------------------------------------------------------------------------------------

	//ThreadManager variables

	std::vector<Processor>										ThreadManager::s_Processors;           //Queue with processors, based on cpu core count.
	std::mutex													ThreadManager::s_StallQueueLock;       //Mutex for the stall queue.
	std::unordered_map<RunnableHandle, std::queue<Ref<Thread>>>	ThreadManager::s_Stalled;              //Collection of stalled threads - int dependency, queue dependant threads.
	std::mutex													ThreadManager::s_ThreadIdentifierLock; //Mutex for the thread identifier.
	std::unordered_map<RunnableHandle, RunnableHandle>			ThreadManager::s_ThreadIdentifier;     //Pairs the thread id with the index of the processor that is currently working on it.
	bool														ThreadManager::s_Shutdown = false;     //Shutdown yes : no.
	std::mutex													ThreadManager::s_ControlLock;          //Lock for control variables.
	std::stack<RunnableHandle>									ThreadManager::s_IDStack;              //Counter to generate ids for the threads.
	

	//ThreadManager functions------------------------------------------------------------------------------

	/*
	* \brief Initializes the ThreadManager (only allowed once).
	*/
	ModuleInfo ThreadManager::init()
	{
		GRIZZLY_CORE_LOG(LogLevel::Trace, "Starting to initialize ThreadManager module...");

		try
		{
			//TODO Not accurate in all cases
			int processors = boost::thread::hardware_concurrency();
			if (processors == 0) processors = 1;

			s_Processors = std::vector<Processor>(processors);

			//Hardcoded thread id limit (e.g. Threads that can be registered at the same time)
			for (int i = 1; i < 100000; i++)
			{
				s_IDStack.push(i);
			}

			for (int i = 0; i < processors; i++)
			{
				s_Processors[i].m_OwnID = i;
			}

			for (int i = 0; i < processors; i++)
			{
				s_Processors[i].start();
			}

			return { ModuleState::GOOD, ""};
		}
		catch (std::exception& e)
		{
			return { ModuleState::ERROR_RISKY, e.what()};
		}
	}

	/*
	* \brief Shuts the ThreadManager down, all currently executing tasks will be finished first.
	*/
	void ThreadManager::shutdown()
	{
		s_Shutdown = true;

		for (int i = 0; i < s_Processors.size(); i++)
		{
			s_Processors[i].wake_up();
			s_Processors[i].m_Processor.join();
		}
	}

	void ThreadManager::panic()
	{
		s_Shutdown = true;

		for (int i = 0; i < s_Processors.size(); i++)
		{
			s_Processors[i].panic();
		}
	}

	/*
	* \brief Queues a task onto the deque of the processor with the least work to do. If two processors have the same amount of work,
	* the processor with the lesser index will be favored.
	* \param func The function which will be executed by the processor.
	* \param immediately If true the thread will be added to the back of the deque, if false it will be added to the front.
	* \return Returns the id of the newly created thread.
	*/
	RunnableHandle ThreadManager::queue_thread(ThreadFunc func, bool wait, bool immediately)
	{
		RunnableHandle id = 0;

		//Create default Thread, but with unique id!
		queue_thread(CreateRef<Thread>(ThreadActions(id = ThreadManager::generate_identifier()), func), wait, immediately);

		return id;
	}

	/*
	* \brief Queues a task onto the deque of the processor with the least work to do. If two processors have the same amount of work,
	* the processor with the lesser index will be favored.
	* \param thread The thread which will be executed by the processor.
	* \param immediately If true the thread will be added to the back of the deque, if false it will be added to the front.
	* \return Returns the id of the newly created thread.
	*/
	RunnableHandle ThreadManager::queue_thread(Ref<Thread> thread, bool wait, bool immediately)
	{
		int min = 0;
		int sizeMin = -1;

		//Get the processor with the least work to do to achieve faster execution
		for (int i = 0; i < s_Processors.size(); i++)
		{
			Processor& proc = s_Processors[i];

			{
				std::scoped_lock<std::mutex> lock(proc.m_ProcessorLock);

				if (proc.m_Threads.size() < sizeMin || sizeMin == -1)
				{
					min = i;
					sizeMin = (int)proc.m_Threads.size();

					if (sizeMin == 0)
					{
						break;
					}
				}
			}
		}

		update_assigned_processor(thread->get_identifier(), min);

		//Add a thread to the processor list
		s_Processors[min].add_thread(thread, immediately);
		
		//And wake it up
		s_Processors[min].wake_up();

		if (wait)
		{
			thread->wait();
		}
			
		return thread->get_identifier();
	}

	/*
	* \brief Wakes up all processors.
	*/
	void ThreadManager::wake_up_all()
	{
		for (int i = 0; i < s_Processors.size(); i++)
		{
			s_Processors[i].wake_up();
		}
	}

	/*
	* \brief Generates an id for a thread.
	* \return Returns the generated id.
	*/
	RunnableHandle ThreadManager::generate_identifier()
	{
		s_ControlLock.lock();

		RunnableHandle returnID = s_IDStack.top();
		s_IDStack.pop();

		s_ControlLock.unlock();

		return returnID;
	}

	/*
	* \brief Returns a previously used id back to the id stack
	*/
	void ThreadManager::free_identifier(RunnableHandle handle)
	{
		{
			std::scoped_lock<std::mutex> lock(s_ControlLock);

			s_IDStack.push(handle);
		}
	}

	/*
	* \brief Internal function, sets which processor currently executes a specific thread.
	* \param identifier The id of the thread.
	* \param newProcessor The index of the processor in the vector.
	*/
	void ThreadManager::update_assigned_processor(RunnableHandle identifier, int newProcessor)
	{
		{
			std::scoped_lock<std::mutex> lock(s_ThreadIdentifierLock);
			
			s_ThreadIdentifier[identifier] = newProcessor;
		}
	}

	//----------------------------------------------------------------------------------


	//-------------------------------------------------------------------------------------
	// * Name: Repeating and delayed thread section
	// * Purpose: Queue threads which will be executed after a specific duration or repeated.
	// * Version: 1.0
	// * Author: Thomas Wachter
	//-------------------------------------------------------------------------------------

	Ref<Context> ThreadManager::create_context()
	{
		return std::make_shared<Context>();
	}

	Context::Context()
		: m_Running(true)
	{
		m_Processor = boost::thread(&Context::run, this);
	}

	Context::~Context()
	{
		m_Running = false;
		m_Processor.join();
	}
	
	RunnableHandle Context::register_task(TimeUnit delay, TimeUnit interval, TaskFunc func)
	{
		if (delay == 0) delay = 1;

		int id;

		{
			std::scoped_lock<std::mutex> lock(m_ControlLock);

			id = m_IdCounter++;
		}

		{
			std::scoped_lock<std::mutex> lock(m_ContextLock);

			m_Tasks.push_back({ func, delay, interval, TimingFunctions::now(), id});
		}

		return id;
	}

	bool Context::suspend_task(RunnableHandle task)
	{
		{
			std::scoped_lock<std::mutex> lock(m_ContextLock);

			for (auto it = m_Tasks.begin(); it < m_Tasks.end(); it++)
			{
				if (it->m_Handle == task)
				{
					m_Tasks.erase(it);
					return true;
				}
			}
		}

		return false;
	}

	void Context::run()
	{
		while (m_Running)
		{
			bool del = false;

			{
				std::scoped_lock<std::mutex> lock(m_ContextLock);

				for (auto it = m_Tasks.begin(); it != m_Tasks.end(); it++)
				{
					if (del)
					{
						it--;
						del = false;
					}

					if (it->m_Delay != 0)
					{
						if (TimingFunctions::can_run(it->m_LastInterrupt, it->m_Delay))
						{
							it->m_Func();

							if (it->m_Interval == 0)
							{
								it = m_Tasks.erase(it);
								del = true;

								if (m_Tasks.empty()) break;

								continue;
							}
							else
							{
								it->m_Delay = 0;
								it->m_LastInterrupt = TimingFunctions::now();
								continue;
							}
						}
					}
					else
					{
						if (TimingFunctions::can_run(it->m_LastInterrupt, it->m_Interval))
						{
							it->m_Func();

							it->m_LastInterrupt = TimingFunctions::now();
						}
					}
				}
			}
		}
	}
}