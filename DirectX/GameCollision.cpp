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
	if (!(enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo))
	{
		//�v���C���[�̓_���[�W����炤
		player->Damage();
		player->SetKnockback();

		//�G�����S
		enemy->SetDelete();
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

	//�Փ˔���
	bool isCollision = false;

	//�^�C�g�����S�̏ꍇ�~�Ƌ�`�̔�������
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo)
	{
		//��]0�̂Ƃ��̃^�C�g�����S�̎l�����W
		XMFLOAT3 leftTop = { enemyPos.x - 45, enemyPos.y + 20, 0 };
		XMFLOAT3 leftButtom = { enemyPos.x - 45, enemyPos.y - 15, 0 };
		XMFLOAT3 rightButtom = { enemyPos.x + 45, enemyPos.y - 15, 0 };
		XMFLOAT3 rightTop = { enemyPos.x + 45, enemyPos.y + 1, 0 };

		//���S�̉�]
		float angle = XMConvertToRadians(enemy->GetRotation().z);
		float angleSin = sinf(angle);
		float angleCos = cosf(angle);

		//��]��̎l�����W
		XMFLOAT3 leftTopRota = { (leftTop.x - enemyPos.x) * angleCos - (leftTop.y - enemyPos.y) * angleSin,
			(leftTop.x - enemyPos.x) * angleSin - (leftTop.y - enemyPos.y) * angleCos, 0 };
		XMFLOAT3 leftButtomRota = { (leftButtom.x - enemyPos.x) * angleCos - (leftButtom.y - enemyPos.y) * angleSin,
			(leftButtom.x - enemyPos.x) * angleSin - (leftButtom.y - enemyPos.y) * angleCos, 0 };
		XMFLOAT3 rightButtomRota = { (rightButtom.x - enemyPos.x) * angleCos - (rightButtom.y - enemyPos.y) * angleSin,
			(rightButtom.x - enemyPos.x) * angleSin - (rightButtom.y - enemyPos.y) * angleCos, 0 };
		XMFLOAT3 rightTopRota = { (rightTop.x - enemyPos.x) * angleCos - (rightTop.y - enemyPos.y) * angleSin,
			(rightTop.x - enemyPos.x) * angleSin - (rightTop.y - enemyPos.y) * angleCos, 0 };

		//�����蔻��Ɏg�p����l�����W
		XMFLOAT3 leftTopReTrans = { leftTopRota.x + enemyPos.x, leftTopRota.y + enemyPos.y, 0 };
		XMFLOAT3 leftButtomReTrans = { leftButtomRota.x + enemyPos.x, leftButtomRota.y + enemyPos.y, 0 };
		XMFLOAT3 rightButtomReTrans = { rightButtomRota.x + enemyPos.x, rightButtomRota.y + enemyPos.y, 0 };
		XMFLOAT3 rightTopReTrans = { rightTopRota.x + enemyPos.x, rightTopRota.y + enemyPos.y, 0 };

		//�Փ˔�����v�Z
		isCollision = Collision::CheckCircle2Rectangle(
			wavePos, waveSize, leftTopReTrans, leftButtomReTrans, rightButtomReTrans, rightTopReTrans);
	}
	//�^�C�g�����S�ȊO�̓G�͉~���m�̔�������
	else
	{
		//�Փ˔�����v�Z
		isCollision = Collision::CheckCircle2Circle(
			wavePos, waveSize, enemyPos, enemySize);
	}

	//�Ռ��g�ƓG���Փˏ�ԂłȂ���Δ�����
	if (!isCollision) { return false; }

	//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��_���[�W��H�炢�����Ă��܂�����)
	if (shockWave->IsKnowEnemy(enemy)) { return false; }

	//�G���m�b�N�o�b�N�Ŕ�΂�
	float angle = atan2f(enemyPos.y - wavePos.y, enemyPos.x - wavePos.x);
	int powerLevel = shockWave->GetPowerLevel();
	//�^�C�g�����S�݈̂З͂����߂Đ�����΂�
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo) { powerLevel = 2; }
	int shockWaveGroup = shockWave->GetGroup();
	float powerMagnification = shockWave->GetPowerMagnification();
	enemy->SetKnockBack(angle, powerLevel, powerMagnification, shockWaveGroup);

	return true;
}

bool GameCollision::CheckWallToEnemy(WallManager* wall, BaseEnemy* enemy)
{
	//�G���m�b�N�o�b�N���łȂ���Δ�����
	if (!enemy->GetIsKnockBack()) { return false; }

	//�ǂɓ������Ă��Ȃ���Δ�����
	if (!enemy->IsCollisionWall()) { return false; }

	//�G�Ƀ_���[�W��^����
	enemy->Damage();

	//�ǂɂ��_���[�W��^����
	int damagePower = 1;
	//�^�C�g�����S�̂݃_���[�W���グ��
	if (enemy->GetGroup() == BaseEnemy::EnemyGroup::TitleLogo) { damagePower = 7; }
	wall->Damage(damagePower);

	return true;
}
