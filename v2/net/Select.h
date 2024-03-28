#pragma once

#include "IOMultiplex.h"

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <map>
#include <vector>

/*
* int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
*/

class Select : public IIOMultiplex {
public:
	Select();
	virtual ~Select();

public:
	virtual void poll(int timeoutUs, std::vector<IEventDispatcher*>& triggeredEventDispatchers) override;

	virtual void registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) override;
	virtual void registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) override;

	virtual void unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) override;
	virtual void unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) override;
	virtual void unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher) override;

private:
	int									m_maxfd { -1 };
	//key:fd value:events
	std::map<int, int>					m_fdEvents;
	//key fd
	std::map<int, IEventDispatcher*>	m_eventDispatchers;
};
