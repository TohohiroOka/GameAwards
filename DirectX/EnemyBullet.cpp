#include "EnemyBullet.h"
#include "SafeDelete.h"

DirectX::XMFLOAT2 EnemyBullet::deadPos = { 110, 60 };

EnemyBullet* EnemyBullet::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	EnemyBullet* instance = new EnemyBullet();
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

EnemyBullet::~EnemyBullet()
{
	//�I�u�W�F�N�g���
	safe_delete(bulletObject);
}

bool EnemyBullet::Initialize(Model* model)
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

	//�F�����F�ɂ���
	bulletObject->SetColor({ 1, 1, 0, 1 });

	return true;
}

void EnemyBullet::Update()
{
	//�e�𓮂���
	Move();

	//�I�u�W�F�N�g�X�V
	bulletObject->Update();
}

void EnemyBullet::Draw()
{
	//���˒��̒e�̂ݕ`��
	if (!isAlive) { return; }

	//�I�u�W�F�N�g�`��
	bulletObject->Draw();
}

void EnemyBullet::AimBulletStart(XMFLOAT3 position, XMFLOAT3 targetPosition, float moveSpeed)
{
	//���ˈʒu��ݒ�
	bulletObject->SetPosition(position);
	//���ˊp�x��ݒ肷��i�W�I�Ɍ������Ĉ꒼���j
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	angle = radian;
	//�I�u�W�F�N�g�̊p�x��ݒ�
	float degree = DirectX::XMConvertToDegrees(radian);
	XMFLOAT3 rotation = { 0, 0, degree - 90 };
	bulletObject->SetRotation(rotation);

	//�e�̈ړ����x��ݒ�
	this->moveSpeed = moveSpeed;

	//���ˏ�Ԃɂ���
	isAlive = true;
}

void EnemyBullet::StraightBulletStart(XMFLOAT3 position, float angle, float moveSpeed)
{
	//���ˈʒu�A�e�̊p�x�A���ˊp�x��ݒ�
	bulletObject->SetPosition(position);
	XMFLOAT3 rota = { 0, 0, angle };
	bulletObject->SetRotation(rota);
	//���ˊp�x��ݒ肷�邽�߂Ɋp�x�����W�A���ɒ���(�E������0�Ȃ̂ŏ�����ɂ��炷)
	this->angle = DirectX::XMConvertToRadians(angle + 90);

	//�e�̈ړ����x��ݒ�
	this->moveSpeed = moveSpeed;

	//���ˏ�Ԃɂ���
	isAlive = true;
}

void EnemyBullet::Dead()
{
	//�e�𔭎ˏ�Ԃł͂Ȃ�����
	isAlive = false;
}

void EnemyBullet::Move()
{
	XMFLOAT3 pos = bulletObject->GetPosition();
	pos.x += moveSpeed * cosf(angle);
	pos.y += moveSpeed * sinf(angle);
	//�X�V�������W���Z�b�g
	bulletObject->SetPosition(pos);

	//��ʊO�܂Ői�񂾂�
	XMFLOAT3 size = bulletObject->GetScale();
	if (pos.x <= -deadPos.x || pos.y <= -deadPos.y || pos.x >= deadPos.x || pos.y >= deadPos.y)
	{
		//�e������
		Dead();
	}
}
