#pragma once

//TODO: ͷ�ļ�������ʽ���޸� TCPConnection.h�������ƵĿ��ļ�
#include "TCPConnection.h"

#include "Calc24Protocol.h"

class Calc24Session final {
public:
	Calc24Session(std::shared_ptr<TCPConnection>&& spConn);
	~Calc24Session() = default;

	void onRead(ByteBuffer& recvBuf);
	void onWrite();

	void onClose();

	//ҵ�����
	void sendWelcomeMsg();

private:
	bool decodePackage(ByteBuffer& recvBuf, const MsgHeader& msgHeader);


private:
	std::shared_ptr<TCPConnection>			m_spConn;
};
