#include "Hagetata.h"

Hagetata *Hagetata::Create(Model *model, XMFLOAT3 position, XMFLOAT3 targetPosition)
{
    //インスタンスを生成
	Hagetata *instance = new Hagetata();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	//移動角度を設定
	instance->SetMoveAngle(targetPosition);

	return instance;
}

bool Hagetata::Initialize(Model *model, XMFLOAT3 position)
{
	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(position);
	//大きさをセット
	enemyObject->SetScale({ 6, 6, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を緑にする
	enemyObject->SetColor({ 0, 1, 0, 1 });

	return true;
}

void Hagetata::SetMoveAngle(XMFLOAT3 targetPosition)
{
	//移動角度を設定する（標的に向かって一直線）
	XMFLOAT3 position = enemyObject->GetPosition(); 
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	moveAngle = radian;
}

