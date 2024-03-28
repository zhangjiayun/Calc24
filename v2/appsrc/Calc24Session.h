#pragma once

//TODO: 头文件包含方式待修改 TCPConnection.h属性自制的库文件
#include "TCPConnection.h"

#include "Calc24Protocol.h"

class Calc24Session final {
public:
	Calc24Session(std::shared_ptr<TCPConnection>&& spConn);
	~Calc24Session() = default;

	void onRead(ByteBuffer& recvBuf);
	void onWrite();

	void onClose();

	//业务代码
	void sendWelcomeMsg();

private:
	bool decodePackage(ByteBuffer& recvBuf, const MsgHeader& msgHeader);


private:
	std::shared_ptr<TCPConnection>			m_spConn;
};
