/**
* �̳߳���
*  zhangjiayun 2024.03.18
*/
#pragma once

//#include <stdint.h>
#include <cstdint> //C++д��
#include <functional>
#include <memory>
#include <thread>
#include <vector>

#include "EventLoop.h"

class ThreadPool final {
public:
	ThreadPool() = default;
	~ThreadPool() = default;

	void start(int32_t threadNum = 1);
	void stop();

	std::shared_ptr<EventLoop> getNextEventLoop();

private:
	void threadFunc(size_t eventLoopIndex);


private:
	ThreadPool(const ThreadPool& rhs) = delete;
	ThreadPool& operator= (const ThreadPool& rhs) = delete;

	ThreadPool(ThreadPool&& rhs) = delete;
	ThreadPool& operator= (ThreadPool&& rhs) = delete;


private:
	bool										m_stop{false};
	size_t										m_lastEventLoopNo{ 0 };
	std::vector<std::shared_ptr<std::thread> >	m_threads;
	std::vector<std::shared_ptr<EventLoop> >	m_eventLoops;
};
