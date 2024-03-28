#include "Epoll.h"

#include <string.h>
#include <sys/epoll.h>

#include "util.h"

Epoll::Epoll() {
	m_epollfd = ::epoll_create1(EPOLL_CLOEXEC);
	if (m_epollfd < 0) {
		crash();
	}
}

Epoll::~Epoll() {
}

void Epoll::poll(int timeoutUs, std::vector<IEventDispatcher*>& triggeredEventDispatchers) {
	
	//epoll_wait

	struct epoll_event events[1024];
	int timeoutMs = timeoutUs / 1000;

	bool enableRead = false;
	bool enableWrite = false;

	int n = ::epoll_wait(m_epollfd, events, 1024, timeoutMs);

	for (int i = 0; i < n; ++i) {
		if (events[i].events & EPOLLIN)
			enableRead = true;
		else
			enableRead = false;

		if (events[i].events & EPOLLOUT)
			enableWrite = true;
		else
			enableWrite = false;

		//events[0].data.ptr => IEventDispatcher*
		IEventDispatcher* eventDispatcher = static_cast<IEventDispatcher*>(events[i].data.ptr);
		eventDispatcher->enableReadWrite(enableRead, enableWrite);
		triggeredEventDispatchers.push_back(eventDispatcher);
	}
}

void Epoll::registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) {
	//epoll_ctl
	int32_t eventFlag = 0;

	auto iter = m_fdEventFlags.find(fd);
	if (iter == m_fdEventFlags.end()) {
		eventFlag |= EPOLLIN;
		m_fdEventFlags[fd] = eventFlag;
	}
	else {
		eventFlag = iter->second;
		if (eventFlag & EPOLLIN) {
			return;
		}

		eventFlag |= EPOLLIN;
		m_fdEventFlags[fd] = eventFlag;
	}

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = eventFlag;
	event.data.ptr = eventDispatcher;
	if (::epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event) < 0) {
		//TODO: 打印错误日志
		crash();
	}
}

void Epoll::registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) {
	//epoll_ctl
	int32_t eventFlag = 0;

	auto iter = m_fdEventFlags.find(fd);
	if (iter == m_fdEventFlags.end()) {
		eventFlag |= EPOLLOUT;
		m_fdEventFlags[fd] = eventFlag;
	}
	else {
		eventFlag = iter->second;
		if (eventFlag & EPOLLOUT) {
			return;
		}

		eventFlag |= EPOLLOUT;
		m_fdEventFlags[fd] = eventFlag;
	}

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = eventFlag;
	event.data.ptr = eventDispatcher;
	if (::epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &event) < 0) {
		//TODO: 打印错误日志
		crash();
	}
}

void Epoll::unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) {
	int32_t eventFlag = 0;
	int operation;

	auto iter = m_fdEventFlags.find(fd);
	if (iter == m_fdEventFlags.end()) {
		return;
	}
	else {
		eventFlag = iter->second;
		if (!(eventFlag & EPOLLIN)) {
			return;
		}

		eventFlag &= ~EPOLLIN;

		if (eventFlag == 0) {
			m_fdEventFlags.erase(iter);
			operation = EPOLL_CTL_DEL;
		}
		else {
			m_fdEventFlags[fd] = eventFlag;
			operation = EPOLL_CTL_MOD;
		}
	}

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = eventFlag;
	event.data.ptr = eventDispatcher;
	if (::epoll_ctl(m_epollfd, operation, fd, &event) < 0) {
		//TODO: 打印错误日志
		crash();
	}
}

void Epoll::unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) {
	int32_t eventFlag = 0;
	int operation;

	auto iter = m_fdEventFlags.find(fd);
	if (iter == m_fdEventFlags.end()) {
		return;
	}
	else {
		eventFlag = iter->second;
		if (!(eventFlag & EPOLLOUT)) {
			return;
		}

		eventFlag &= ~EPOLLOUT;

		if (eventFlag == 0) {
			m_fdEventFlags.erase(iter);
			operation = EPOLL_CTL_DEL;
		}
		else {
			m_fdEventFlags[fd] = eventFlag;
			operation = EPOLL_CTL_MOD;
		}
	}

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = eventFlag;
	event.data.ptr = eventDispatcher;
	if (::epoll_ctl(m_epollfd, operation, fd, &event) < 0) {
		//TODO: 打印错误日志
		crash();
	}
}

void Epoll::unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher) {
	int32_t eventFlag = 0;

	auto iter = m_fdEventFlags.find(fd);
	if (iter == m_fdEventFlags.end()) {
		return;
	}
	else {
		m_fdEventFlags.erase(iter);
	}

	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	event.events = eventFlag;
	event.data.ptr = eventDispatcher;
	if (::epoll_ctl(m_epollfd, EPOLL_CTL_DEL, fd, &event) < 0) {
		//TODO: 打印错误日志
		crash();
	}
}
