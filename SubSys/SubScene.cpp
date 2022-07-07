#include "../Game/stdafx.h"

SubScene::SubScene() :
	Scene()
{
	GetPlayerCenter().SetSubScene(this);
	CallBackFunInit();
}

SubScene::~SubScene()
{
}

// 初始化回调函数
void SubScene::CallBackFunInit()
{

}