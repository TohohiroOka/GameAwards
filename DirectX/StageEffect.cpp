#include "StageEffect.h"
#include "SafeDelete.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "Player.h"

using namespace DirectX;

#define PLAYER_MOVE_RAND_VELOCITY (float)((rand()%20)-10)/100.0f
#define ENEMEY_DEAD1_RAND_VELOCITY (float)((rand()%400)-200)/100.0f

const float CHANGE_RADIAN = 3.141592f / 180.0f;

const XMFLOAT3 nullNunber = { 0,0,0 };//0を入れる時の変数

StageEffect::~StageEffect()
{
	safe_delete(playerMove);
	safe_delete(enemeyDead1);
}

void StageEffect::Initialize()
{
	ParticleManager::LoadTexture(0, L"Resources/particle/effect1.png");

	playerMove = new Emitter();
	playerMove->Create(0);

	enemeyDead1 = new Emitter();
	enemeyDead1->Create(0);
}

void StageEffect::SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation)
{
	if (playerMoveContro == 0)
	{
		for (int i = 0; i < 5; i++)
		{
			//エフェクトの座標をプレイヤーの後ろにする
			float radian = (rotation.z - 90) * CHANGE_RADIAN;
			//sin cosの保存
			XMFLOAT2 moveAdd = { cosf(radian) ,sinf(radian) };
			//最大個数
			const int MAX_PARTICLE_PLAYER_MOVE = 50;
			//出現時間
			const int MAX_FRAME_PLAYER_MOVE = 30;
			//速度
			XMFLOAT3 velocity = { PLAYER_MOVE_RAND_VELOCITY,PLAYER_MOVE_RAND_VELOCITY,0 };
			//開始サイズ
			const float startSize = 3.0f;
			//終了サイズ
			const float endSize = 0.5f;
			//開始サイズ
			const XMFLOAT4 startColor = { 1,1,1,0.5f };
			//終了サイズ
			const XMFLOAT4 endColor = { 1,1,1,0.5f };

			//位置の差
			const int range = 5;
			//座標
			XMFLOAT3 pos = position;
			pos.x += 5.0f * moveAdd.x;
			pos.y += 5.0f * moveAdd.y;

			playerMove->InEmitter(MAX_PARTICLE_PLAYER_MOVE, MAX_FRAME_PLAYER_MOVE, pos,
				velocity, nullNunber, startSize, endSize, startColor, endColor);
		}
	}

	playerMoveContro++;
	if (playerMoveContro > 3) { playerMoveContro = 0; }
}

int StageEffect::SetEnemeyDead1(const XMFLOAT3 position)
{
	//最大個数
	const int MAX_PARTICLE_ENEMEY_DEAD1 = 100;
	//出現時間
	const int MAX_FRAME_ENEMEY_DEAD1 = 50;
	//開始サイズ
	const float startSize = 2.0f;
	//終了サイズ
	const float endSize = 2.0f;
	//開始サイズ
	const XMFLOAT4 startColor = { 1,1,1,0.5f };
	//終了サイズ
	const XMFLOAT4 endColor = { 1,1,1,0.5f };

	//一度に出る個数
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		//速度
		XMFLOAT3 velocity = { ENEMEY_DEAD1_RAND_VELOCITY,ENEMEY_DEAD1_RAND_VELOCITY,0 };
		//加速度
		XMFLOAT3 accel = { -(velocity.x * 2) / MAX_FRAME_ENEMEY_DEAD1,
			-(velocity.y * 2) / MAX_FRAME_ENEMEY_DEAD1,0 };

		enemeyDead1->InEmitter(MAX_PARTICLE_ENEMEY_DEAD1, MAX_FRAME_ENEMEY_DEAD1, position,
			velocity, accel, startSize, endSize, startColor, endColor);
	}

	return MAX_FRAME_ENEMEY_DEAD1;
}

void StageEffect::Update(Camera* camera)
{
	playerMove->Update(camera);
	enemeyDead1->Update(camera);
}

void StageEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	ParticleManager::PreDraw(cmdList);

	if (playerMove->GetCount() != 0)
	{
		playerMove->Draw();
	}
	if (enemeyDead1->GetCount() != 0)
	{
		enemeyDead1->Draw();
	}

	ParticleManager::PostDraw();
}