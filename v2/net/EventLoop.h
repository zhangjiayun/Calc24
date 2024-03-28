/**
*  zhangjiayun 2024.03.18
*/

#pragma once

#include <memory>

#include "IEventDispatcher.h"
#include "IOMultiplex.h"

enum class IOMultiplexType {
	IOMultiplexTypeSelect,
	IOMultiplexTypePoll,
	IOMultiplexTypeEpoll
};

class EventLoop final {
public:
	EventLoop() = default;
	~EventLoop() = default;

public:
	bool init(IOMultiplexType type = IOMultiplexType::IOMultiplexTypePoll);

	void run();

	void registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher);
	void registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher);
	void unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher);
	void unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher);
	void unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher);

private:
	bool							m_running { false };
	int								m_epollfd;

	std::unique_ptr<IIOMultiplex>	m_spIOMultiplex;
};
