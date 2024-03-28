/*
* 2024.03.18 zhangjiayun
* 自定义缓冲区类
*/
#pragma once

#include <string>

class ByteBuffer {
public:
	ByteBuffer() = default;
	~ByteBuffer() = default;

	ByteBuffer(const ByteBuffer& rhs) = default;
	ByteBuffer& operator=(const ByteBuffer& rhs) = default;

	ByteBuffer(ByteBuffer&& rhs) = default;
	ByteBuffer& operator=(ByteBuffer&& rhs) = default;

public:
	//TODO: ???
	operator const char* ();

	void append(const char* buf, int bufLen);
	void retrieve(std::string& outBuf, size_t bufLen = 0);
	size_t retrieve(char* buf, size_t bufLen);

	size_t peek(char* buf, size_t bufLen);

	void erase(size_t bufLen = 0);

	size_t remaining();

	void clear();

	bool isEmpty();

private:
	std::string			m_internalBuf;
};
