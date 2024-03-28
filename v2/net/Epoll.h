#pragma once

#include "IOMultiplex.h"

#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <vector>

/*
* int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
*/
//
//#define EVENT_FLAG_READ			0x0001
//#define EVENT_FLAG_WRITE		0x0010		//#define EVENT_FLAG_WRITE		0x0001 << 1

class Epoll : public IIOMultiplex {
public:
	Epoll();
	virtual ~Epoll();

public:
	virtual void poll(int timeoutUs, std::vector<IEventDispatcher*>& triggeredEventDispatchers) override;

	virtual void registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) override;
	virtual void registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) override;

	virtual void unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) override;
	virtual void unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) override;
	virtual void unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher) override;

private:
	Epoll(const Epoll& rhs) = delete;
	Epoll& operator=(const Epoll& rhs) = delete;

	Epoll(Epoll&& rhs) = delete;
	Epoll&& operator=(Epoll&& rhs) = delete;

private:
	int							m_epollfd { -1 };
	//key=>fd, value=eventFlags
	std::map<int, int32_t>		m_fdEventFlags;
};
