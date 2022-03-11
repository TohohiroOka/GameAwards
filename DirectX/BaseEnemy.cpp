#include "BaseEnemy.h"
#include "SafeDelete.h"

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
}

void BaseEnemy::Draw()
{
	//生存中の敵のみ描画
	//if (!isAlive) return;

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