#pragma once

class TCPClient
{
private:
	TCPClient();
	virtual ~TCPClient();

public:
	static TCPClient* Instance();

public:
	int Add(int n1, int n2);
	int Sub(int n1, int n2);
};