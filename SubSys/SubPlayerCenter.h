#pragma once

class SubPlayerCenter : public PlayerCenter
{
public:
	~SubPlayerCenter();
	SubPlayerCenter(SubScene* pSubScene);

private:
	// ��ʼ���ص�����
	void CallBackFunInit();

};