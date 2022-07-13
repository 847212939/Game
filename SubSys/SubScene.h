#pragma once

class SubScene : public Scene
{
public:
	SubScene();
	virtual~SubScene();

private:
	// 初始化回调函数
	void CallBackFunInit();
};
