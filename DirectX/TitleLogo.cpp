#include "TitleLogo.h"
#include "Easing.h"

Model* TitleLogo::titleLogoModel = nullptr;
bool TitleLogo::isSpawnEnd = false;
const DirectX::XMFLOAT3 TitleLogo::spawnPos = { 0, 150, 0 };
const DirectX::XMFLOAT3 TitleLogo::stayPos = { 0, 61, 0 };

TitleLogo* TitleLogo::Create()
{
	//�C���X�^���X�𐶐�
	TitleLogo* instance = new TitleLogo();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize({}, 0)) {
		delete instance;
		assert(0);
	}

	return instance;
}

void TitleLogo::SetModel(Model* titleLogoModel)
{
	//�����̃��f�������ʂŎg�����߃Z�b�g����
	TitleLogo::titleLogoModel = titleLogoModel;
}

bool TitleLogo::GetTriggerSpawnEnd()
{
	if (isSpawnEnd)
	{
		//�g���K�[�Ȃ̂�false�ɖ߂�
		isSpawnEnd = false;

		return true;
	}

	return false;
}

bool TitleLogo::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//�����O���[�v��ݒ�
	group = EnemyGroup::TitleLogo;

	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�X�|�[�����W�Z�b�g
	enemyObject->SetPosition(spawnPos);

	//�傫�����Z�b�g
	enemyObject->SetScale({ 20, 20, 1 });

	//���f�����Z�b�g
	if (titleLogoModel) {
		enemyObject->SetModel(titleLogoModel);
	}

	//��̃_���[�W�ʂ��Z�b�g
	baseDamagePower = 1;

	return true;
}

void TitleLogo::SetKnockBack(float angle, int powerLevel, float powerMagnification)
{
	BaseEnemy::SetKnockBack(angle, powerLevel, powerMagnification);

	//��x�ǂɓ��������������Z�b�g����
	isCollisionWallFirst = false;
}

bool TitleLogo::IsCollisionWall()
{
	//�g�ɂԂ����Ă�����true��Ԃ�
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	bool isCollision = false;

	if (pos.x <= wallLineMin.x + size.x / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.x = wallLineMin.x + size.x / 2;
		enemyObject->SetPosition(pos);

		//���˂�����
		ReflectionX();

		//�ǂɓ�������
		isCollision = true;
	}
	else if (pos.x >= wallLineMax.x - size.x / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.x = wallLineMax.x - size.x / 2;
		enemyObject->SetPosition(pos);

		//���˂�����
		ReflectionX();

		//�ǂɓ�������
		isCollision = true;
	}
	if (pos.y <= wallLineMin.y + size.y / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.y = wallLineMin.y + size.y / 2;
		enemyObject->SetPosition(pos);

		//���˂�����
		ReflectionY();

		//�ǂɓ�������
		isCollision = true;
	}
	else if (pos.y >= wallLineMax.y - size.y / 2)
	{
		//�g����o�Ȃ��悤�ɂ���
		pos.y = wallLineMax.y - size.y / 2;
		enemyObject->SetPosition(pos);

		//���˂�����
		ReflectionY();

		//�ǂɓ�������
		isCollision = true;
	}

	//���Ɉ�x�ǂɓ������Ă���ꍇ��false
	if (isCollisionWallFirst) { return false; }

	//�ǂɓ������Ă��Ȃ���Δ�����
	if (!isCollision) { return false; }

	//��x�ǂɓ�������
	isCollisionWallFirst = true;

	//�ǂɓ���������Ԃ�
	return true;
}

void TitleLogo::Move()
{
	//�X�|�[�����ȊO�͔�����
	if (!isSpawn) { return; }

	//�X�|�[�����s������
	const int spawnTime = 90;

	//�^�C�}�[���X�V
	spawnTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spawnTimer / spawnTime;

	//�C�[�W���O�Ń^�C�g�����S�𓮂���
	XMFLOAT3 logoPos = enemyObject->GetPosition();
	logoPos.y = Easing::OutBounce(spawnPos.y, stayPos.y, easeTimer);
	enemyObject->SetPosition(logoPos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spawnTimer >= spawnTime)
	{
		//�X�|�[���I��
		isSpawn = false;
		//�X�|�[������
		isSpawnEnd = true;
	}
}

void TitleLogo::ResultMove()
{
}

void TitleLogo::ReflectionX()
{
	//���E���˗p�ɔ��ˊp���Z�b�g����
	float knockBackDegree = DirectX::XMConvertToDegrees(knockBackAngle);
	float reflectionDegree = 180 - knockBackDegree;
	knockBackAngle = DirectX::XMConvertToRadians(reflectionDegree);

	//�m�b�N�o�b�N�̎��Ԃ��������₷
	knockBackTime += 10;
}

void TitleLogo::ReflectionY()
{
	//�㉺���˗p�ɔ��ˊp���Z�b�g����
	float knockBackDegree = DirectX::XMConvertToDegrees(knockBackAngle);
	float reflectionDegree = 360 - knockBackDegree;
	knockBackAngle = DirectX::XMConvertToRadians(reflectionDegree);

	//�m�b�N�o�b�N�̎��Ԃ��������₷
	knockBackTime += 10;
}
