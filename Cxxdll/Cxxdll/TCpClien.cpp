#include "pch.h"

TCPClient::TCPClient()
{

}

TCPClient::~TCPClient()
{

}

TCPClient* TCPClient::Instance()
{
	static TCPClient g_mgr;
	return &g_mgr;
}

int TCPClient::Add(int n1, int n2)
{
	return n1 + n2;
}

int TCPClient::Sub(int n1, int n2)
{
	return n1 - n2;
}