#pragma once

class SubScene : public Scene
{
public:
	SubScene(SubPlayerPreproces* pSubPlayerPreproces);
	~SubScene();

private:
	// ��ʼ���ص�����
	void CallBackFunInit();

};
