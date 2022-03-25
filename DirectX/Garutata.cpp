#include "Garutata.h"

Garutata *Garutata::Create(Model *garutataModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//インスタンスを生成
	Garutata *instance = new Garutata();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(garutataModel, stayPointModel, spawnPosition, stayPosition)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Garutata::Initialize(Model *enemyModel, Model *stayPointModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//スポーン時の座標と移動後の座標をセット
	this->spawnPosition = spawnPosition;
	this->stayPosition = stayPosition;

	//ガルタタオブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(spawnPosition);
	//大きさをセット
	enemyObject->SetScale({ 3, 3, 1 });

	//モデルをセット
	if (enemyModel) {
		enemyObject->SetModel(enemyModel);
	}

	//色を変更
	enemyObject->SetColor({ 1, 1, 1, 1 });

	//ブルームをかける
	enemyObject->SetBloom(true);

	//スポーン地点オブジェクト生成
	stayPointObject = Object3d::Create();
	if (stayPointObject == nullptr) {
		return false;
	}
	//スポーン座標セット
	stayPointObject->SetPosition(stayPosition);
	//大きさをセット
	stayPointObject->SetScale({ 3, 3, 1 });

	//モデルをセット
	if (stayPointModel) {
		stayPointObject->SetModel(stayPointModel);
	}
	//色を変更
	stayPointObject->SetColor({ 1, 1, 1, 0.4 });

	//ブルームをかける
	//stayPointObject->SetBloom(true);

	//停止座標オブジェクトを更新
	stayPointObject->Update();

	return true;
}