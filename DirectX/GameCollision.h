#pragma once

#include "Player.h"
#include "BaseEnemy.h"
#include "ShockWave.h"
#include "Wall.h"

/// <summary>
/// �Q�[���I�u�W�F�N�g�����蔻��N���X
/// </summary>
class GameCollision
{
public:
	/// <summary>
	/// �v���C���[�ƓG�̓����蔻��
	/// </summary>
	/// <param name="player">�v���C���[</param>
	/// <param name="enemy">�G</param>
	/// <returns>�Փ˂��Ă��邩�ۂ�</returns>
	static bool CheckPlayerToEnemy(Player* player, BaseEnemy* enemy);

	/// <summary>
	/// �Ռ��g�ƓG�̓����蔻��
	/// </summary>
	/// <param name="shockWave">�Ռ��g</param>
	/// <param name="enemy">�G</param>
	/// <returns>�Փ˂��Ă��邩�ۂ�</returns>
	static bool CheckShockWaveToEnemy(ShockWave* shockWave, BaseEnemy* enemy);

	/// <summary>
	/// �ǂƓG�̓����蔻��
	/// </summary>
	/// <param name="wall">��</param>
	/// <param name="enemy">�G</param>
	/// <returns>�Փ˂��Ă��邩�ۂ�</returns>
	static bool CheckWallToEnemy(Wall* wall, BaseEnemy* enemy);
};