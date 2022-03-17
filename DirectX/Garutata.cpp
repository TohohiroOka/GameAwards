#include "Garutata.h"

Garutata *Garutata::Create(Model *model, XMFLOAT3 position, XMFLOAT3 targetPosition)
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

	//移動角度を設定
	instance->SetMoveAngle(targetPosition);

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
	enemyObject->SetScale({ 6, 6, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を緑にする
	//enemyObject->SetColor({ 0, 1, 0, 1 });

	return true;
}

void Garutata::Update()
{
	//通常時の移動
	if (isAlive)
	{
		//移動速度に移動角度を乗算して座標を更新
		float moveSpeed = 0.05f;
		XMFLOAT3 pos = enemyObject->GetPosition();
		pos.x += moveSpeed * cosf(moveAngle);
		pos.y += moveSpeed * sinf(moveAngle);
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

void Garutata::SetMoveAngle(XMFLOAT3 targetPosition)
{
	//移動角度を設定する（標的に向かって一直線）
	XMFLOAT3 position = enemyObject->GetPosition(); 
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	moveAngle = radian;
}
