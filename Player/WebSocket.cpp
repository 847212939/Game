#include "WebSocket.h"
#include <WinSock2.h>
#include "Sha1Base64.h"

using namespace PLAYER;
WebSocket::WebSocketPkt::WebSocketPkt() : m_MsgType(WebSocketPkt::MsgType::MsgType_Handshake), m_Fin(0), m_OpCode(0), m_Mask(0), m_PayloadLength(0), m_Data("")
{
	memset(m_MaskingKey, 0, sizeof(m_MaskingKey));
	m_HeaderMap.clear();
}

WebSocket::WebSocketPkt::~WebSocketPkt() 
{

}

WebSocket::WebSocket()
{
	m_request.ResetFrameData();
	m_response.ResetFrameData();
}

WebSocket::~WebSocket()
{
}

void WebSocket::Tokenize(const std::string &str, std::string strseparator, std::vector<std::string> &vet)
{
	size_t index1 = 0;
	size_t index2 = 0;
	size_t size = strseparator.length();
	while (true)
	{
		index2 = str.find(strseparator, index1);

		if (index2 != std::string::npos)
		{
			std::string temp = str.substr(index1, index2 - index1);
			vet.push_back(temp);
		}
		else
		{
			std::string temp = str.substr(index1, str.length() - index1);
			vet.push_back(temp);
			break;
		}

		index1 = index2 + size;
	}
}

bool WebSocket::FromHandshakePkt(int nread, const char *buf)
{
	m_request.m_MsgType = WebSocketPkt::MsgType::MsgType_Handshake;

	std::string str(buf, nread);
	std::vector<std::string> tokens;
	Tokenize(str, "\r\n", tokens);
	if (tokens.size() <= 0)
	{
		return false;
	}

	for (size_t i = 0; i < tokens.size(); ++i)
	{
		std::vector<std::string> lineEle;
		Tokenize(tokens.at(i), ": ", lineEle);
		if (lineEle.size() == 2)
		{
			m_request.m_HeaderMap[lineEle.at(0)] = lineEle.at(1);
		}
	}

	return true;
}

int WebSocket::FromFrameDataPkt(int nread, const char *buf)
{
	m_request.m_MsgType = WebSocketPkt::MsgType::MsgType_FrameData;
	unsigned bytePos = 0;

	m_request.m_Fin = (buf[bytePos] >> 7);
	m_request.m_OpCode = buf[bytePos] & 0x0F;
	bytePos++;

	m_request.m_Mask = (buf[bytePos] >> 7);
	m_request.m_PayloadLength = buf[bytePos] & 0x7F;
	bytePos++;

	if (m_request.m_PayloadLength == 126)
	{
		uint16_t length = 0;
		memcpy(&length, buf + bytePos, 2);

		m_request.m_PayloadLength = ntohs(length);
		bytePos += 2;
	}
	else if (m_request.m_PayloadLength == 127)
	{
		long long length = 0;
		memcpy(&length, buf + bytePos, 8);

		m_request.m_PayloadLength = ntohll(length);
		bytePos += 8;
	}

	if (m_request.m_Mask != 0)
	{
		for (int i = 0; i < 4; ++i) 
		{
			m_request.m_MaskingKey[i] = buf[bytePos++];
		}
	}

	std::string s = "";
	if (m_request.m_Mask == 0)
	{
		s = std::string(buf + bytePos, (unsigned)m_request.m_PayloadLength);
	}
	else 
	{
		for (unsigned i = 0; i < m_request.m_PayloadLength; ++i)
		{
			unsigned j = i % 4U;
			char c = buf[bytePos + i] ^ m_request.m_MaskingKey[j];
			s.push_back(c);
		}
	}
	m_request.m_Data.append(s);
	bytePos += (unsigned)m_request.m_PayloadLength;

	return m_request.m_Fin;
}

std::string WebSocket::GetHandshakeString(std::string& handSharkStr)
{
	// Sec-WebSocket-Key: kMgvb6KivsYVl2EHinJHZg==
	// 对kMgvb6KivsYVl2EHinJHZg==258EAFA5-E914-47DA-95CA-C5AB0DC85B11做Sha1：
	// 得到c417b4a47e13bfbd5f68d7cfc5a30c9644abb895，将其分隔成字节数组，再对字节数据做Base64：
	SHA1 sha;
	unsigned int iDigSet[5];
	sha.Reset();
	sha << handSharkStr.c_str();
	sha.Result(iDigSet);

	for (int i = 0; i < 5; i++)
	{
		iDigSet[i] = htonl(iDigSet[i]);
	}

	return base64::base64_encode(reinterpret_cast<const unsigned char*>(iDigSet), 20);
}

std::string WebSocket::ToHandshakePkt()
{
	auto &headerMap = m_request.m_HeaderMap;
	if (headerMap.count(s_ReqWSKey) == 0) 
	{
		return "";
	}

	std::string pkt;
	pkt.append("HTTP/1.1 101 Switching Protocols\r\n");
	pkt.append("Connection: upgrade\r\n");
	pkt.append("Upgrade: websocket\r\n");
	pkt.append("Sec-WebSocket-Accept: ");

	std::string SecWSKey = headerMap.at(s_ReqWSKey) + s_Key;
	std::string&& rs = GetHandshakeString(SecWSKey);
	if (rs.empty())
	{
		pkt = "";
	}
	else
	{
		pkt.append(rs + "\r\n\r\n");
	}

	return pkt;
}

std::string WebSocket::ToFrameDataPkt(const std::string &data, WebSocket::WebSocketPkt::OpcodeType type)
{
	unsigned size = (unsigned)data.size();
	std::string frame;

	uint8_t c = (1 << 7);
	c = c | (uint8_t)type;
	frame.push_back((char)c);

	uint8_t paylength = 0;
	if (size < 126U) 
	{
		paylength = size;
		frame.push_back((char)paylength);
	}
	else if (size >= 126U && size <= 0xFFFFU) 
	{
		paylength = 126;
		frame.push_back(paylength);

		uint16_t l = htons(size);
		char buf[2] = { 0 };
		memcpy(buf, &l, 2);
		frame.append(buf);
	}
	else 
	{
		paylength = 127;
		frame.push_back(paylength);

		uint64_t l = htonll((int64_t)size);
		char buf[8] = { 0 };
		memcpy(buf, &l, 8);
		frame.append(buf);
	}

	frame.append(data);
	return frame;
}

void WebSocket::WebSocketPkt::ResetFrameData()
{
	m_Fin = 0;
	m_OpCode = 0;
	m_Mask = 0;
	memset(m_MaskingKey, 0, sizeof(m_MaskingKey));
	m_PayloadLength = 0;
	m_Data = "";
}

WebSocket::WebSocketPkt::MsgType WebSocket::RequestOpcode()
{ 
	return (WebSocket::WebSocketPkt::MsgType)m_request.m_OpCode; 
}

std::string WebSocket::RequestData()
{ 
	return m_request.m_Data; 
}

void WebSocket::ResetRequestData()
{ 
	m_request.ResetFrameData(); 
}