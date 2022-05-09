#include "GameCollision.h"
#include "Collision.h"

using namespace DirectX;

bool GameCollision::CheckPlayerToEnemy(Player* player, BaseEnemy* enemy)
{
	//プレイヤーがダメージ状態なら抜ける
	if (player->GetIsDamege()) { return false; }

	//衝突用に座標と半径の大きさを借りる
	XMFLOAT3 enemyPos = enemy->GetPosition();
	float enemySize = enemy->GetScale().x;
	XMFLOAT3 playerPos = player->GetPosition();
	float playerSize = player->GetScale().x;

	//衝突判定を計算
	bool isCollision = Collision::CheckCircle2Circle(
		enemyPos, enemySize, playerPos, playerSize);

	//敵とプレイヤーが衝突状態でないなら抜ける
	if (!isCollision) { return false; }

	//タイトルロゴ以外の敵の場合
	if (enemy->GetGroup() >= 1 && enemy->GetGroup() <= 4)
	{
		//プレイヤーはダメージを喰らう
		player->Damage();
		player->SetKnockback();

		//敵も死亡
		enemy->SetDelete();
	}
	//タイトルロゴの場合
	else
	{
	}

	return true;
}

bool GameCollision::CheckShockWaveToEnemy(ShockWave* shockWave, BaseEnemy* enemy)
{
	//衝撃波が発射状態でなければ抜ける
	if (!shockWave->GetIsAlive()) { return false; }

	//当たり判定用変数
	XMFLOAT3 wavePos = shockWave->GetPosition();
	float waveSize = shockWave->GetRadius();
	XMFLOAT3 enemyPos = enemy->GetPosition();
	float enemySize = enemy->GetScale().x;

	//衝突判定を計算
	bool isCollision = Collision::CheckCircle2Circle(
		wavePos, waveSize, enemyPos, enemySize);

	//衝撃波と敵が衝突状態でなければ抜ける
	if (!isCollision) { return false; }

	//既に衝突したことがあるか確認(衝突中ダメージを食らい続けてしまうため)
	if (shockWave->IsKnowEnemy(enemy)) { return false; }

	//敵をノックバックで飛ばす
	float angle = atan2f(enemyPos.y - wavePos.y, enemyPos.x - wavePos.x);
	int powerLevel = shockWave->GetPowerLevel();
	//タイトルロゴのみ威力を高めて吹っ飛ばす
	if (enemy->GetGroup() == 5) { powerLevel = 2; }
	int shockWaveGroup = shockWave->GetGroup();
	enemy->SetKnockBack(angle, powerLevel, shockWaveGroup);

	return true;
}

bool GameCollision::CheckWallToEnemy(Wall* wall, BaseEnemy* enemy)
{
	//敵がノックバック中でなければ抜ける
	if (!enemy->GetIsKnockBack()) { return false; }

	//壁に当たっていなければ抜ける
	if (!enemy->IsCollisionWall()) { return false; }

	//敵を死亡させる死亡
	enemy->Dead();

	//壁にもダメージを与える
	int damagePower = enemy->GetPower();
	wall->Damage(damagePower);

	return true;
}
