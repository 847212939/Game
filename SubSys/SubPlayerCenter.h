#pragma once

class SubPlayerCenter : public PlayerCenter
{
public:
	SubPlayerCenter(SubScene* pSubScene);
	~SubPlayerCenter();

private:
	// ��ʼ���ص�����
	void CallBackFunInit();

};