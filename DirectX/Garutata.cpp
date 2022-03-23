#include "Garutata.h"

Garutata *Garutata::Create(Model *model, XMFLOAT3 position)
{
	//インスタンスを生成
	Garutata *instance = new Garutata();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Garutata::Initialize(Model *model, XMFLOAT3 position)
{
	//ザコリンオブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(position);
	//大きさをセット
	enemyObject->SetScale({ 3, 3, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を緑にする
	//enemyObject->SetColor({ 0, 1, 0, 1 });

	return true;
}