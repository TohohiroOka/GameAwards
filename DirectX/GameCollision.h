#pragma once

#include "Player.h"
#include "BaseEnemy.h"
#include "ShockWave.h"
#include "WallManager.h"
#include "HealingZone.h"


/// <summary>
/// ゲームオブジェクト当たり判定クラス
/// </summary>
class GameCollision
{
public:
	/// <summary>
	/// プレイヤーと敵の当たり判定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="enemy">敵</param>
	/// <returns>衝突しているか否か</returns>
	static bool CheckPlayerToEnemy(Player* player, BaseEnemy* enemy);

	/// <summary>
	/// 衝撃波と敵の当たり判定
	/// </summary>
	/// <param name="shockWave">衝撃波</param>
	/// <param name="enemy">敵</param>
	/// <returns>衝突しているか否か</returns>
	static bool CheckShockWaveToEnemy(ShockWave* shockWave, BaseEnemy* enemy);

	/// <summary>
	/// 壁と敵の当たり判定
	/// </summary>
	/// <param name="wall">壁</param>
	/// <param name="enemy">敵</param>
	/// <returns>衝突しているか否か</returns>
	static bool CheckWallToEnemy(WallManager* wall, BaseEnemy* enemy);

	/// <summary>
	/// プレイヤーとゲージ回復地点の当たり判定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="healingZone">ゲージ回復地点</param>
	/// <returns>衝突しているか否か</returns>
	static bool CheckPlayerToHealingZone(Player* player, HealingZone* healingZone);
};