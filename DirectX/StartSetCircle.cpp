#include "StartSetCircle.h"

StartSetCircle *StartSetCircle::Create(Model *model, XMFLOAT3 position, float radius)
{
	//インスタンスを生成
	StartSetCircle *instance = new StartSetCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position, radius)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool StartSetCircle::Initialize(Model *model, XMFLOAT3 position, float radius)
{
	//円オブジェクト生成
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//固定オブジェクトの状態を保持
	this->fixedObject = fixedObject;

	//座標をセット 固定オブジェクトの中心座標と同じ
	XMFLOAT3 pos = fixedObject->GetPosition();
	circleObject->SetPosition(pos);

	//基準のサイズをセット
	baseRadius = radius;

	//変更前の円のサイズをセット
	changeRadiusStart = 0;
	//変更後の円のサイズをセット
	changeRadiusEnd = baseRadius;
	circleObject->SetScale({ changeRadiusStart, changeRadiusStart, 1 });

	//円のサイズを変更状態にする
	isChangeRadius = true;

	if (model) {
		circleObject->SetModel(model);
	}

	//色を赤にする
	circleObject->SetColor({ 1, 0.2f, 0.2f, 0.4f });

	return true;
}
