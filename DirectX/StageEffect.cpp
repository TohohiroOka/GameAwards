#include "StageEffect.h"
#include "SafeDelete.h"
#include "Camera.h"
#include <random>
#include <cstdlib>

using namespace DirectX;

Emitter* StageEffect::generalEffect = nullptr;
int StageEffect::playerMoveControl = 0;
const XMFLOAT3 NULL_NUMBER = { 0,0,0 };//0�����鎞�̕ϐ�
std::array<Emitter*, StageEffect::wallTexNum> StageEffect::wallEffect;
//Emitter* StageEffect::smash = nullptr;
//std::forward_list<StageEffect::SMASH> StageEffect::smashInfo;
Emitter* StageEffect::pushEnemy = nullptr;
Emitter* StageEffect::heal = nullptr;
int StageEffect::healControl = 0;
Emitter* StageEffect::pop = nullptr;
int StageEffect::healFieldControl = 0;

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
	safe_delete(pushEnemy);
	safe_delete(heal);
	safe_delete(pop);
}

void StageEffect::Initialize()
{
	ParticleManager::LoadTexture(0, L"Resources/particle/effect1.png");//�ėp�G�t�F�N�g
	ParticleManager::LoadTexture(1, L"Resources/particle/garakuta1.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(2, L"Resources/particle/garakuta2.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(3, L"Resources/particle/garakuta3.png");//�ǃI�u�W�F�N�g�n
	ParticleManager::LoadTexture(4, L"Resources/particle/star.png");//�G��e�������̃G�t�F�N�g
	ParticleManager::LoadTexture(5, L"Resources/particle/heal.png");//�񕜃G�t�F�N�g
	ParticleManager::LoadTexture(6, L"Resources/particle/pop.png");//�e����G�t�F�N�g

	generalEffect = new Emitter();
	generalEffect->Create(0);
	generalEffect->SetBloom();

	for (int i = 0; i < wallTexNum; i++)
	{
		wallEffect[i] = new Emitter();
		wallEffect[i]->Create(1 + i);
	}

	pushEnemy = new Emitter();
	pushEnemy->Create(4);

	heal = new Emitter();
	heal->Create(5);

	pop = new Emitter();
	pop->Create(6);
}

void StageEffect::SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation)
{
	if (playerMoveControl == 0)
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

	playerMoveControl++;
	if (playerMoveControl > 3) { playerMoveControl = 0; }
}

void StageEffect::SetPushEnemy(const XMFLOAT3 position)
{
	//�o������
	const int maxFrame = 30;
	//�J�n�J���[
	XMFLOAT4 startColor = { 0.9f,0.15f,0.15f,1.0f };

	//�I���J���[
	const XMFLOAT4 endColor = { 0.0f,0.0f,0.0f,1.0f };
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

void StageEffect::SetPushEnemyPower(const XMFLOAT3 position, const unsigned char power)
{
	//�o������
	const int maxFrame = 30;
	//�J�n�T�C�Y
	float numPower = (float)power * 2.0f;
	const XMFLOAT2 startSize = { 7.0f * numPower ,7.0f * numPower };
	//�I���T�C�Y
	const XMFLOAT2 endSize = { 0.0f,0.0f };
	//�J�n�J���[
	XMFLOAT4 startColor = { 1,1,1,1 };
	//�I���J���[
	const XMFLOAT4 endColor = { 0.0f,0.0f,0.0f,1.0f };
	//���W
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//���x
	XMFLOAT3 velocity = { 0,0,0 };

	//���x
	velocity.x = (Randomfloat(20) - 10.0f) / 100.0f;
	velocity.y = (Randomfloat(20) - 10.0f) / 100.0f;

	pushEnemy->InEmitter(maxFrame, pos,
		velocity, NULL_NUMBER, startSize, endSize, startColor, endColor);
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

	velocity.x = (Randomfloat(100) - 50.0f) / 300.0f;
	velocity.y = (Randomfloat(100) - 50.0f) / 300.0f;

	int useTex = (int)Randomfloat(wallTexNum - 1);

	wallEffect[useTex]->InEmitter(maxFrame, pos,
		velocity, NULL_NUMBER, size, size, S_E_color, S_E_color);
}

void StageEffect::SetWallHitEnemey(const XMFLOAT3 position, const unsigned char direction)
{
	//�o������
	const int maxFrame = 30;
	//�J�n�J���[
	const XMFLOAT4 startColor = { 0.9f,0.25f,0.0f,1.0f };
	//�I���J���[
	const XMFLOAT4 endColor = { 0.0f,0.0f,0.0f,1.0f };
	//�I���T�C�Y
	const XMFLOAT2 size = { 20.0f,20.0f };
	//���W
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//���x
	XMFLOAT3 velocity = { 0,0,0 };
	//�ǂ̕����ɂ��p�x
	float angle = {};
	//��
	if (direction == 1)
	{
		angle = 235;
	}
	//��
	else if (direction == 2)
	{
		angle = -35;
	}
	//��
	else if (direction == 3)
	{
		angle = 55;
	}
	//�E
	else if (direction == 4)
	{
		angle = 145;
	}

	//��x�ɏo���
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		float randAngle = Randomfloat(70) + angle;
		//���x
		velocity.x = cosf(XMConvertToRadians(randAngle)) * 1.01f;
		velocity.y = sinf(XMConvertToRadians(randAngle)) * 1.01f;

		pop->InEmitter(maxFrame, pos, velocity, NULL_NUMBER,
			size, size, startColor, endColor, { 0,0, randAngle });
	}
}

void StageEffect::SetDeleteEnemey(const XMFLOAT3 position)
{
	//�o������
	const int maxFrame = 100;
	//�J�n�J���[
	const XMFLOAT4 startColor = { 1.0f,0.25f,0.25f,1.0f };
	//�I���J���[
	const XMFLOAT4 endColor = { 0.0f,0.0f,0.0f,1.0f };
	//�J�n�T�C�Y
	const XMFLOAT2 startSize = { 3.0f,3.0f };
	//�I���T�C�Y
	const XMFLOAT2 endSize = { 0.0f,0.0f };
	//���W
	const XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//���x
	XMFLOAT3 velocity = { 0,0,0 };
	//��x�ɏo���
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		velocity.x = (Randomfloat(100) - 50.0f) / 800.0f;
		velocity.y = (Randomfloat(100) - 50.0f) / 800.0f;

		XMFLOAT3 setpos = pos;
		setpos.x += (Randomfloat(20) - 10.0f) / 100.0f;
		setpos.y += (Randomfloat(20) - 10.0f) / 100.0f;

		generalEffect->InEmitter(maxFrame, setpos, velocity, NULL_NUMBER,
			startSize, endSize, startColor, endColor);
	}
}

void StageEffect::SetHealField(const XMFLOAT3 position)
{
	healFieldControl++;
	if (healFieldControl > 100) { healFieldControl = 0; }
	if (healFieldControl != 1) { return; }

	//�o������
	const int maxFrame = 200;
	//�J�n�T�C�Y
	const XMFLOAT2 startSize = { 1.0f,1.0f };
	//�I���T�C�Y
	const XMFLOAT2 endSize = { 0.0f,0.0f };
	//�J�n�J���[
	const XMFLOAT4 color = { 0.15f,0.85f,0.15f,0.5f };
	//���W
	const XMFLOAT3 pos = { position.x,position.y,position.z - 1 };

	//��x�ɏo���
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		float radian = XMConvertToRadians(Randomfloat(360));
		float radius = Randomfloat(35);
		XMFLOAT3 setpos = pos;
		setpos.x += cosf(radian) * radius;
		setpos.y += sinf(radian) * radius;

		generalEffect->InEmitter(maxFrame, setpos,
			NULL_NUMBER, NULL_NUMBER, startSize, endSize, color, color);
	}
}

void StageEffect::SetHeal(const XMFLOAT3 position)
{
	if (healControl == 0)
	{
		//�o������
		const int maxFrame = 50;
		//�J�n�J���[
		const XMFLOAT4 color = { 0.2f,0.9f,0.2f,1.0f };
		//�J�n�T�C�Y
		const XMFLOAT2 size = { 7.0f,7.0f };
		//���W
		XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
		//���x
		XMFLOAT3 velocity = { 0,0.2f,0 };

		pos.x += (Randomfloat(100) - 50.0f) / 20.0f;
		pos.y += (Randomfloat(100) - 50.0f) / 50.0f;

		heal->InEmitter(maxFrame, pos,
			velocity, NULL_NUMBER, size, size, color, color);
	}

	healControl++;
	if (healControl > 20) { healControl = 0; }
}

void StageEffect::Update(Camera* camera)
{
	ParticleManager::SetCamera(camera);
	generalEffect->Update();
	for (int i = 0; i < wallTexNum; i++)
	{
		wallEffect[i]->Update();
	}
	pushEnemy->Update();
	heal->Update();
	pop->Update();
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
	if (pushEnemy->GetCount() != 0)
	{
		pushEnemy->Draw();
	}
	if (heal->GetCount() != 0)
	{
		heal->Draw();
	}
	if (pop->GetCount() != 0)
	{
		pop->Draw();
	}
	ParticleManager::PostDraw();
}