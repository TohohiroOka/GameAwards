#include "GameCollision.h"

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
	if (!(enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo))
	{
		//プレイヤーはダメージを喰らう
		player->Damage();
		player->SetKnockback();

		//敵も死亡
		enemy->SetDelete();
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

	//衝突判定
	bool isCollision = false;

	//タイトルロゴの場合円と矩形の判定を取る
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo)
	{
		//回転0のときのタイトルロゴの四隅座標
		XMFLOAT3 leftTop = { enemyPos.x - 45, enemyPos.y + 20, 0 };
		XMFLOAT3 leftButtom = { enemyPos.x - 45, enemyPos.y - 15, 0 };
		XMFLOAT3 rightButtom = { enemyPos.x + 45, enemyPos.y - 15, 0 };
		XMFLOAT3 rightTop = { enemyPos.x + 45, enemyPos.y + 1, 0 };

		//ロゴの回転
		float angle = XMConvertToRadians(enemy->GetRotation().z);
		float angleSin = sinf(angle);
		float angleCos = cosf(angle);

		//回転後の四隅座標
		XMFLOAT3 leftTopRota = { (leftTop.x - enemyPos.x) * angleCos - (leftTop.y - enemyPos.y) * angleSin,
			(leftTop.x - enemyPos.x) * angleSin - (leftTop.y - enemyPos.y) * angleCos, 0 };
		XMFLOAT3 leftButtomRota = { (leftButtom.x - enemyPos.x) * angleCos - (leftButtom.y - enemyPos.y) * angleSin,
			(leftButtom.x - enemyPos.x) * angleSin - (leftButtom.y - enemyPos.y) * angleCos, 0 };
		XMFLOAT3 rightButtomRota = { (rightButtom.x - enemyPos.x) * angleCos - (rightButtom.y - enemyPos.y) * angleSin,
			(rightButtom.x - enemyPos.x) * angleSin - (rightButtom.y - enemyPos.y) * angleCos, 0 };
		XMFLOAT3 rightTopRota = { (rightTop.x - enemyPos.x) * angleCos - (rightTop.y - enemyPos.y) * angleSin,
			(rightTop.x - enemyPos.x) * angleSin - (rightTop.y - enemyPos.y) * angleCos, 0 };

		//当たり判定に使用する四隅座標
		XMFLOAT3 leftTopReTrans = { leftTopRota.x + enemyPos.x, leftTopRota.y + enemyPos.y, 0 };
		XMFLOAT3 leftButtomReTrans = { leftButtomRota.x + enemyPos.x, leftButtomRota.y + enemyPos.y, 0 };
		XMFLOAT3 rightButtomReTrans = { rightButtomRota.x + enemyPos.x, rightButtomRota.y + enemyPos.y, 0 };
		XMFLOAT3 rightTopReTrans = { rightTopRota.x + enemyPos.x, rightTopRota.y + enemyPos.y, 0 };

		//衝突判定を計算
		isCollision = Collision::CheckCircle2Rectangle(
			wavePos, waveSize, leftTopReTrans, leftButtomReTrans, rightButtomReTrans, rightTopReTrans);
	}
	//タイトルロゴ以外の敵は円同士の判定を取る
	else
	{
		//衝突判定を計算
		isCollision = Collision::CheckCircle2Circle(
			wavePos, waveSize, enemyPos, enemySize);
	}

	//衝撃波と敵が衝突状態でなければ抜ける
	if (!isCollision) { return false; }

	//既に衝突したことがあるか確認(衝突中ダメージを食らい続けてしまうため)
	if (shockWave->IsKnowEnemy(enemy)) { return false; }

	//敵をノックバックで飛ばす
	float angle = atan2f(enemyPos.y - wavePos.y, enemyPos.x - wavePos.x);
	int powerLevel = shockWave->GetPowerLevel();
	//タイトルロゴのみ威力を高めて吹っ飛ばす
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo) { powerLevel = 2; }
	float powerMagnification = shockWave->GetPowerMagnification();
	enemy->SetKnockBack(angle, powerLevel, powerMagnification);

	return true;
}

bool GameCollision::CheckWallToEnemy(WallManager* wall, BaseEnemy* enemy)
{
	//敵がノックバック中でなければ抜ける
	if (!enemy->GetIsKnockBack()) { return false; }

	//タイトルロゴの場合
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo)
	{
		//壁に当たっていなければ抜ける
		if (!enemy->IsCollisionWall()) { return false; }
	}
	//タイトルロゴ以外の敵の場合
	else
	{
		//衝突用に座標と半径の大きさを借りる
		bool isCollision = false;
		XMFLOAT3 enemyPos = enemy->GetPosition();
		float enemySize = enemy->GetScale().x;

		//壁オブジェクト全てと当たり判定をとる
		for (auto& i : wall->GetWallObject())
		{
			XMFLOAT3 wallObjectPos = i->GetPosition();
			float wallObjectSize = i->GetScale().x;

			//衝突判定を計算
			isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, wallObjectPos, wallObjectSize);

			//敵と壁のオブジェクトが衝突状態ならループを抜ける
			if (isCollision) { break; }
		}

		//オブジェクト全てと当たっていなければ抜ける
		if (!isCollision) { return false; }
	}

	//敵にダメージを与える
	enemy->Damage();

	//壁にもダメージを与える(衝撃波を当てた時の距離で壁に与えるダメージを変更)
	int damagePower = enemy->GetDamagePower();
	//タイトルロゴのみダメージを上げる
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo) { damagePower = 7; }
	wall->Damage(damagePower);

	return true;
}
