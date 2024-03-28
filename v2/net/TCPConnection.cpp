#include "TCPConnection.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

TCPConnection::TCPConnection(int clientfd, const std::shared_ptr<EventLoop>& spEventLoop)
	: m_fd(clientfd), m_spEventLoop(spEventLoop) {

}

TCPConnection::~TCPConnection() {
	::close(m_fd);
}

bool TCPConnection::startRead() {
	//TODO: registerReadEventӦ����bool�����޸�
	m_spEventLoop->registerReadEvent(m_fd, true, this);
	return true;
}

bool TCPConnection::send(const char* buf, int bufLen){
	m_sendBuf.append(buf, bufLen);

	while (true) {
		int n = ::send(m_fd, m_sendBuf, m_sendBuf.remaining(), 0);
		if (n == 0) {
			//�Զ˹ر�������
			//TODO: ����Ҳ�ر����� 
			onClose();
			return false;
		}
		else if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			else if (errno == EWOULDBLOCK || errno == EAGAIN) {
				//��ǰ����TCP����̫С�����ݷ�����ȥ��
				//ע��д�¼�
				m_writecallback();

				registerWriteEvent();
				return true;
			}

			//������������ˣ��ر�����
			onClose();
			return false;
		}

		//���ͳɹ���
		m_sendBuf.erase(n);
		if (m_sendBuf.isEmpty()) {
			return true;
		}
	}

	//return false;
}

bool TCPConnection::send(const std::string& buf){
	return send(buf.c_str(), buf.length());
}

void TCPConnection::onRead(){
	if (!m_enableRead)
		return;

	//������
	char buf[1024];
	int n = ::recv(m_fd, buf, sizeof(buf), 0);
	if (n == 0) {
		//�Զ˹ر�������
		//����Ҳ�ر�����
		onClose();
	}else if (n < 0) {
		if (errno == EINTR /*|| errno == EWOULDBLOCK || errno == EAGAIN*/) {
			return;
		}

		//������������ˣ��ر�����
		onClose();
		return;
	}

	//�����յ����ݰ�
	m_recvBuf.append(buf, n);

	//���
	//m_readcallback=>Calc24Session::onRead
	m_readcallback(m_recvBuf);
}

void TCPConnection::onWrite(){
	if (!m_enableWrite)
		return;

	while (true) {
		int n = ::send(m_fd, m_sendBuf, m_sendBuf.remaining(), 0);
		if (n == 0) {
			//�Զ˹ر�������
			//����Ҳ�ر�����
			onClose();
			return;
		}
		else if (n < 0) {
			if (errno == EINTR) {
				continue;
			}
			else if (errno == EWOULDBLOCK || errno == EAGAIN) {
				//��ǰ����TCP����̫С�����ݷ�����ȥ��
				//ע��д�¼�
				m_writecallback();

				registerWriteEvent();

				return;
			}

			//������������ˣ��ر�����
			onClose();
			return;
		}

		//���ͳɹ���
		m_sendBuf.erase(n);
		if (m_sendBuf.isEmpty()) {
			unregisterWriteEvent();
			return;
		}
	}
}

void TCPConnection::onClose() {
	m_closecallback(); //���û��ص�

	unregisterAllEvents();

}

void TCPConnection::enableReadWrite(bool read, bool write) {
	m_enableRead = read;
	m_enableWrite = write;
}

void TCPConnection::registerWriteEvent() {
	if (m_registerWriteEvent)
		return;

	//��IO���ú���ע��д�¼�
	m_spEventLoop->registerWriteEvent(m_fd, true, this);
	m_registerWriteEvent = true;
}

void TCPConnection::unregisterWriteEvent() {
	if (!m_registerWriteEvent)
		return;

	//��IO���ú�����ע��д�¼�
	m_spEventLoop->registerWriteEvent(m_fd, false, this);

	m_registerWriteEvent = false;
}

void TCPConnection::unregisterAllEvents() {
	//��IO���ú�����ע�����ж�д�¼�
	m_spEventLoop->unregisterAllEvents(m_fd, this);

	m_registerWriteEvent = false;
}
