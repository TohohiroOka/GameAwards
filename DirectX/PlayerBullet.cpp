#include "PlayerBullet.h"
#include "SafeDelete.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 PlayerBullet::deadPosMin = {};
DirectX::XMFLOAT2 PlayerBullet::deadPosMax = {};

PlayerBullet* PlayerBullet::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	PlayerBullet* instance = new PlayerBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PlayerBullet::~PlayerBullet()
{
	//�I�u�W�F�N�g���
	safe_delete(bulletObject);
}

bool PlayerBullet::Initialize(Model* model)
{
	//�e�I�u�W�F�N�g����
	bulletObject = Object3d::Create();
	if (bulletObject == nullptr) {
		return false;
	}

	//�傫�����Z�b�g
	bulletObject->SetScale({ 2, 2, 1 });

	//���f�����Z�b�g
	if (model) {
		bulletObject->SetModel(model);
	}

	bulletObject->SetBloom(true);

	return true;
}

void PlayerBullet::Update()
{
	//�����Ă���Ƃ��̂�
	if (isAlive)
	{
		//�e�𓮂���
		Move();

		//�������ԍX�V
		AliveTimeUpdate();
	}
	else
	{
		//�Ռ��g�𔭎˂��Ȃ���ԂŌŒ肵�Ă���
		isShockWaveStart = false;
	}

	//�I�u�W�F�N�g�X�V
	bulletObject->Update();
}

void PlayerBullet::Draw()
{
	//���˒��̒e�̂ݕ`��
	if (!isAlive) return;

	//�I�u�W�F�N�g�`��
	bulletObject->Draw();
}

void PlayerBullet::Reset()
{
	//���ˊp�x
	angle = 0.0f;
	//�e�̐������ԃ^�C�}�[
	aliveTimer = 0;
	//�e�������Ă��邩
	isAlive = false;
	//�Ռ��g�𔭎˂��邩
	isShockWaveStart = false;
}

void PlayerBullet::BulletStart(XMFLOAT3 position, float angle)
{
	//���ˈʒu�A�e�̊p�x�A���ˊp�x��ݒ�
	bulletObject->SetPosition(position);
	XMFLOAT3 rota = { 0, 0, angle };
	bulletObject->SetRotation(rota);
	//���ˊp�x��ݒ肷�邽�߂Ɋp�x�����W�A���ɒ���(�E������0�Ȃ̂ŏ�����ɂ��炷)
	this->angle = DirectX::XMConvertToRadians(angle);

	//�������ԃ^�C�}�[��������
	aliveTimer = 0;

	//���ˏ�Ԃɂ���
	isAlive = true;

	//�Ռ��g�𔭎˂��Ȃ�
	isShockWaveStart = false;
}

void PlayerBullet::Dead()
{
	//�e�𔭎ˏ�Ԃł͂Ȃ�����
	isAlive = false;

	//�Ռ��g�𔭎˂���
	isShockWaveStart = true;
}

void PlayerBullet::Move()
{
	float moveSpeed = 5.0f;
	XMFLOAT3 pos = bulletObject->GetPosition();
	pos.x -= moveSpeed * sinf(angle);
	pos.y += moveSpeed * cosf(angle);
	//�X�V�������W���Z�b�g
	bulletObject->SetPosition(pos);

	//��ʊO�܂Ői�񂾂�
	XMFLOAT3 size = bulletObject->GetScale();
	if (pos.x <= deadPosMin.x + size.x / 2 || pos.y <= deadPosMin.y + size.y / 2 ||
		pos.x >= deadPosMax.x - size.x / 2 || pos.y >= deadPosMax.y - size.y / 2)
	{
		//���S
		Dead();
	}
}

void PlayerBullet::AliveTimeUpdate()
{
	//�����\����
	const int aliveTime = 10;

	//�������ԍX�V
	aliveTimer++;

	//�����\���Ԃ܂Ő�������
	if (aliveTimer >= aliveTime)
	{
		//���S
		Dead();
	}
}
