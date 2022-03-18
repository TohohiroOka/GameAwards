#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
}

void BaseEnemy::Update()
{
	//通常時の移動
	if (isAlive)
	{
		//移動処理
		Move();

		//弾発射処理
		ShotBullet();
	}
	//ノックバックでの移動
	else
	{
		KnockBack();
	}

	//オブジェクト更新
	enemyObject->Update();
}

void BaseEnemy::Draw()
{
	//オブジェクト描画
	enemyObject->Draw();
}

void BaseEnemy::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	HP -= damagePower;
}

void BaseEnemy::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void BaseEnemy::SetKnockBack(float angle, int power)
{
	//ノックバックに使用する角度と強さをセット
	this->knockBackAngle = angle;
	this->killBulletPower = power;

	//死亡状態にする
	Dead();
}

void BaseEnemy::Move()
{
	//移動速度に移動角度を乗算して座標を更新
	float moveSpeed = 0.1f;
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

void BaseEnemy::ShotBullet()
{
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

void BaseEnemy::KnockBack()
{
	//ノックバックを行う時間
	const int knockBackTime = 20;
	//イージング計算用の時間
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//ノックバック基準の速度
	const float knockBackStartSpeed = 1.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);
	int power = killBulletPower / 10;

	//座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x -= knockBackSpeed * sinf(knockBackAngle) * power;
	pos.y += knockBackSpeed * cosf(knockBackAngle) * power;
	//更新した座標をセット
	enemyObject->SetPosition(pos);

	//ノックバックタイマー更新
	knockBackTimer++;
	//タイマーが指定した時間になったら
	if (knockBackTimer >= knockBackTime)
	{
		//色を薄くする
		//enemyObject->SetColor({ 1, 0, 0, 0.5f });

		//存在すら終了
		isExistence = false;
	}
}
