#include "EnemyBullet.h"
#include "SafeDelete.h"

EnemyBullet *EnemyBullet::Create(Model *model)
{
	//�C���X�^���X�𐶐�
	EnemyBullet *instance = new EnemyBullet();
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

bool EnemyBullet::Initialize(Model *model)
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

void EnemyBullet::BulletStart(XMFLOAT3 position, XMFLOAT3 targetPosition)
{
	//���ˈʒu�A�e�̊p�x�A���ˊp�x��ݒ�
	bulletObject->SetPosition(position);

	//���ˊp�x��ݒ肷��i�W�I�Ɍ������Ĉ꒼���j
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	angle = radian;

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
	float moveSpeed = 0.5f;
	XMFLOAT3 pos = bulletObject->GetPosition();
	pos.x += moveSpeed * cosf(angle);
	pos.y += moveSpeed * sinf(angle);
	//�X�V�������W���Z�b�g
	bulletObject->SetPosition(pos);

	//�͈͊O�܂Ői�񂾂�
	XMFLOAT2 bulletAliveLineMin = { -100.0f, -100.0f };
	XMFLOAT2 bulletAliveLineMax = { 100.0f, 100.0f };
	XMFLOAT3 size = bulletObject->GetScale();
	if (pos.x < bulletAliveLineMin.x - size.x || pos.y < bulletAliveLineMin.y - size.y
		|| pos.x > bulletAliveLineMax.x + size.x || pos.y > bulletAliveLineMax.y + size.y)
	{
		//�e������
		Dead();
	}
}
