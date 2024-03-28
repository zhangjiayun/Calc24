#include "TCPServer.h"

#include <functional>
#include <iostream>


bool TCPServer::init(int32_t threadNum, const std::string& ip, uint16_t port) {
	m_threadPool.start(threadNum);

    //TODO: ��ip��port��һЩ��Ҫ�Ĳ���У��
    m_ip = ip;
    m_port = port;

    m_baseEventLoop.init();

    m_pAcceptor = new Acceptor(&m_baseEventLoop);

    if (!m_pAcceptor->startListen(ip, port)) {
        return false;
    }

    //TODO: AcceptCallbackֻ����һ�����������ﴫ������������������ bind��������
    m_pAcceptor->setAcceptCallback(std::bind(&TCPServer::onAccept, this, std::placeholders::_1));

    return true;
}

void TCPServer::uninit() {
	m_threadPool.stop();  
}

void TCPServer::start() {
    m_baseEventLoop.run();
}

void TCPServer::onAccept(int clientfd) {
    std::shared_ptr<EventLoop> spEventLoop = m_threadPool.getNextEventLoop();
    auto spTCPConnection = std::make_shared<TCPConnection>(clientfd, spEventLoop);
    spTCPConnection->startRead();

    m_connectedCallback(spTCPConnection);

    //m_connections[clientfd] = std::move(spTCPConnection);
}
