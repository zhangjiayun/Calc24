#include "EventLoop.h"

#include <vector>

#include "Select.h"
#include "Poll.h"
#include "Epoll.h"

bool EventLoop::init(IOMultiplexType type /*= IOMultiplexType::IOMultiplexTypeEpoll*/) {
	if (type == IOMultiplexType::IOMultiplexTypeSelect) {
		m_spIOMultiplex = std::make_unique<Select>();
	}
	else if (type == IOMultiplexType::IOMultiplexTypePoll) {
		m_spIOMultiplex = std::make_unique<Poll>();
	}
	else {
		m_spIOMultiplex = std::make_unique<Epoll>();
	}

	m_running = true;

	return true;
}

void EventLoop::run(){
	while (m_running) {
		//1. ���ʹ���ʱ���¼�
		//
		//2. ʹ��select/poll/epoll��IO���ú������һ��socket�Ķ�д�¼�
		//
		std::vector<IEventDispatcher*> eventDispatchers;
		m_spIOMultiplex->poll(500000, eventDispatchers);
		for (size_t i = 0; i < eventDispatchers.size(); ++i) {
			eventDispatchers[i]->onRead();
			eventDispatchers[i]->onWrite();
		}
		//3. �����д�¼�
		//for (int )
		//4. ���û���fd���ƴ����Զ����¼�
	}
}

void EventLoop::registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) {
	m_spIOMultiplex->registerReadEvent(fd, true, eventDispatcher);
}

void EventLoop::registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) {
	m_spIOMultiplex->registerWriteEvent(fd, true, eventDispatcher);
}

void EventLoop::unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) {
	m_spIOMultiplex->unregisterReadEvent(fd, true, eventDispatcher);
}

void EventLoop::unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) {
	m_spIOMultiplex->unregisterWriteEvent(fd, true, eventDispatcher);
}

void EventLoop::unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher) {
	m_spIOMultiplex->unregisterAllEvents(fd, eventDispatcher);
}

