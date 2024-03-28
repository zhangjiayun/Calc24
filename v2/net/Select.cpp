#include "Select.h"

#include <sys/select.h>

Select::Select() {
}

Select::~Select() {
}

void Select::poll(int timeoutUs, std::vector<IEventDispatcher*>& triggeredEventDispatchers) {
	fd_set readfds, writefds;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	int tmpfd;
	for (const auto& iter : m_fdEvents) {
		tmpfd = iter.first;

		if (iter.second & EVENT_FLAG_READ)
			FD_SET(tmpfd, &readfds);

		if (iter.second & EVENT_FLAG_WRITE)
			FD_SET(tmpfd, &writefds);
	}

	struct timeval tv;
	tv.tv_sec = timeoutUs / 1000000;
	tv.tv_usec = timeoutUs - (timeoutUs / 1000000) * 1000000;

	int n = ::select(m_maxfd + 1, &readfds, &writefds, nullptr/*&m_exceptfds*/, &tv);
	if (n <= 0) {
		//超时，没有任何事件响应
		return;
	} 

	int hasEventCount = 0;
	bool enableRead = false;
	bool enableWrite = false;
	for (const auto& iter : m_fdEvents) {

		int tempfd = iter.first;
		if (FD_ISSET(tempfd, &readfds)) {
			enableRead = true;
		}
		else {
			enableRead = false;
		}

		if (FD_ISSET(tempfd, &writefds)) {
			enableWrite = true;
		}
		else {
			enableWrite = false;
		}
		IEventDispatcher* eventDispatcher = m_eventDispatchers[tempfd];
		eventDispatcher->enableReadWrite(enableRead, enableWrite);
		triggeredEventDispatchers.push_back(eventDispatcher);
		if (enableRead || enableWrite) {
			hasEventCount++;
			if (hasEventCount >= n)
				break;
		}
	}
}

void Select::registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		eventFlags |= EVENT_FLAG_READ;
	}
	else {
		eventFlags = iter->second;
		if (!(eventFlags & EVENT_FLAG_READ)) {
			eventFlags |= EVENT_FLAG_READ;;
		}
	}

	m_fdEvents[fd] = eventFlags;

	if (m_maxfd < fd) {
		m_maxfd = fd;
	}

	m_eventDispatchers[fd] = eventDispatcher;
}

void Select::registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		eventFlags |= EVENT_FLAG_WRITE;
	}
	else {
		eventFlags = iter->second;
		if (!(eventFlags & EVENT_FLAG_WRITE)) {
			eventFlags |= EVENT_FLAG_WRITE;
		}
	}

	m_fdEvents[fd] = eventFlags;

	if (m_maxfd < fd) {
		m_maxfd = fd;
	}

	m_eventDispatchers[fd] = eventDispatcher;
}

void Select::unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		return;
	}
	else {
		eventFlags = iter->second;
		if (!(eventFlags & EVENT_FLAG_READ)) {
			return;
		}

		eventFlags &= ~EVENT_FLAG_READ;

		if (eventFlags == 0) {
			m_fdEvents.erase(iter);
			m_eventDispatchers.erase(fd);
			if (m_maxfd == fd) {
				if (!m_fdEvents.empty())
					m_maxfd = m_fdEvents.rbegin()->first;
				else
					m_maxfd = -1;
			}
		}
		else {
			m_fdEvents[fd] = eventFlags;
			m_eventDispatchers[fd] = eventDispatcher;
		}
	}
}

void Select::unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		return;
	}
	else {
		eventFlags = iter->second;
		if (!(eventFlags & EVENT_FLAG_WRITE)) {
			return;
		}

		eventFlags &= ~EVENT_FLAG_WRITE;

		if (eventFlags == 0) {
			m_fdEvents.erase(iter);
			m_eventDispatchers.erase(fd);
			if (m_maxfd == fd) {
				if (!m_fdEvents.empty())
					m_maxfd = m_fdEvents.rbegin()->first;
				else
					m_maxfd = -1;
			}
		}
		else {
			m_fdEvents[fd] = eventFlags;
			m_eventDispatchers[fd] = eventDispatcher;
		}
	}
}

void Select::unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		return;
	}
	else {
		m_fdEvents.erase(iter);
		m_eventDispatchers.erase(fd);
		if (m_maxfd == fd) {
			if (!m_fdEvents.empty())
				m_maxfd = m_fdEvents.rbegin()->first;
			else
				m_maxfd = -1;
		}
	}
}
