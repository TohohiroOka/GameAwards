#include "PinCircle.h"

PinCircle* PinCircle::Create(Model* model, Pin* pin)
{
	//インスタンスを生成
	PinCircle* instance = new PinCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, pin)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool PinCircle::Initialize(Model* model, Pin* pin)
{
	//円オブジェクト生成
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//敵の情報を保持
	this->pin = pin;

	//座標をセット 敵の中心座標と同じ
	XMFLOAT3 pos = pin->GetPosition();
	circleObject->SetPosition(pos);

	//基準の半径をセット(ピンの大きさ × 5）
	this->baseRadius = pin->GetScale().x * 5;

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

	//色を緑にする
	circleObject->SetColor({ 0.2f, 1, 0.2f, 0.4f });

	return true;
}
