#pragma once

const char s_Key[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
const char s_ReqWSKey[] = "Sec-WebSocket-Key";

class WebSocket
{
public:
	struct WebSocketPkt
	{
		enum class MsgType
		{
			MsgType_Handshake,
			MsgType_FrameData,
		};

		enum class OpcodeType
		{
			OpcodeType_Continue = 0x0,
			OpcodeType_Text = 0x1,
			OpcodeType_Binary = 0x2,
			OpcodeType_Close = 0x8,
			OpcodeType_Ping = 0x9,
			OpcodeType_Pong = 0xA,
		};

		WebSocketPkt();
		~WebSocketPkt();

		void ResetFrameData();

		MsgType                            m_MsgType;
		uint8_t                            m_Fin;            // 1bit
		uint8_t                            m_OpCode;         // 4bit
		uint8_t                            m_Mask;           // 1bit
		uint8_t                            m_MaskingKey[4]; // 0 or 4 bytes
		uint64_t                           m_PayloadLength; // 1 or 2 or 8 bytes
		std::string                        m_Data;
		std::map<std::string, std::string> m_HeaderMap;
	};

public:
	WebSocket();
	virtual ~WebSocket();

public:
	// 解析网络发送过来的包 return: -1:error, 0:continue, >0:done
	int FromFrameDataPkt(int nread, const char* buf);
	// 解析网络发送过来的握手协议
	bool FromHandshakePkt(int nread, const char* buf);
	// 发送打包成websocket握手协议的包
	std::string ToHandshakePkt();
	// 发送打包成websocket的包
	std::string ToFrameDataPkt(const std::string& data, WebSocket::WebSocketPkt::OpcodeType type = WebSocket::WebSocketPkt::OpcodeType::OpcodeType_Text);
	// 发送过来的数据放到m_request.m_Data中
	std::string RequestData();
	// 发送过来的数据类型
	WebSocket::WebSocketPkt::MsgType RequestOpcode();

private:
	// 拆分网络发送过来的协议"\r\n分隔符"
	void Tokenize(const std::string& str, std::string strseparator, std::vector<std::string>& vet);
	// 获得握手协议的包
	std::string GetHandshakeString(std::string& handSharkStr);

	void ResetRequestData();

private:
	WebSocketPkt m_request;
	WebSocketPkt m_response;
};
