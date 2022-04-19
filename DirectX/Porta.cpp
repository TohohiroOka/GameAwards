#include "Porta.h"
#include "SafeDelete.h"

DirectX::XMFLOAT2 Porta::reflectionLine = { 97, 53 };

Porta* Porta::Create(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
{
	//�C���X�^���X�𐶐�
	Porta* instance = new Porta();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model, spawnPosition, moveDegree)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Porta::~Porta()
{
	safe_delete(enemyObject);
}

bool Porta::Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
{
	//�I�u�W�F�N�g����
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemyObject->SetPosition(spawnPosition);
	//�傫�����Z�b�g
	enemyObject->SetScale({ 2, 2, 1 });

	//�ړ��p�x���Z�b�g
	SetMoveAngle(moveDegree);

	//���f�����Z�b�g
	if (model) {
		enemyObject->SetModel(model);
	}

	//�F��Ԃ�����
	enemyObject->SetColor({ 1, 1, 1, 1 });

	return true;
}

void Porta::Update()
{
	//�ړ�����
	Move();

	//�G�����S���Ă�����폜��Ԃɂ���
	if (!isAlive)
	{
		SetDelete();
	}

	//�G�I�u�W�F�N�g�X�V
	enemyObject->Update();
}

void Porta::Draw()
{
	//�G�I�u�W�F�N�g�`��
	enemyObject->Draw();
}

void Porta::Damage(int damagePower)
{
	//�����Ŏw�肵�������̕�HP�����炷
	HP -= damagePower;

	//HP��0�ȉ��Ȃ�Ύ��S������
	if (HP <= 0)
	{
		Dead();
	}
}

void Porta::Dead()
{
	//���S��Ԃɂ���
	isAlive = false;
}

void Porta::SetDelete()
{
	//�폜����
	isDelete = true;
}

void Porta::Move()
{
	//�ړ����x�Ɉړ��p�x����Z���č��W���X�V
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();

	//�g���ɂ���ꍇ
	if (isInScreen)
	{
		//�g�ɓ������Ă����甽�˂�����
		/*if (pos.x <= -reflectionLine.x - size.x / 2) { vel = { }; }
		else if (pos.x >= reflectionLine.x + size.x / 2) { vel = { }; }
		if (pos.y <= -reflectionLine.y - size.y / 2) { vel = { }; }
		else if (pos.y >= reflectionLine.y + size.y / 2) { vel = { }; }*/
		if (pos.x <= -reflectionLine.x - size.x / 2) { ReflectionX(); }
		else if (pos.x >= reflectionLine.x + size.x / 2) { ReflectionX(); }
		if (pos.y <= -reflectionLine.y - size.y / 2) { ReflectionY(); }
		else if (pos.y >= reflectionLine.y + size.y / 2) { ReflectionY(); }
	}
	//�g���ɂ��Ȃ��ꍇ
	else
	{
		//�g���ɓ��������`�F�b�N����
		if (pos.x > -reflectionLine.x - size.x / 2 && pos.x < reflectionLine.x + size.x / 2 &&
			pos.y > -reflectionLine.y - size.y / 2 && pos.y < reflectionLine.y + size.y / 2)
		{
			isInScreen = true;
		}
	}

	//�ړ��ʂ����W�ɉ��Z���Ĉړ�������
	pos.x += vel.x;
	pos.y += vel.y;

	//�X�V�������W���Z�b�g
	enemyObject->SetPosition(pos);
}

void Porta::SetMoveAngle(float moveDegree)
{
	//�������0�ɂ��邽��90�x�X����
	this->moveDegree = moveDegree;

	//�x�������W�A���ɒ���
	moveAngle = DirectX::XMConvertToRadians(this->moveDegree + 90);

	//�I�u�W�F�N�g�̌�����i�s�����ɃZ�b�g
	XMFLOAT3 rota = { 0, 0, this->moveDegree };
	enemyObject->SetRotation(rota);

	//�ړ��ʂ��Z�b�g
	vel.x = moveSpeed * cosf(moveAngle);
	vel.y = moveSpeed * sinf(moveAngle);
}

void Porta::ReflectionX()
{
	//�X�s�[�h�A�b�v
	SpeedUp();

	//���E���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 360 - moveDegree;
	SetMoveAngle(reflectionAngle);
}

void Porta::ReflectionY()
{
	//�X�s�[�h�A�b�v
	SpeedUp();

	//�㉺���˗p�ɔ��ˊp���Z�b�g����
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);
}

void Porta::SpeedUp()
{
	//����ɒB���Ă��Ȃ��ꍇ���x���グ��
	const float maxSpeed = 1.1f;
	const float speedUp = 0.2f;
	if (moveSpeed < maxSpeed)
	{
		moveSpeed += speedUp;
	}
}
