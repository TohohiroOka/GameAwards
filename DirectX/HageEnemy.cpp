#include "HageEnemy.h"
#include "SafeDelete.h"

HageEnemy::~HageEnemy()
{
	safe_delete(enemyObject);
}

void HageEnemy::Update()
{
	//移動処理
	Move();

	//弾発射処理
	ShotBullet();

	//敵が画面外または死亡していたら削除状態にする
	if (isInScreen == false || isAlive == false)
	{
		SetDelete();
	}

	//敵オブジェクト更新
	enemyObject->Update();
}

void HageEnemy::Draw()
{
	//敵オブジェクト描画
	enemyObject->Draw();
}

void HageEnemy::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	HP -= damagePower;
}

void HageEnemy::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void HageEnemy::SetDelete()
{
	//削除する
	isDelete = true;
}

void HageEnemy::Move()
{
	//移動速度に移動角度を乗算して座標を更新
	float moveSpeed = 1.6f;
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += moveSpeed * cosf(moveAngle);
	pos.y += moveSpeed * sinf(moveAngle);

	//画面外に出たらフラグをfalseにする
	if (pos.x >= 120 || pos.x <= -120 || pos.y >= 75 || pos.y <= -75)
	{
		isInScreen = false;
	}

	//更新した座標をセット
	enemyObject->SetPosition(pos);
}

void HageEnemy::ShotBullet()
{
	//弾は毎フレーム発射しないのでfalseに戻しておく
	isBulletShot = false;
	//弾発射タイマーを更新する
	bulletShotTimer++;
	//弾発射タイマーが一定時間までカウントされたら
	const int bulletInterval = 800;
	if (bulletShotTimer >= bulletInterval)
	{
		//弾発射タイマー初期化
		bulletShotTimer = 0;

		//弾発射
		isBulletShot = true;
	}
}