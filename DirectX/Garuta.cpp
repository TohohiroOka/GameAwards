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
	enemyObject->SetScale({ 3, 3, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を赤くする
	//enemyObject->SetColor({ 1, 0, 0, 1 });

	return true;
}

void Garuta::Update()
{
	//通常時の移動
	if (isAlive)
	{
		//移動速度に移動角度を乗算して座標を更新
		float moveSpeed = 0.05f;
		XMFLOAT3 pos = enemyObject->GetPosition();
		pos.x -= moveSpeed * sinf(moveAngle);
		pos.y += moveSpeed * cosf(moveAngle);
		//更新した座標をセット
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

void Garuta::SetMoveAngle(float moveDegree)
{
	//引数の角度をラジアンに直して移動角度をセットする
	moveAngle = DirectX::XMConvertToRadians(moveDegree);
}
