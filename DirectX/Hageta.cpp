#include "Hageta.h"

Hageta *Hageta::Create(Model *model, XMFLOAT3 position, float moveDegree)
{
	//インスタンスを生成
	Hageta *instance = new Hageta();
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

bool Hageta::Initialize(Model *model, XMFLOAT3 position)
{
	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(position);
	//大きさをセット
	enemyObject->SetScale({ 2, 2, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//ブルームをかける
	enemyObject->SetBloom(true);

	//色を赤くする
	enemyObject->SetColor({ 1, 1, 1, 1 });

	return true;
}

void Hageta::SetMoveAngle(float moveDegree)
{
	//上向きを0にするため90度傾ける
	float degree = moveDegree;
	degree += 90;

	//度数をラジアンに直す
	float angle = DirectX::XMConvertToRadians(degree);
	moveAngle = angle;

	//オブジェクトの向きを進行方向にセット
	XMFLOAT3 rota = { 0, 0, moveDegree };
	enemyObject->SetRotation(rota);
}
