#include "PlayerBullet.h"
#include "SafeDelete.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 PlayerBullet::deadPos = { 110, 60 };

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
	//�e���m���Ă�����̃��X�g���
	alreadyLines.clear();

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

void PlayerBullet::BulletStart(XMFLOAT3 position, XMFLOAT3 rotation, int power)
{
	//���ˈʒu�A�e�̊p�x�A���ˊp�x��ݒ�
	bulletObject->SetPosition(position);
	bulletObject->SetRotation(rotation);
	//���ˊp�x��ݒ肷�邽�߂Ɋp�x�����W�A���ɒ���
	this->angle = DirectX::XMConvertToRadians(rotation.z);

	//�e�̈З͂��Z�b�g
	this->power = power;
	if (power == 10) { bulletObject->SetColor({ 0.2f, 0.2f, 0.9f, 1 }); }
	else if (power == 20) { bulletObject->SetColor({ 0.2f, 0.9f, 0.9f, 1 }); }
	else if (power == 30) { bulletObject->SetColor({ 0.2f, 0.9f, 0.2f, 1 }); }
	else if (power == 40) { bulletObject->SetColor({ 0.9f, 0.9f, 0.2f, 1 }); }
	else if (power == 50) { bulletObject->SetColor({ 0.9f, 0.2f, 0.2f, 1 }); }
	else if (power == 60) { bulletObject->SetColor({ 0.9f, 0.9f, 0.9f, 1 }); }

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

bool PlayerBullet::IsKnowLine(PowerUpLine* line)
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

	//��ʊO�܂Ői�񂾂�
	XMFLOAT3 size = bulletObject->GetScale();
	if (pos.x <= -deadPos.x || pos.y <= -deadPos.y || pos.x >= deadPos.x || pos.y >= deadPos.y)
	{
		//�e������
		Dead();
	}
}
