#include "Garuta.h"

Garuta *Garuta::Create(Model *model, XMFLOAT3 position, float moveDegree)
{
	//インスタンスを生成
	Garuta *instance = new Garuta();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	//移動角度を設定
	instance->SetMoveAngle(moveDegree);

	return instance;
}

bool Garuta::Initialize(Model *model, XMFLOAT3 position)
{
	//ザコリンオブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(position);
	//大きさをセット
	enemyObject->SetScale({ 1.5, 1.5, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	enemyObject->SetBloom(true);

	//色を赤くする
	//enemyObject->SetColor({ 1, 0, 0, 1 });

	return true;
}

void Garuta::SetMoveAngle(float moveDegree)
{
	//上向きを0にするため90度傾ける
	float degree = moveDegree;
	degree += 90;

	//度数をラジアンに直す
	float angle = DirectX::XMConvertToRadians(degree);
	moveAngle = angle;
}
