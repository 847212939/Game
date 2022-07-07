#pragma once

class SubScene : public Scene
{
public:
	SubScene(SubPlayerPreproces* pSubPlayerPreproces);
	~SubScene();

private:
	// 初始化回调函数
	void CallBackFunInit();

};
