#pragma once

class SubPlayerCenter : public PlayerCenter
{
public:
	SubPlayerCenter(SubScene* pSubScene);
	~SubPlayerCenter();

private:
	// 初始化回调函数
	void CallBackFunInit();

};