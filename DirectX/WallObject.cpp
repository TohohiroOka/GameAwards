#include "WallObject.h"
#include "Easing.h"
#include "StageEffect.h"

using namespace DirectX;

const XMFLOAT2 WallObject::minPosition = { -204, -110 };
const XMFLOAT2 WallObject::maxPosition = { 204, 92 };
const float WallObject::transparentMaxTime = 30.0f;
bool WallObject::isSlow = false;

WallObject* WallObject::Create(Model* model)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	WallObject* instance = new WallObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

void WallObject::staticReset()
{
	isSlow = true;
}

void WallObject::WallMove()
{
	const float speed = 2.0f * (1.0f - slow);

	//���x��0�ȉ��Ȃ玟�ɍs��
	if (speed <= 0.2f)
	{
		time = 0;
		isSlow = false;
		slow = 0.0f;
		state = STATE::WAIT;
	}

	//���ֈړ�
	if (state == STATE::MOVE_UP_LEFT || state == STATE::MOVE_DOWN_LEFT)
	{
		position.x -= speed;
	}
	//���Ɉړ�
	else if (state == STATE::MOVE_RIGHT_DOWN || state == STATE::MOVE_LEFT_DOWN)
	{
		position.y -= speed / 1.1f;
	}
	//�E�Ɉړ�
	else if (state == STATE::MOVE_UP_RIGHT || state == STATE::MOVE_DOWN_RIGHT)
	{
		position.x += speed;
	}
	//��Ɉړ�
	else if (state == STATE::MOVE_RIGHT_UP || state == STATE::MOVE_LEFT_UP)
	{
		position.y += speed / 1.1f;
	}

	//�p�x�ύX
	rotation.x += 3;
	rotation.y += 5;
	rotation.z += 1;
	//�傫���ύX
	if (scale.x < 8.0f)
	{
		scale.x += 0.04f;
		scale.y += 0.04f;
		scale.z += 0.04f;
	}

	//����������
	if (isSlow)
	{
		slow += 0.1f;
	}
}

void WallObject::Transparency()
{
	//lerp%
	float progress = time / transparentMaxTime;

	color.w = Easing::Lerp(1, 0, progress);
	if (time > transparentMaxTime)
	{
		time = 0;
		scale.x = 5.0f;
		scale.y = 5.0f;
		scale.z = 5.0f;
		state = STATE::NONE;
	}
}

void WallObject::Fall()
{
	position.y -= moveSpeed.y;
	moveSpeed.y *= 1.03f;

	if (position.y < minPosition.y - 30.0f)
	{
		moveSpeed = { 0,0,0 };
		state = STATE::NONE;
	}
}

void WallObject::OutScreen()
{
	//�ړ����x�L�^
	if (oldState == STATE::WAIT)
	{
		if (position.x > maxPosition.x - 5)
		{
			moveSpeed.x = 1;
		} else if (position.x < minPosition.x + 5)
		{
			moveSpeed.x = -1;
		} else if (position.y > maxPosition.y - 5)
		{
			moveSpeed.y = 1;
		} else if (position.y < minPosition.y + 5)
		{
			moveSpeed.y = -1;
		}
	}

	position.x += moveSpeed.x;
	position.y += moveSpeed.y;
	if (position.x > maxPosition.x + 50 || position.y > maxPosition.y + 50 ||
		position.x < minPosition.x - 50 || position.y < minPosition.y - 50)
	{
		STATE state = STATE::NONE;
		moveSpeed = { 0,0,0 };
	}
}

bool WallObject::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	SetPosition({ 0,0,0 });
	SetScale({ 2,2,2 });

	return true;
}

void WallObject::Update()
{
	//�ǈړ�
	if (state >= STATE::MOVE_UP_LEFT && state <= STATE::MOVE_LEFT_UP)
	{
		WallMove();
	}
	//�����j��ɓ���������
	else if (state == STATE::TRANSPARENCY)
	{
		time++;
		Transparency();

		//particle
		StageEffect::SetWallBreak(position);
	}
	//����
	else if (state == STATE::FALL)
	{
		if (oldState == STATE::WAIT)
		{
			moveSpeed.y = 2.0f;
		}

		Fall();
	}
	//
	else if (state == STATE::OUT_SCREEN)
	{
		OutScreen();
	}

	Object3d::Update();

	//��t���[���O�̏�ԕێ�
	oldState = state;
}

void WallObject::Draw()
{
	if (state != STATE::NONE)
	{
		Object3d::Draw();
	}
}

void WallObject::Reset()
{
	//�I�u�W�F�N�g�̌��݂̏��
	state = STATE::NONE;
	//�I�u�W�F�N�g�̑O�̏��
	oldState = STATE::NONE;
	//�X�^�[�g���̉��o����
	time = 0;
	//��]�̉��o���Ɏg�p����p�x
	angle = 0;
	//������
	slow = 0.0f;
	//�������x
	moveSpeed = {};
	//Object3D�̏�����
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	color = { 1,1,1,1 };
}