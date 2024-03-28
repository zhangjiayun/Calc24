#include "Poll.h"

#include <poll.h>

Poll::Poll() {
}

Poll::~Poll() {
}

void Poll::poll(int timeoutUs, std::vector<IEventDispatcher*>& triggeredEventDispatchers) {

	//����poll��������д�¼�
	std::vector<pollfd> fds;
	for (const auto& iter : m_fdEvents) {
		pollfd itemFd;
		itemFd.fd = iter.first;
		itemFd.events = iter.second;
		itemFd.revents = 0;
		fds.push_back(itemFd);
	}

	int timeMs = timeoutUs / 1000;
	int n = ::poll(&fds[0], fds.size(), timeMs);

	if (n <= 0) {
		//��ʱ��û���κ��¼���Ӧ
		return;
	}

	bool enableRead = false;
	bool enableWrite = false;
	int tempfd;
	for (size_t i = 0; i < fds.size(); ++i) {

		tempfd = fds[i].fd;
		//�¼��ɶ�
		if (fds[i].revents & POLLIN) {
			enableRead = true;
		}
		else {
			enableRead = false;
		}

		//�¼���д
		if (fds[i].revents & POLLOUT) {
			enableWrite = true;
		}
		else {
			enableWrite = false;
		}

		IEventDispatcher* eventDispatcher = m_eventDispatchers[tempfd];
		eventDispatcher->enableReadWrite(enableRead, enableWrite);
		triggeredEventDispatchers.push_back(eventDispatcher);
	}
}

void Poll::registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		eventFlags |= POLLIN;
	}
	else {
		eventFlags = iter->second;
		if (!(eventFlags & POLLIN)) {
			eventFlags |= POLLIN;;
		}
	}

	m_fdEvents[fd] = eventFlags;

	m_eventDispatchers[fd] = eventDispatcher;
}

void Poll::registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		eventFlags |= POLLOUT;
	}
	else {
		eventFlags = iter->second;
		if (!(eventFlags & POLLOUT)) {
			eventFlags |= POLLOUT;;
		}
	}

	m_fdEvents[fd] = eventFlags;

	m_eventDispatchers[fd] = eventDispatcher;
}

void Poll::unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		return;
	}
	else {
		eventFlags = iter->second;
		if (!(eventFlags & POLLIN)) {
			return;
		}

		eventFlags &= ~POLLIN;

		if (eventFlags == 0) {
			m_fdEvents.erase(iter);
			m_eventDispatchers.erase(fd);
		}
		else {
			m_fdEvents[fd] = eventFlags;
			m_eventDispatchers[fd] = eventDispatcher;
		}
	}
}

void Poll::unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		return;
	}
	else {
		eventFlags = iter->second;
		if (!(eventFlags & POLLOUT)) {
			return;
		}

		eventFlags &= ~POLLOUT;

		if (eventFlags == 0) {
			m_fdEvents.erase(iter);
			m_eventDispatchers.erase(fd);
		}
		else {
			m_fdEvents[fd] = eventFlags;
			m_eventDispatchers[fd] = eventDispatcher;
		}
	}
}

void Poll::unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher) {
	int eventFlags = 0;
	auto iter = m_fdEvents.find(fd);
	if (iter == m_fdEvents.end()) {
		return;
	}
	else {
		m_fdEvents.erase(iter);
		m_eventDispatchers.erase(fd);
	}
}
