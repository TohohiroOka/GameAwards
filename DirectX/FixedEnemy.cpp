#include "FixedEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

FixedEnemy* FixedEnemy::Create(Model* coreModel, Model* squareModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//�C���X�^���X�𐶐�
	FixedEnemy* instance = new FixedEnemy();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(coreModel, squareModel, spawnPosition, stayPosition)) {
		delete instance;
		assert(0);
	}

	return instance;
}

FixedEnemy::~FixedEnemy()
{
	safe_delete(coreObject);
	safe_delete(squareObject);
}

bool FixedEnemy::Initialize(Model* coreModel, Model* squareModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//�X�|�[�����̍��W�ƈړ���̍��W���Z�b�g
	this->spawnPosition = spawnPosition;
	this->stayPosition = stayPosition;

	//�R�A�I�u�W�F�N�g����
	coreObject = Object3d::Create();
	if (coreObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	coreObject->SetPosition(spawnPosition);
	//�傫�����Z�b�g
	coreObject->SetScale({ 2.0f, 2.0f, 1 });

	//���f�����Z�b�g
	if (coreObject) {
		coreObject->SetModel(coreModel);
	}
	//�u���[����������
	coreObject->SetBloom(true);


	//�O�g�I�u�W�F�N�g����
	squareObject = Object3d::Create();
	if (squareObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	squareObject->SetPosition(spawnPosition);
	//�傫�����Z�b�g
	squareObject->SetScale({ 0 ,0, 1 });

	//���f�����Z�b�g
	if (squareObject) {
		squareObject->SetModel(squareModel);
	}
	//�u���[����������
	squareObject->SetBloom(true);

	return true;
}

void FixedEnemy::Update()
{
	//���݂�����ꍇ
	if (isExistence)
	{
		//�X�|�[�����̏���
		if (isDuringSpawn)
		{
			//�G���X�|�[��
			Spawn();
		}
		//�X�|�[�����ȊO�̏���
		else
		{
			//���S�����ꍇ
			if (!isAlive)
			{

				//�m�b�N�o�b�N��̃G�t�F�N�g����
				if (isEffect)
				{
					effectCount--;
					//�G�t�F�N�g���Ԃ�0�ȉ��ɂȂ�����
					if (effectCount <= 0)
					{
						//���݂��Ȃ��Ȃ�
						isExistence = false;
						//�G�t�F�N�g���I��
						isEffect = false;
						//�O�g�̑傫����ύX��ԂɃZ�b�g
						isChangeSquareSize = true;
						//�m�b�N�o�b�N���I������u��
						triggerEndKnockBack = true;
					}
				}
				//�m�b�N�o�b�N����
				else
				{
					KnockBack();
				}
			}
		}
	}
	//���݂��Ȃ��Ȃ�����
	else
	{
		//�傫���ύX��ԂȂ�
		if (isChangeSquareSize)
		{
			ChangeSquareSize();
		}
	}

	//�I�u�W�F�N�g�X�V
	coreObject->Update();
	squareObject->Update();
}

void FixedEnemy::Draw()
{
	//�I�u�W�F�N�g�`��
	coreObject->Draw();
	squareObject->Draw();
}

void FixedEnemy::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void FixedEnemy::SetDelete()
{
	//�폜����
	isDelete = true;
}

void FixedEnemy::SetKnockBack(float angle, int power)
{
	//�m�b�N�o�b�N�Ɏg�p����p�x�Ƌ������Z�b�g
	this->knockBackAngle = angle;
	this->killBulletPower = power;

	//���S��Ԃɂ���
	Dead();
}

bool FixedEnemy::TriggerEndKnockBack()
{
	//�m�b�N�o�b�N���I������u�ԂȂ�
	if (triggerEndKnockBack)
	{
		//���̃t���[���̓g���K�[�ł͂Ȃ��̂�false�ɖ߂�
		triggerEndKnockBack = false;

		return true;
	}

	//�m�b�N�o�b�N���I������u�Ԃł͂Ȃ�
	return false;
}

void FixedEnemy::Spawn()
{
	//�X�|�[�����s������
	const int spawnTime = 150;

	//�X�|�[���^�C�}�[�X�V
	spawnTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)spawnTimer / spawnTime;
	//�X�|�[�����̉�ʊO����̍��W�ړ�
	XMFLOAT3 pos = {};
	pos.x = Easing::OutCubic(spawnPosition.x, stayPosition.x, easeTimer);
	pos.y = Easing::OutCubic(spawnPosition.y, stayPosition.y, easeTimer);
	//�X�V�������W���Z�b�g
	coreObject->SetPosition(pos);
	squareObject->SetPosition(pos);

	//�X�|�[�����̐F�A���t�@�l
	float colorAlpha = Easing::OutCubic(0.0f, 1.0f, easeTimer);
	//�X�V�����A���t�@�l���Z�b�g
	coreObject->SetColor({ 1, 1, 1, colorAlpha });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (spawnTimer >= spawnTime)
	{
		//�X�|�[���I��
		isDuringSpawn = false;
	}
}

void FixedEnemy::KnockBack()
{
	//�m�b�N�o�b�N���s������
	const int knockBackTime = 20;

	//�m�b�N�o�b�N�^�C�}�[�X�V
	knockBackTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//�m�b�N�o�b�N��̑��x
	const float knockBackStartSpeed = 1.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);
	int power = killBulletPower / 10;

	//���W���X�V
	XMFLOAT3 pos = coreObject->GetPosition();
	pos.x -= knockBackSpeed * sinf(knockBackAngle) * power;
	pos.y += knockBackSpeed * cosf(knockBackAngle) * power;
	//�X�V�������W���Z�b�g
	coreObject->SetPosition(pos);
	squareObject->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (knockBackTimer >= knockBackTime)
	{
		//�G���|���ꂽ�Ƃ��̃G�t�F�N�g
		effectCount = StageEffect::SetEnemeyDead(coreObject->GetPosition());
		isEffect = true;
	}
}

void FixedEnemy::ChangeSquareSize()
{
	//�傫���ύX���s������
	const int changeTime = 20;

	//�傫���ύX�^�C�}�[�X�V
	changeSquareSizeTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeSquareSizeTimer / changeTime;

	//�傫�����X�V
	float size = 0;
	size = Easing::OutBack(0, 2, easeTimer);
	//�X�V�������W���Z�b�g
	squareObject->SetScale({ size, size, 1 });

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeSquareSizeTimer >= changeTime)
	{
		//�傫���ύX�I��
		isChangeSquareSize = false;
	}
}
