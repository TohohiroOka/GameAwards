#include "WallObject.h"
#include "Easing.h"
#include "StageEffect.h"

using namespace DirectX;

const XMFLOAT3 WallObject::startPosition = { 0,0,500 };
float WallObject::directingMaxTime = 0;
const XMFLOAT2 WallObject::minPosition = { -196, -102 };
const XMFLOAT2 WallObject::maxPosition = { 196, 70 };
const float WallObject::initDistance = 50.0f;
const float WallObject::disperseMaxTime = 30.0f;
const float WallObject::transparentMaxTime = 30.0f;

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

void WallObject::Directing()
{
	//���W�A��
	float radius = XMConvertToRadians((float)angle);
	//lerp%
	float progress = time / directingMaxTime;

	distance = Easing::Lerp(initDistance, 0, progress);

	position.x = cos(radius) * distance + Easing::Lerp(0, easingPos.end.x, progress);
	position.y = sin(radius) * distance + Easing::Lerp(-55, easingPos.end.y, progress);
	position.z = Easing::Lerp(startPosition.z, 0, progress);

	//��ʒu�ւ̈ړ����I�������ǂ����
	if (time >= directingMaxTime)
	{
		//�ǈړ��ɏ�Ԃ�ύX
		//�E�Ɉړ�
		if (state == STATE::DIRECTING_LEFT_UP)
		{
			state = STATE::MOVE_UP;
		}
		//���Ɉړ�
		else if (state == STATE::DIRECTING_RIGHT_UP)
		{
			state = STATE::MOVE_RIGHT;
		}
		//���Ɉړ�
		else if (state == STATE::DIRECTING_RIGHT_DOWN)
		{
			state = STATE::MOVE_DOWN;
		}
		//��Ɉړ�
		else if (state == STATE::DIRECTING_LEFT_DOWN)
		{
			state = STATE::MOVE_LEFT;
		}
	}
}

void WallObject::WallMove()
{
	const float speed = 2.0f;
	//�E�Ɉړ�
	if (state == STATE::MOVE_UP)
	{
		position.x += speed;
		if (position.x >= maxPosition.x)
		{
			state = STATE::MOVE_RIGHT;
		}
	}
	//���Ɉړ�
	else if (state == STATE::MOVE_RIGHT)
	{
		position.y -= speed;
		if (position.y <= minPosition.y)
		{
			state = STATE::MOVE_DOWN;
		}
	}
	//���Ɉړ�
	else if (state == STATE::MOVE_DOWN)
	{
		position.x -= speed;
		if (position.x <= minPosition.x)
		{
			state = STATE::MOVE_LEFT;
		}
	}
	//��Ɉړ�
	else if (state == STATE::MOVE_LEFT)
	{
		position.y += speed;
		if (position.y >= maxPosition.y)
		{
			state = STATE::MOVE_UP;
		}
	}

	rotation.x += 3;
	rotation.y += 5;
	rotation.z += 1;
}

void WallObject::Transparency()
{
	//lerp%
	float progress = time / transparentMaxTime;

	color.w = Easing::Lerp(1, 0, progress);
	if (time > transparentMaxTime)
	{
		time = 0;
		state = STATE::NONE;
	}
}

bool WallObject::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	SetPosition({ 0,0,-100 });
	SetScale({ 2,2,2 });
	distance = initDistance;

	return true;
}

void WallObject::Update()
{
	//��ʒu�ւ̈ړ�
	if (state >= STATE::DIRECTING_LEFT_UP && state <= STATE::DIRECTING_LEFT_DOWN)
	{
		angle += 6;
		time++;
		Directing();
	}
	//�ǈړ�
	else if (state >= STATE::MOVE_UP && state <= STATE::MOVE_LEFT)
	{
		WallMove();
	}
	//�����j�󎞂̗���
	else if (state == STATE::TRANSPARENCY)
	{
		time++;
		Transparency();

		//particle
		StageEffect::SetWallBreak(position);
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
	//��]���̕�
	distance = 0;
	//�C�[�W���O�Ŏg�����W
	easingPos = {};
	//�C�[�W���O�Ŏg����]
	easingRota = {};
	//����΂ߎ��̈ړ�����
	disperseMovePos = {};
	//����΂ߎ��̉�]�p
	disperseMoveRota = {};
	//Object3D�̏�����
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	color = { 1,1,1,1 };
}