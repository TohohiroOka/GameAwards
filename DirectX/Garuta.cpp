#include "Garuta.h"

Garuta *Garuta::Create(Model *garutaModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//インスタンスを生成
	Garuta *instance = new Garuta();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(garutaModel, stayPointModel, spawnPosition, stayPosition)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Garuta::Initialize(Model *enemyModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//スポーン時の座標と移動後の座標をセット
	this->spawnPosition = spawnPosition;
	this->stayPosition = stayPosition;

	//ガルタオブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(spawnPosition);
	//大きさをセット
	enemyObject->SetScale({ 1.5, 1.5, 1 });

	//モデルをセット
	if (enemyModel) {
		enemyObject->SetModel(enemyModel);
	}
	//ブルームをかける
	//enemyObject->SetBloom(true);


	//スポーン地点オブジェクト生成
	stayPointObject = Object3d::Create();
	if (stayPointObject == nullptr) {
		return false;
	}
	//スポーン座標セット
	stayPointObject->SetPosition(stayPosition);
	//大きさをセット
	stayPointObject->SetScale({ 1.5, 1.5, 1 });

	//モデルをセット
	if (stayPointModel) {
		stayPointObject->SetModel(stayPointModel);
	}
	//色を変更
	stayPointObject->SetColor({ 0, 1, 1, 1 });
	//停止座標オブジェクトを更新
	stayPointObject->Update();

	return true;
}