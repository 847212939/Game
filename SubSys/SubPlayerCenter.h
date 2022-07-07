#pragma once

class SubPlayerCenter : public PlayerCenter
{
public:
	~SubPlayerCenter();
	SubPlayerCenter(SubScene* pSubScene);

private:
	// 初始化回调函数
	void CallBackFunInit();

};