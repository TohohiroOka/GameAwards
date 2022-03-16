#include "Garuta.h"

Garuta *Garuta::Create(Model *model, XMFLOAT3 position)
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
	enemyObject->SetScale({ 3, 3, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を赤くする
	//enemyObject->SetColor({ 1, 1, 1, 1 });

	return true;
}

void Garuta::Update()
{
	//通常時の移動
	if (isAlive)
	{
		XMFLOAT3 pos = enemyObject->GetPosition();
		pos.x += 0.01f;
		pos.y -= 0.01f;
		enemyObject->SetPosition(pos);

		//弾は毎フレーム発射しないのでfalseに戻しておく
		isBulletShot = false;
		//弾発射タイマーを更新する
		bulletShotTimer++;
		//弾発射タイマーが一定時間までカウントされたら
		const int bulletInterval = 300;
		if (bulletShotTimer >= bulletInterval)
		{
			//弾発射タイマー初期化
			bulletShotTimer = 0;

			//弾発射
			isBulletShot = true;
		}
	}
	//ノックバックでの移動
	else
	{
		KnockBack();
	}

	//オブジェクト更新
	enemyObject->Update();
}