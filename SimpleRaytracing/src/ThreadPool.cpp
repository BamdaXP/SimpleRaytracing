#include "ThreadPool.hpp"

void ThreadPool::SubmitTask(const Task& task)
{
}

void ThreadPool::Start(uint16_t threadNumber)
{
}

void ThreadPool::Stop()
{
	for (auto& t : s_TaskQueue)
	{
		
	}
}
