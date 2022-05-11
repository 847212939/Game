#include "../Game/stdafx.h"

PlayerInfo::PlayerInfo() : pData(nullptr), pMsg(nullptr), pTcpSockInfo(nullptr), uSrverType(SocketType::SOCKET_TYPE_TCP)
{

}

PlayerInfo::~PlayerInfo()
{

}
