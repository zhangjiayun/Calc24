/*
* zhangjiayun 2024.03.18
* IO复用函数包装类的接口类
*/

#pragma once

#include <vector>

#include "IEventDispatcher.h"

#define EVENT_FLAG_READ			0x0001
#define EVENT_FLAG_WRITE		0x0010

class IIOMultiplex {
public:
	virtual void poll(int timeoutUs, std::vector<IEventDispatcher*>& triggeredEventDispatchers) = 0;

	virtual void registerReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) = 0;
	virtual void registerWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) = 0;

	virtual void unregisterReadEvent(int fd, bool readEvent, IEventDispatcher* eventDispatcher) = 0;
	virtual void unregisterWriteEvent(int fd, bool writeEvent, IEventDispatcher* eventDispatcher) = 0;
	virtual void unregisterAllEvents(int fd, IEventDispatcher* eventDispatcher) = 0;
};
