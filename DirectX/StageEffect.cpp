#include "StageEffect.h"
#include "SafeDelete.h"
#include "Camera.h"
#include <random>
#include <cstdlib>

using namespace DirectX;

Emitter* StageEffect::titleCoreExplosion = nullptr;
int StageEffect::explosionTime = 0;
Emitter* StageEffect::playerMove = nullptr;
int StageEffect::playerMoveContro = 0;
Emitter* StageEffect::hitWall = nullptr;
Emitter* StageEffect::playerBulletDelete = nullptr;
Emitter* StageEffect::connectLine = nullptr;
const float CHANGE_RADIAN = 3.141592f / 180.0f;
const XMFLOAT3 NULL_NUMBER = { 0,0,0 };//0�����鎞�̕ϐ�
Emitter* StageEffect::pushEnemy = nullptr;
Emitter* StageEffect::wallBreak[StageEffect::wallTexNum];

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
	safe_delete(titleCoreExplosion);
	safe_delete(playerMove);
	safe_delete(hitWall);
	safe_delete(playerBulletDelete);
	safe_delete(connectLine);
	safe_delete(pushEnemy);
	for (int i = 0; i < wallTexNum; i++)
	{
		safe_delete(wallBreak[i]);
	}
}

void StageEffect::Initialize()
{
	ParticleManager::LoadTexture(0, L"Resources/particle/effect1.png");//�ėp�G�t�F�N�g
	ParticleManager::LoadTexture(1, L"Resources/particle/effect2.png");//�����q���������̃G�t�F�N�g
	ParticleManager::LoadTexture(2, L"Resources/particle/garakuta1.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(3, L"Resources/particle/garakuta2.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(4, L"Resources/particle/garakuta3.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(5, L"Resources/particle/garakuta4.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(6, L"Resources/particle/garakuta5.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(7, L"Resources/particle/garakuta6.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(8, L"Resources/particle/garakuta7.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(9, L"Resources/particle/garakuta8.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(10, L"Resources/particle/garakuta9.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(11, L"Resources/particle/garakuta10.png");//�ǃI�u�W�F�N�g�n

	titleCoreExplosion = new Emitter();
	titleCoreExplosion->Create(0);

	playerMove = new Emitter();
	playerMove->Create(2);

	hitWall = new Emitter();
	hitWall->Create(2);

	playerBulletDelete = new Emitter();
	playerBulletDelete->Create(0);

	connectLine = new Emitter();
	connectLine->Create(1);

	pushEnemy = new Emitter();
	pushEnemy->Create(0);

	for (int i = 0; i < wallTexNum; i++)
	{
		wallBreak[i] = new Emitter();
		wallBreak[i]->Create(2 + i);
	}
}

float StageEffect::SetTitleCoreExplosion(const XMFLOAT3 position)
{
	//�o������
	const int maxFrame = 30;

	//���Ԃ�i�߂�
	explosionTime++;

	//return�l��1�ɂȂ������ɃG�t�F�N�g���S�ď�����^�C�~���O�ɍ����悤�G�t�F�N�g���o��
	if (explosionTime <= explosionTimeMax - maxFrame)
	{
		//�ő��
		const int maxParticlNum = 300;
		//�J�n�T�C�Y
		const XMFLOAT2 startSize = { 3.0f,3.0f };
		//�I���T�C�Y
		const XMFLOAT2 endSize = { 0.5f ,0.5 };
		//�J�n�J���[
		const XMFLOAT4 startColor = { 0.9f,0.0f,0.0f,0.5f };
		//�I���J���[
		const XMFLOAT4 endColor = { 0.9f,0.5f,0.0f,0.5f };
		//�T�t���[���ɏo�鐔
		const int maxNum = 10;
		//���x
		XMFLOAT3 velocity = {};

		//�T�t���[������������
		for (int i = 0; i < maxNum; i++)
		{
			//���x�������_���łƂ�
			velocity.x = Randomfloat(-400, 400) / 100.0f;
			velocity.y = Randomfloat(-400, 400) / 100.0f;

			titleCoreExplosion->InEmitter(maxParticlNum, maxFrame, position,
				velocity, NULL_NUMBER, startSize, endSize, startColor, endColor);
		}
	}


	//���Ԃ�ۑ�
	int time = explosionTime;

	//explosionTimeMax�Ɠ����Ȃ�explosionTime������������
	if (explosionTime >= explosionTimeMax)
	{
		explosionTime = 0;
	}

	return (float)time / explosionTimeMax;
}

void StageEffect::SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation)
{
	if (playerMoveContro == 0)
	{
		//�ő��
		const int maxParticlNum = 50;
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

			playerMove->InEmitter(maxParticlNum, maxFrame, pos,
				velocity, NULL_NUMBER, startSize, endSize, startColor, endColor);
		}
	}

	playerMoveContro++;
	if (playerMoveContro > 3) { playerMoveContro = 0; }
}

void StageEffect::SetHitWall(const XMFLOAT3 position, const float angle)
{
	//�ő��
	const int maxParticlNum = 100;
	//�o������
	const int maxFrame = 50;
	//�J�n�T�C�Y
	const XMFLOAT2 size = { 5.0f,5.0f };
	//�J�n�J���[
	const XMFLOAT4 color = { 1.0f,1.0f,1.0f,1.0f };
	//���W
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//���x
	XMFLOAT3 velocity = {};

	//��x�ɏo���
	const int MaxNum = 3;
	for (int i = 0; i < MaxNum; i++)
	{
		//���x�������_���łƂ�
		float inAngle = Randomfloat(angle - 15, angle + 15);
		float radius = DirectX::XMConvertToRadians(inAngle);
		velocity.x = cos(radius);
		velocity.y = sin(radius);

		hitWall->InEmitter(maxParticlNum, maxFrame, pos,
			velocity, NULL_NUMBER, size, size, color, color);
	}
}

void StageEffect::SetPlayerBulletDelete(const XMFLOAT3 position, const XMFLOAT4 color)
{
	//�ő��
	const int maxParticlNum = 100;
	//�o������
	const int maxFrame = 30;
	//�J�n�T�C�Y
	const XMFLOAT2 startSize = { 5.0f,5.0f };
	//�I���T�C�Y
	const XMFLOAT2 endSize = { 1.0f,1.0f };
	//�J���[(�ω��Ȃ��̂��ߕϐ����)
	const XMFLOAT4 S_E_color = { color.x,color.y,color.z,0.5f };
	//���W
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//���x
	XMFLOAT3 velocity = {};

	//��x�ɏo���
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		//���x
		velocity.x = Randomfloat(-100, 100) / 1000.0f;

		playerBulletDelete->InEmitter(maxParticlNum, maxFrame, pos,
			velocity, NULL_NUMBER, startSize, endSize, S_E_color, S_E_color);
	}
}

void StageEffect::SetConnectLine(const XMFLOAT3 position_one, const XMFLOAT3 position_two)
{
	//�ő��
	const int maxParticlNum = 50;
	//�o������
	const int maxFrame = 40;
	//�J�n�T�C�Y
	const XMFLOAT2 startSize = { 5.0f,5.0f };
	//�I���T�C�Y
	const XMFLOAT2 endSize = { 300.0f, 300.0f };
	//�J�n�J���[
	const XMFLOAT4 startColor = { 1,1,1,0.5f };
	//�I���J���[
	const XMFLOAT4 endColor = { 0,0,1,0.5f };

	connectLine->InEmitter(maxParticlNum, maxFrame, { position_one.x,position_one.y,position_one.z - 1 },
		NULL_NUMBER, NULL_NUMBER, startSize, endSize, startColor, endColor);
	connectLine->InEmitter(maxFrame, maxFrame, { position_two.x,position_two.y,position_two.z - 1 },
		NULL_NUMBER, NULL_NUMBER, startSize, endSize, startColor, endColor);
}

void StageEffect::SetPushEnemy(const XMFLOAT3 position, const float radius, const XMFLOAT4 color)
{
	//�ő��
	const int maxParticlNum = 400;
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

			pushEnemy->InEmitter(maxParticlNum, maxFrame, inPos,
				velocity, NULL_NUMBER, size, size, S_E_color, S_E_color);
		}
	}
}

void StageEffect::SetWallBreak(const XMFLOAT3 position)
{
	//�ő��
	const int maxParticlNum = 350;
	//�o������
	const int maxFrame = 30;
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

	int useTex = (int)Randomfloat(0.0f, 9.0f);

	wallBreak[0]->InEmitter(maxParticlNum, maxFrame, pos,
		velocity, NULL_NUMBER, size, size, S_E_color, S_E_color);
}

void StageEffect::Update(Camera* camera)
{
	ParticleManager::SetCamera(camera);
	titleCoreExplosion->Update();
	playerMove->Update();
	hitWall->Update();
	playerBulletDelete->Update();
	connectLine->Update();
	pushEnemy->Update();
	for (int i = 0; i < wallTexNum; i++)
	{
		wallBreak[i]->Update();
	}
}

void StageEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	ParticleManager::PreDraw(cmdList);

	if (titleCoreExplosion->GetCount() != 0)
	{
		titleCoreExplosion->Draw();
	}
	if (playerMove->GetCount() != 0)
	{
		playerMove->Draw();
	}
	if (hitWall->GetCount() != 0)
	{
		hitWall->Draw();
	}
	if (playerBulletDelete->GetCount() != 0)
	{
		playerBulletDelete->Draw();
	}
	if (connectLine->GetCount() != 0)
	{
		connectLine->Draw();
	}
	if (pushEnemy->GetCount() != 0)
	{
		pushEnemy->Draw();
	}
	for (int i = 0; i < wallTexNum; i++)
	{
		if (wallBreak[i]->GetCount() != 0)
		{
			wallBreak[i]->Draw();
		}
	}

	ParticleManager::PostDraw();
}