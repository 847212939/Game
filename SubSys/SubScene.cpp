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

// ��ʼ���ص�����
void SubScene::CallBackFunInit()
{

}