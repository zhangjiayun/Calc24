#pragma once

#include <functional>
#include <memory>

#include "ByteBuffer.h"
#include "IEventDispatcher.h"
#include "EventLoop.h"

//ReadCallback�������false, ��ʾ�ϲ�ҵ���߼���Ϊ�������ϣ���ر�����
using ReadCallback = std::function<void (ByteBuffer&)>;
using WriteCallback = std::function<void()>; //���û����������ص�
using CloseCallback = std::function<void()>;


class TCPConnection : public IEventDispatcher {
public:
	TCPConnection(int clientfd, const std::shared_ptr<EventLoop>& spEventLoop);
	virtual ~TCPConnection();

	bool startRead();

	//TODO: copy ctor...

	void setReadCallback(ReadCallback&& readCallback) {
		m_readcallback = std::move(readCallback);
	}

	void setWriteCallback(WriteCallback&& writeCallback) {
		m_writecallback = std::move(writeCallback);
	}

	void setCloseCallback(CloseCallback&& closeCallback) {
		m_closecallback = std::move(closeCallback);
	}

	bool send(const char* buf, int bufLen);
	bool send(const std::string& buf);

	virtual void onRead() override;
	virtual void onWrite() override;

	virtual void onClose() override;

	virtual void enableReadWrite(bool read, bool write) override;

private:
	void registerWriteEvent();
	void unregisterWriteEvent();

	void unregisterAllEvents();

private:
	int							m_fd;
	//����socketfd��ô����
	bool						m_registerWriteEvent{ false };
	bool						m_enableRead{ false };
	bool						m_enableWrite{ false };

	ByteBuffer					m_recvBuf;
	ByteBuffer					m_sendBuf;

	ReadCallback				m_readcallback;
	WriteCallback				m_writecallback;
	CloseCallback				m_closecallback;

	std::shared_ptr<EventLoop>	m_spEventLoop;

};
