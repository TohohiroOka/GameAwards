#include "BaseEnemy.h"
#include "SafeDelete.h"

int BaseEnemy::deadCount = 0;

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
}

void BaseEnemy::Draw()
{
	//生存中の敵のみ描画
	if (!isAlive) return;

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

	//分かりやすいように色を変える
	//enemyObject->SetColor({ 1,0,0,1 });

	//カウントを増やす
	deadCount++;

	//死んだ順番を割り当てる(0は初期値なので1から)
	//deadNum = deadCount;
}
