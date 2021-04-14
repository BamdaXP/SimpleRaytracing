#include <thread>
#include <future>
#include <atomic>
#include <functional>
#include <vector>
#include <queue>
using Task = std::function<void()>;

class ThreadPool
{
public:

	static void Start();
	static void Stop();

	static void SubmitTask(const Task& task);
	static void WaitUntilNoTask();
	static inline uint32_t GetTaskCount() { std::unique_lock<std::mutex> lock(s_TaskQueueMutex); uint32_t count = s_TaskQueue.size(); lock.unlock(); return count; }
	static inline uint32_t GetThreadCount() { return s_ThreadCount.load(); }
	static inline bool IsRunning() { return s_Running.load(); }
	static inline bool IsNoTask() { return s_TaskQueue.empty(); }
private:
	static std::atomic<uint32_t> s_ThreadCount;
	static std::atomic<bool> s_Running;
	static std::vector<std::thread> s_ThreadPool;
	static std::condition_variable s_ConditionVariable;

	static std::queue<Task> s_TaskQueue;
	static std::mutex s_TaskQueueMutex;

	static void ThreadFunc();
};
