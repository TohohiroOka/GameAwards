#include "Zakorin.h"

Zakorin *Zakorin::Create(Model *model, XMFLOAT3 position)
{
	//インスタンスを生成
	Zakorin *instance = new Zakorin();
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

bool Zakorin::Initialize(Model *model, XMFLOAT3 position)
{
	//ザコリンオブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(position);
	//大きさをセット
	enemyObject->SetScale({ 0.4f, 0.4f, 0.4f });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	return true;
}

void Zakorin::Update()
{
	//生存中の敵のみ描画
	if (!isAlive) return;

	//XMFLOAT3 pos = enemyObject->GetPosition();
	//pos.x += 0.01f;
	//pos.y -= 0.01f;
	//enemyObject->SetPosition(pos);

	//オブジェクト更新
	enemyObject->Update();
}
