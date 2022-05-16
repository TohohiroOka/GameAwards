#include "StageEffect.h"
#include "SafeDelete.h"
#include "Camera.h"
#include <random>
#include <cstdlib>

using namespace DirectX;

Emitter* StageEffect::generalEffect = nullptr;
int StageEffect::playerMoveContro = 0;
const float CHANGE_RADIAN = 3.141592f / 180.0f;
const XMFLOAT3 NULL_NUMBER = { 0,0,0 };//0�����鎞�̕ϐ�
Emitter* StageEffect::wallEffect[StageEffect::wallTexNum];

/// <summary>
/// ��������
/// �͈�1����͈�2�܂ł̗������o��
/// </summary>
/// <param name="before">�͈�1</param>
/// <param name="after">�͈�2</param>
/// <returns>����</returns>
float Randomfloat(float before, float after)
{
	float range = 0;
	//�����͈͎̔w��
	//�������̒l�Ȃ�-
	if (before < 0 && after < 0)
	{
		range = abs(before) - abs(after);
	}
	//�������̒l�Ȃ�-
	else if (before > 0 && after > 0)
	{
		range = abs(after) - abs(before);
	}
	//����ȊO�Ȃ�+
	else {
		range = abs(before) + abs(after);
	}

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, (int)range);

	return (float)rand100(mt) - abs(before);
}

StageEffect::~StageEffect()
{
	safe_delete(generalEffect);
	for (int i = 0; i < wallTexNum; i++)
	{
		safe_delete(wallEffect[i]);
	}
}

void StageEffect::Initialize()
{
	ParticleManager::LoadTexture(0, L"Resources/particle/effect1.png");//�ėp�G�t�F�N�g
	ParticleManager::LoadTexture(1, L"Resources/particle/garakuta1.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(2, L"Resources/particle/garakuta2.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(3, L"Resources/particle/garakuta3.png");//�ǃI�u�W�F�N�g�n

	generalEffect = new Emitter();
	generalEffect->Create(0);
	generalEffect->SetBloom();

	for (int i = 0; i < wallTexNum; i++)
	{
		wallEffect[i] = new Emitter();
		wallEffect[i]->Create(1 + i);
	}
}

void StageEffect::SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation)
{
	if (playerMoveContro == 0)
	{
		//�o������
		const int maxFrame = 30;
		//�J�n�T�C�Y
		const XMFLOAT2 startSize = { 3.0f,3.0f };
		//�I���T�C�Y
		const XMFLOAT2 endSize = { 0.5f,0.5f };
		//�J�n�J���[
		const XMFLOAT4 startColor = { 0.0f,0.0f,0.9f,0.5f };
		//�I���J���[
		const XMFLOAT4 endColor = { 0.2f,0.5f,0.8f,0.5f };
		//���x
		XMFLOAT3 velocity = {};

		for (int i = 0; i < 5; i++)
		{
			//�G�t�F�N�g�̍��W���v���C���[�̌��ɂ���
			float radian = (rotation.z - 90) * CHANGE_RADIAN;
			//sin cos�̕ۑ�
			XMFLOAT2 moveAdd = { cosf(radian) ,sinf(radian) };
			//���x
			velocity.x = Randomfloat(-10, 10) / 100.0f;
			velocity.y = Randomfloat(-10, 10) / 100.0f;
			//���W
			XMFLOAT3 pos = position;
			pos.x += 5.0f * moveAdd.x;
			pos.y += 5.0f * moveAdd.y;
			pos.z += pos.z - 1;

			generalEffect->InEmitter(maxFrame, pos,
				velocity, NULL_NUMBER, startSize, endSize, startColor, endColor);
		}
	}

	playerMoveContro++;
	if (playerMoveContro > 3) { playerMoveContro = 0; }
}

void StageEffect::SetHitWall(const XMFLOAT3 position, const float angle)
{
	//�o������
	const int maxFrame = 50;
	//�J�n�T�C�Y
	const XMFLOAT2 size = { 5.0f,5.0f };
	//�J�n�J���[
	const XMFLOAT4 S_color = { 0.5f,0.5f,0.5f,1.0f };
	//�I���J���[
	const XMFLOAT4 E_color = { 0.0f,0.0f,0.0f,1.0f };
	//���W
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//���x
	XMFLOAT3 velocity = {};

	//��x�ɏo���
	const int MaxNum = 5;
	for (int i = 0; i < MaxNum; i++)
	{
		//���x�������_���łƂ�
		float inAngle = Randomfloat(angle - 15, angle + 15);
		float radius = DirectX::XMConvertToRadians(inAngle);
		velocity.x = cos(radius);
		velocity.y = sin(radius);

		int useTex = (int)Randomfloat(0.0f, wallTexNum - 1);

		wallEffect[useTex]->InEmitter(maxFrame, pos,
			velocity, NULL_NUMBER, size, size, S_color, E_color);
	}
}

void StageEffect::SetPushEnemy(const XMFLOAT3 position, const float radius, const XMFLOAT4 color)
{
	//�o������
	const int maxFrame = 30;
	//�J���[(�ω��Ȃ��̂��ߕϐ����)
	const XMFLOAT4 S_E_color = { color.x,color.y,color.z,0.5f };
	//�T�C�Y
	const XMFLOAT2 size = { 1.0f,1.0f };
	//���W
	const XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//���x
	XMFLOAT3 velocity = {};
	//�p�x
	float angle = 0;

	//��x�ɏo���
	const int MaxNum = 10;
	//��̉ΉԂɎg����
	const int oneEffectNum = 5;
	for (int i = 0; i < MaxNum; i++)
	{
		//���x
		float pushAngle = XMConvertToDegrees(radius);
		//���˂���p�x�̔��΂���
		pushAngle += 270.0f;
		//������ԃG�t�F�N�g�̊p�x
		const float pushRange = 45;
		//��`�ɂɂ��邽�߃����_���Ŋp�x��ݒ肷��
		pushAngle = Randomfloat(pushAngle - pushRange, pushAngle + pushRange);

		//���W�A���ɕϊ�
		float pushRadius = XMConvertToRadians((float)-pushAngle);

		velocity.x = cosf(pushRadius);
		velocity.y = sinf(pushRadius);
		XMFLOAT3 inPos = pos;

		for (int j = 0; j < oneEffectNum; j++)
		{
			inPos.x += velocity.x * j;
			inPos.y += velocity.y * j;

			generalEffect->InEmitter(maxFrame, inPos,
				velocity, NULL_NUMBER, size, size, S_E_color, S_E_color);
		}
	}
}

void StageEffect::SetWallBreak(const XMFLOAT3 position)
{
	//300�����Ă�����ǉ����Ȃ�
	int count = wallEffect[0]->GetCount() + wallEffect[1]->GetCount() + wallEffect[2]->GetCount();
	if (count > 300) { return; }

	//�o������
	const int maxFrame = 20;
	//�J���[(�ω��Ȃ��̂��ߕϐ����)
	const XMFLOAT4 S_E_color = { 1,1,1,1 };
	//�T�C�Y
	const XMFLOAT2 size = { 5.0f,5.0f };
	//���x
	XMFLOAT3 velocity = {};
	//�p�x
	float angle = 0;

	//��x�ɏo���
	const int MaxNum = 10;
	//��̉ΉԂɎg����
	const int oneEffectNum = 5;
	XMFLOAT3 pos = position;
	pos.x += Randomfloat(-5.0f, 5.0f);
	pos.y += Randomfloat(-5.0f, 5.0f);

	velocity.x = Randomfloat(-50.0f, 50.0f) / 300.0f;
	velocity.y = Randomfloat(-50.0f, 50.0f) / 300.0f;

	int useTex = (int)Randomfloat(0.0f, wallTexNum - 1);

	wallEffect[useTex]->InEmitter(maxFrame, pos,
		velocity, NULL_NUMBER, size, size, S_E_color, S_E_color);
}

void StageEffect::Update(Camera* camera)
{
	ParticleManager::SetCamera(camera);
	generalEffect->Update();
	for (int i = 0; i < wallTexNum; i++)
	{
		wallEffect[i]->Update();
	}
}

void StageEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	ParticleManager::PreDraw(cmdList);

	if (generalEffect->GetCount() != 0)
	{
		generalEffect->Draw();
	}
	for (int i = 0; i < wallTexNum; i++)
	{
		if (wallEffect[i]->GetCount() != 0)
		{
			wallEffect[i]->Draw();
		}
	}

	ParticleManager::PostDraw();
}