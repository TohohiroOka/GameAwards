#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
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
		enemyObject->SetColor({ 1, 0, 0, 0.5f });

		//存在すら終了
		isExistence = false;
	}
}
