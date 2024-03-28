#include "ThreadPool.h"


//加上static表示MAX_THREAD_NUM只对当前文件可见，不加的话会对所有模块可见
static constexpr int32_t MAX_THREAD_NUM = 20;
static constexpr int32_t DEFAULT_THREAD_NUM = 5;


void ThreadPool::start(int32_t threadNum /*= 1*/) {
	if (threadNum <= 0 || threadNum > MAX_THREAD_NUM)
		threadNum = DEFAULT_THREAD_NUM;

	for (int32_t i = 0; i < threadNum; ++i) {
		auto spEventLoop = std::make_shared<EventLoop>();
		spEventLoop->init();
		m_eventLoops.push_back(std::move(spEventLoop));
	}

	for (int32_t i = 0; i < threadNum; ++i) {
		//pthread_create
		auto spThread = std::make_shared<std::thread>(std::bind(&ThreadPool::threadFunc, this, i));
		m_threads.push_back(std::move(spThread));
	}
}

void ThreadPool::stop() {
	m_stop = true;

	int32_t threadNum = m_threads.size();
	for (int32_t i = 0; i < threadNum; ++i) {
		m_threads[i]->join();
	}
}

std::shared_ptr<EventLoop> ThreadPool::getNextEventLoop() {
	auto spEventLoop = m_eventLoops[m_lastEventLoopNo];

	++m_lastEventLoopNo;
	if (m_lastEventLoopNo >= m_eventLoops.size()) {
		m_lastEventLoopNo = 0;
	}

	return spEventLoop;
}

void ThreadPool::threadFunc(size_t eventLoopIndex) {
	while (!m_stop) {
		//实际的线程工作
		m_eventLoops[eventLoopIndex]->run();
	}
}
