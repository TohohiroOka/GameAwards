#include "StageEffect.h"
#include "SafeDelete.h"
#include "Camera.h"
#include <random>
#include <cstdlib>

using namespace DirectX;

Emitter* StageEffect::generalEffect = nullptr;
int StageEffect::playerMoveContro = 0;
const XMFLOAT3 NULL_NUMBER = { 0,0,0 };//0�����鎞�̕ϐ�
Emitter* StageEffect::wallEffect[StageEffect::wallTexNum];

/// <summary>
/// ��������
/// 0����͈͂܂ł̗������o��
/// </summary>
/// <param name="after">�͈�</param>
/// <returns>����</returns>
float Randomfloat(int after)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(0, after);

	return (float)rand100(mt);
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
			float radian = XMConvertToRadians(rotation.z - 90);
			//sin cos�̕ۑ�
			XMFLOAT2 moveAdd = { cosf(radian) ,sinf(radian) };
			//���x
			velocity.x = (Randomfloat(20) - 10.0f) / 100.0f;
			velocity.y = (Randomfloat(20) - 10.0f) / 100.0f;
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
		float inAngle = Randomfloat(30) + angle;
		float radius = DirectX::XMConvertToRadians(inAngle);
		velocity.x = cos(radius);
		velocity.y = sin(radius);

		int useTex = (int)Randomfloat(wallTexNum - 1);

		wallEffect[useTex]->InEmitter(maxFrame, pos,
			velocity, NULL_NUMBER, size, size, S_color, E_color);
	}
}

void StageEffect::SetPushEnemy(const XMFLOAT3 position, const unsigned char power)
{
	//�o������
	const int maxFrame = 30;
	//�J�n�J���[
	XMFLOAT4 startColor = { 1,1,1,1 };
	startColor.y = 0.25f * power;
	startColor.z = 0.25f * power;

	//�I���J���[
	const XMFLOAT4 endColor = { 0,0,0,1 };
	//�T�C�Y
	const XMFLOAT2 size = { 1.0f,1.0f };
	//���W
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//���x
	XMFLOAT3 velocity = { 0,0,0 };

	//��x�ɏo���
	const int MaxNum = 3;
	for (int i = 0; i < MaxNum; i++)
	{
		//���x
		velocity.x = (Randomfloat(20) - 10.0f) / 100.0f;
		velocity.y = (Randomfloat(20) - 10.0f) / 100.0f;
		//���W
		pos.x += (Randomfloat(20) - 10.0f) / 100.0f;
		pos.y += (Randomfloat(20) - 10.0f) / 100.0f;

		generalEffect->InEmitter(maxFrame, pos,
			velocity, NULL_NUMBER, size, size, startColor, endColor);
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
	pos.x += Randomfloat(10) - 5.0f;
	pos.y += Randomfloat(10) - 5.0f;

	velocity.x = (Randomfloat(50) - 50.0f) / 300.0f;
	velocity.y = (Randomfloat(50) - 50.0f) / 300.0f;

	int useTex = (int)Randomfloat(wallTexNum - 1);

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