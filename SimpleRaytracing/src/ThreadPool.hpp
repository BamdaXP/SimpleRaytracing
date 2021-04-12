#include <thread>
#include <future>
#include <atomic>
#include <functional>
#include <vector>
#include <deque>
using Task = std::function<void()>;

class ThreadPool 
{
public:
	static void SubmitTask(const Task& task);
	static void Start(uint16_t threadNumber);
	static void Stop();
private:
	static std::atomic<bool> s_Running = false;
	static std::vector<std::thread> s_ThreadPool;
	static std::deque<Task> s_TaskQueue;
};
