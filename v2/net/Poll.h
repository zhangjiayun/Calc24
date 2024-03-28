#pragma once

#include "IOMultiplex.h"

#include <sys/types.h>
#include <unistd.h>

#include <map>
#include <vector>

/*
* int poll(struct pollfd *fds, nfds_t nfds, int timeout);
*/

class Poll : public IIOMultiplex {
public:
	Poll();
	virtual ~Poll();

public:
	virtual void poll(int timeoutUs, std::vector<IEventDispatcher*>& triggeredEventDispatchers) override;

	virtual void registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) override;
	virtual void registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) override;

	virtual void unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) override;
	virtual void unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) override;
	virtual void unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher) override;

private:


private:
	//key:fd value:events
	std::map<int, int>					m_fdEvents;
	//key fd
	std::map<int, IEventDispatcher*>	m_eventDispatchers;
};
