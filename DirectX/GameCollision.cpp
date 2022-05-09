#include "GameCollision.h"
#include "Collision.h"

using namespace DirectX;

bool GameCollision::CheckPlayerToEnemy(Player* player, BaseEnemy* enemy)
{
	//�v���C���[���_���[�W��ԂȂ甲����
	if (player->GetIsDamege()) { return false; }

	//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
	XMFLOAT3 enemyPos = enemy->GetPosition();
	float enemySize = enemy->GetScale().x;
	XMFLOAT3 playerPos = player->GetPosition();
	float playerSize = player->GetScale().x;

	//�Փ˔�����v�Z
	bool isCollision = Collision::CheckCircle2Circle(
		enemyPos, enemySize, playerPos, playerSize);

	//�G�ƃv���C���[���Փˏ�ԂłȂ��Ȃ甲����
	if (!isCollision) { return false; }

	//�^�C�g�����S�ȊO�̓G�̏ꍇ
	if (enemy->GetGroup() >= 1 && enemy->GetGroup() <= 4)
	{
		//�v���C���[�̓_���[�W����炤
		player->Damage();
		player->SetKnockback();

		//�G�����S
		enemy->SetDelete();
	}
	//�^�C�g�����S�̏ꍇ
	else
	{
	}

	return true;
}

bool GameCollision::CheckShockWaveToEnemy(ShockWave* shockWave, BaseEnemy* enemy)
{
	//�Ռ��g�����ˏ�ԂłȂ���Δ�����
	if (!shockWave->GetIsAlive()) { return false; }

	//�����蔻��p�ϐ�
	XMFLOAT3 wavePos = shockWave->GetPosition();
	float waveSize = shockWave->GetRadius();
	XMFLOAT3 enemyPos = enemy->GetPosition();
	float enemySize = enemy->GetScale().x;

	//�Փ˔�����v�Z
	bool isCollision = Collision::CheckCircle2Circle(
		wavePos, waveSize, enemyPos, enemySize);

	//�Ռ��g�ƓG���Փˏ�ԂłȂ���Δ�����
	if (!isCollision) { return false; }

	//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��_���[�W��H�炢�����Ă��܂�����)
	if (shockWave->IsKnowEnemy(enemy)) { return false; }

	//�G���m�b�N�o�b�N�Ŕ�΂�
	float angle = atan2f(enemyPos.y - wavePos.y, enemyPos.x - wavePos.x);
	int powerLevel = shockWave->GetPowerLevel();
	//�^�C�g�����S�݈̂З͂����߂Đ�����΂�
	if (enemy->GetGroup() == 5) { powerLevel = 2; }
	int shockWaveGroup = shockWave->GetGroup();
	enemy->SetKnockBack(angle, powerLevel, shockWaveGroup);

	return true;
}

bool GameCollision::CheckWallToEnemy(Wall* wall, BaseEnemy* enemy)
{
	//�G���m�b�N�o�b�N���łȂ���Δ�����
	if (!enemy->GetIsKnockBack()) { return false; }

	//�ǂɓ������Ă��Ȃ���Δ�����
	if (!enemy->IsCollisionWall()) { return false; }

	//�G�����S�����鎀�S
	enemy->Dead();

	//�ǂɂ��_���[�W��^����
	int damagePower = enemy->GetPower();
	wall->Damage(damagePower);

	return true;
}
