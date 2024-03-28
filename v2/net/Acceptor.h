#pragma once

#include <cstdint>
#include <functional>
#include <string>

#include "EventLoop.h"
#include "IEventDispatcher.h"

using AcceptCallback = std::function<void(int clientfd)>;

class Acceptor final : public IEventDispatcher{
public:
	Acceptor(EventLoop* pEventLoop);
	~Acceptor();
	//...其它构造

	void setAcceptCallback(AcceptCallback&& acceptCallback) {
		m_acceptCallback = acceptCallback;
	}

public:
	virtual void onRead() override;

	bool startListen(const std::string& ip = "", uint16_t port = 8888);

private:
	virtual void onWrite() override {};

	virtual void onClose() override {};

	virtual void enableReadWrite(bool read, bool write) override {};


private:
	int				m_listenfd { -1 };

	EventLoop*		m_pEventLoop;

	AcceptCallback	m_acceptCallback;
};
