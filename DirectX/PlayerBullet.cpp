#include "PlayerBullet.h"
#include "SafeDelete.h"

PlayerBullet *PlayerBullet::Create(Model *model)
{
	//�C���X�^���X�𐶐�
	PlayerBullet *instance = new PlayerBullet();
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
	//�e���m���Ă�����̃��X�g���
	alreadyLines.clear();

	//�I�u�W�F�N�g���
	safe_delete(bulletObject);
}

bool PlayerBullet::Initialize(Model *model)
{
	//�e�I�u�W�F�N�g����
	bulletObject = Object3d::Create();
	if (bulletObject == nullptr) {
		return false;
	}

	//�傫�����Z�b�g
	bulletObject->SetScale({ 2, 2, 2 });

	//���f�����Z�b�g
	if (model) {
		bulletObject->SetModel(model);
	}

	return true;
}

void PlayerBullet::Update()
{
	//���˒��̒e�̂ݍX�V
	if (!isAlive) return;

	//�e�𓮂���
	Move();

	//�������ԃ^�C�}�[���X�V
	lifeTimer++;
	//�������ԃ^�C�}�[�������\���Ԃ܂œ��B������
	if(lifeTimer >= lifeTime)
	{
		//�e������
		Dead();
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

void PlayerBullet::BulletStart(XMFLOAT3 position, XMFLOAT3 rotation)
{
	//���ˈʒu�A�e�̊p�x�A���ˊp�x��ݒ�
	bulletObject->SetPosition(position);
	bulletObject->SetRotation(rotation);
	//���ˊp�x��ݒ肷�邽�߂Ɋp�x�����W�A���ɒ���
	this->angle = DirectX::XMConvertToRadians(rotation.z);
	//�e�̋�����������
	power = 10;
	//�e�̐������ԃ^�C�}�[�Ɛ����\���Ԃ�������
	lifeTimer = 0;
	lifeTime = 10;

	//���ˏ�Ԃɂ���
	isAlive = true;
}

void PlayerBullet::Dead()
{
	//�e���m���Ă�����̃��X�g���
	alreadyLines.clear();

	//�e�𔭎ˏ�Ԃł͂Ȃ�����
	isAlive = false;
}

void PlayerBullet::PowerUp()
{
	//�e�̈З͂���������
	power += 2;

	//�����\���Ԃ�L�΂�
	lifeTime += 10;
}

bool PlayerBullet::IsKnowLine(PowerUpLine *line)
{
	//�����̐������ɒm���Ă��邩�m�F
	for (auto itr = alreadyLines.begin(); itr != alreadyLines.end(); itr++)
	{
		//���ɒm���Ă�����true��Ԃ�
		if (line == (*itr))
		{
			return true;
		}
	}

	//�S�Ċm�F���Ă��m��Ȃ�������V���ɒǉ�����
	alreadyLines.push_front(line);

	//�m��Ȃ������ꍇ��false
	return false;
}

void PlayerBullet::Move()
{
	float moveSpeed = 3.0f;
	XMFLOAT3 pos = bulletObject->GetPosition();
	pos.x -= moveSpeed * sinf(angle);
	pos.y += moveSpeed * cosf(angle);
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
