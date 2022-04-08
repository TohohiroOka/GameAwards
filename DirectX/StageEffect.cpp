#include "StageEffect.h"
#include "SafeDelete.h"
#include "Camera.h"

using namespace DirectX;

#define PLAYER_MOVE_RAND_VELOCITY (float)((rand()%20)-10)/100.0f
#define ENEMEY_DEAD_RAND_VELOCITY (float)((rand()%400)-200)/100.0f
#define BULLET_DELETE_RAND_VELOCITY (float)((rand()%200)-100)/1000.0f

Emitter* StageEffect::playerMove = nullptr;
int StageEffect::playerMoveContro = 0;
Emitter* StageEffect::enemeyDead = nullptr;
Emitter* StageEffect::playerBulletDelete = nullptr;
Emitter* StageEffect::connectLine = nullptr;

const float CHANGE_RADIAN = 3.141592f / 180.0f;
const XMFLOAT3 NULL_NUMBER = { 0,0,0 };//0を入れる時の変数

StageEffect::~StageEffect()
{
	safe_delete(playerMove);
	safe_delete(enemeyDead);
	safe_delete(playerBulletDelete);
	safe_delete(connectLine);
}

void StageEffect::Initialize()
{
	ParticleManager::LoadTexture(0, L"Resources/particle/effect1.png");//汎用エフェクト
	ParticleManager::LoadTexture(1, L"Resources/particle/effect2.png");//線が繋がった時のエフェクト

	playerMove = new Emitter();
	playerMove->Create(0);

	enemeyDead = new Emitter();
	enemeyDead->Create(0);

	playerBulletDelete = new Emitter();
	playerBulletDelete->Create(0);

	connectLine = new Emitter();
	connectLine->Create(1);
}

void StageEffect::SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation)
{
	if (playerMoveContro == 0)
	{
		//最大個数
		const int maxParticlNum = 50;
		//出現時間
		const int maxFrame = 30;
		//開始サイズ
		const float startSize = 3.0f;
		//終了サイズ
		const float endSize = 0.5f;
		//開始カラー
		const XMFLOAT4 startColor = { 1,1,1,0.5f };
		//終了カラー
		const XMFLOAT4 endColor = { 1,1,1,0.5f };

		for (int i = 0; i < 5; i++)
		{
			//エフェクトの座標をプレイヤーの後ろにする
			float radian = (rotation.z - 90) * CHANGE_RADIAN;
			//sin cosの保存
			XMFLOAT2 moveAdd = { cosf(radian) ,sinf(radian) };
			//速度
			XMFLOAT3 velocity = { PLAYER_MOVE_RAND_VELOCITY,PLAYER_MOVE_RAND_VELOCITY,0 };
			//座標
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

int StageEffect::SetEnemeyDead(const XMFLOAT3 position)
{
	//最大個数
	const int maxParticlNum = 100;
	//出現時間
	const int maxFrame = 50;
	//開始サイズ
	const float startSize = 2.0f;
	//終了サイズ
	const float endSize = 2.0f;
	//開始カラー
	const XMFLOAT4 startColor = { 1,1,1,0.5f };
	//終了カラー
	const XMFLOAT4 endColor = { 1,1,1,0.5f };
	//座標
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };

	//一度に出る個数
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		//速度
		XMFLOAT3 velocity = { ENEMEY_DEAD_RAND_VELOCITY,ENEMEY_DEAD_RAND_VELOCITY,0 };
		//加速度
		XMFLOAT3 accel = { -(velocity.x * 2) / maxFrame,
			-(velocity.y * 2) / maxFrame,0 };

		enemeyDead->InEmitter(maxParticlNum, maxFrame, pos,
			velocity, accel, startSize, endSize, startColor, endColor);
	}

	return maxFrame;
}

void StageEffect::SetPlayerBulletDelete(const XMFLOAT3 position)
{
	//最大個数
	const int maxParticlNum = 100;
	//出現時間
	const int maxFrame = 30;
	//開始サイズ
	const float startSize = 5.0f;
	//終了サイズ
	const float endSize = 1.0f;
	//開始カラー
	const XMFLOAT4 startColor = { 1,1,1,0.5f };
	//終了カラー
	const XMFLOAT4 endColor = { 1,1,1,0.5f };
	//座標
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };

	//一度に出る個数
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		//速度
		XMFLOAT3 velocity = { BULLET_DELETE_RAND_VELOCITY,BULLET_DELETE_RAND_VELOCITY,0 };

		playerBulletDelete->InEmitter(maxParticlNum, maxFrame, pos,
			velocity, NULL_NUMBER, startSize, endSize, startColor, endColor);
	}
}

void StageEffect::SetConnectLine(const XMFLOAT3 position_one, const XMFLOAT3 position_two)
{
	//最大個数
	const int maxParticlNum = 50;
	//出現時間
	const int maxFrame = 90;
	//開始サイズ
	const float startSize = 5.0f;
	//終了サイズ
	const float endSize = 50.0f;
	//開始カラー
	const XMFLOAT4 startColor = { 1,1,1,0.5f };
	//終了カラー
	const XMFLOAT4 endColor = { 1,1,1,0.5f };

	connectLine->InEmitter(maxParticlNum, maxFrame, { position_one.x,position_one.y,position_one.z - 1 },
		NULL_NUMBER, NULL_NUMBER, startSize, endSize, startColor, endColor);
	connectLine->InEmitter(maxFrame, maxFrame, { position_two.x,position_two.y,position_two.z - 1 },
		NULL_NUMBER, NULL_NUMBER, startSize, endSize, startColor, endColor);
}

void StageEffect::Update(Camera* camera)
{
	playerMove->Update(camera);
	enemeyDead->Update(camera);
	playerBulletDelete->Update(camera);
	connectLine->Update(camera);
}

void StageEffect::Draw(ID3D12GraphicsCommandList* cmdList)
{
	ParticleManager::PreDraw(cmdList);

	if (playerMove->GetCount() != 0)
	{
		playerMove->Draw();
	}
	if (enemeyDead->GetCount() != 0)
	{
		enemeyDead->Draw();
	}
	if (playerBulletDelete->GetCount() != 0)
	{
		playerBulletDelete->Draw();
	}
	if (connectLine->GetCount() != 0)
	{
		connectLine->Draw();
	}

	ParticleManager::PostDraw();
}