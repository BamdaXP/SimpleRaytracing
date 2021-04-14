#include "ThreadPool.hpp"

//static init
std::atomic<uint32_t> ThreadPool::s_ThreadCount = 0;
std::atomic<bool> ThreadPool::s_Running = false;
std::vector<std::thread> ThreadPool::s_ThreadPool;
std::condition_variable ThreadPool::s_ConditionVariable;

std::queue<Task> ThreadPool::s_TaskQueue;
std::mutex ThreadPool::s_TaskQueueMutex;


void ThreadPool::Start()
{
	if (s_Running.load()) return;

	s_ThreadCount = std::thread::hardware_concurrency();
	s_ThreadPool.reserve(s_ThreadCount);
	for (int i = 0; i < s_ThreadCount; i++)
	{
		s_ThreadPool.emplace_back(&ThreadFunc);
	}
	s_Running.store(true);
}

void ThreadPool::Stop()
{
	s_Running.store(false);

	s_ConditionVariable.notify_all();
	for (auto& thread : s_ThreadPool)
	{
		if (thread.joinable())
			thread.join();
	}
	
	//Pop all the thread, thought it won't stop them but we don't need to maintain the here
	while (!s_ThreadPool.empty()) {
		s_ThreadPool.pop_back();
	}

}

void ThreadPool::SubmitTask(const Task& task)
{
	std::unique_lock<std::mutex> lock(s_TaskQueueMutex);
	s_TaskQueue.push(task);
	lock.unlock();
	s_ConditionVariable.notify_one();
}

void ThreadPool::WaitUntilNoTask()
{
	while(!IsNoTask()){}
}


void ThreadPool::ThreadFunc()
{
	Task task;
	while (s_Running.load()) {
		std::unique_lock<std::mutex> lock(s_TaskQueueMutex);
		//Block here and wait for a notify signal
		s_ConditionVariable.wait(lock, []() {return !s_TaskQueue.empty() || !s_Running.load(); });//Avoid fake awake
		if (s_TaskQueue.empty() && !s_Running.load())
			return;
		task = std::move(s_TaskQueue.front()); // È¡Ò»¸ö task
		s_TaskQueue.pop();
		lock.unlock();
		//Execute the task
		task();
	}
}


