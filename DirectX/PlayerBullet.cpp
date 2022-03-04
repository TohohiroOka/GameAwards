#include "PlayerBullet.h"
#include "SafeDelete.h"

PlayerBullet *PlayerBullet::Create(UINT texNumber)
{
	//�C���X�^���X�𐶐�
	PlayerBullet *instance = new PlayerBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(texNumber)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PlayerBullet::~PlayerBullet()
{
	safe_delete(bulletSprite);
}

bool PlayerBullet::Initialize(UINT texNumber)
{
	//�v���C���[�X�v���C�g����
	bulletSprite = Sprite::Create(texNumber);
	if (bulletSprite == nullptr) {
		return false;
	}

	//�傫�����Z�b�g
	bulletSprite->SetSize({ 20.0f, 20.0f });

	return true;
}

void PlayerBullet::Update()
{
	//���˒��̒e�̂ݍX�V
	if(!isAlive) return;

	//�e�𓮂���
	float moveSpeed = 15.0f;
	XMFLOAT2 pos = bulletSprite->GetPosition();
	pos.x += moveSpeed * sinf(angle);
	pos.y -= moveSpeed * cosf(angle);
	//�X�V�������W���Z�b�g
	bulletSprite->SetPosition(pos);

	//�͈͊O�܂Ői�񂾂�e������
	XMFLOAT2 bulletAliveLineMin = { -10.0f, -10.0f };
	XMFLOAT2 bulletAliveLineMax = { 1280.0f, 720.0f };
	XMFLOAT2 size = bulletSprite->GetSize();
	if (pos.x < bulletAliveLineMin.x - size.x || pos.y < bulletAliveLineMin.y - size.y
		|| pos.x > bulletAliveLineMax.x + size.x || pos.y > bulletAliveLineMax.y + size.y)
	{
		isAlive = false;
	}

	//�X�v���C�g�X�V
	bulletSprite->Update();
}

void PlayerBullet::Draw()
{
	//���˒��̒e�̂ݕ`��
	if(!isAlive) return;

	//�X�v���C�g�`��
	bulletSprite->Draw();
}

void PlayerBullet::BulletStart(XMFLOAT2 position, float rotation)
{
	//���ˈʒu�A�e�̊p�x�A���ˊp�x��ݒ�
	bulletSprite->SetPosition(position);
	bulletSprite->SetRotation(rotation);
	this->angle = rotation * (3.141592f * 2) / 360;

	//���ˏ�Ԃɂ���
	isAlive = true;
}

void PlayerBullet::Dead()
{
	//�e�𔭎ˏ�Ԃł͂Ȃ�����
	isAlive = false;
}
