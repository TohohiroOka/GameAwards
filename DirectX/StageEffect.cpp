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
Emitter* StageEffect::enemeyDead = nullptr;
Emitter* StageEffect::playerBulletDelete = nullptr;
Emitter* StageEffect::connectLine = nullptr;
const float CHANGE_RADIAN = 3.141592f / 180.0f;
const XMFLOAT3 NULL_NUMBER = { 0,0,0 };//0を入れる時の変数
Emitter* StageEffect::pushEnemy = nullptr;
Emitter* StageEffect::wallBreak[StageEffect::wallTexNum];

/// <summary>
/// 乱数生成
/// 範囲1から範囲2までの乱数を出力
/// </summary>
/// <param name="before">範囲1</param>
/// <param name="after">範囲2</param>
/// <returns>乱数</returns>
float Randomfloat(float before, float after)
{
	float range = 0;
	//乱数の範囲指定
	//両方負の値なら-
	if (before < 0 && after < 0)
	{
		range = abs(before) - abs(after);
	}
	//両方正の値なら-
	else if (before > 0 && after > 0)
	{
		range = abs(after) - abs(before);
	}
	//それ以外なら+
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
	safe_delete(enemeyDead);
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
	ParticleManager::LoadTexture(0, L"Resources/particle/effect1.png");//汎用エフェクト
	ParticleManager::LoadTexture(1, L"Resources/particle/effect2.png");//線が繋がった時のエフェクト
	ParticleManager::LoadTexture(2, L"Resources/particle/garakuta1.png");//壁オブジェクト系
	ParticleManager::LoadTexture(3, L"Resources/particle/garakuta2.png");//壁オブジェクト系
	ParticleManager::LoadTexture(4, L"Resources/particle/garakuta3.png");//壁オブジェクト系
	ParticleManager::LoadTexture(5, L"Resources/particle/garakuta4.png");//壁オブジェクト系
	ParticleManager::LoadTexture(6, L"Resources/particle/garakuta5.png");//壁オブジェクト系
	ParticleManager::LoadTexture(7, L"Resources/particle/garakuta6.png");//壁オブジェクト系
	ParticleManager::LoadTexture(8, L"Resources/particle/garakuta7.png");//壁オブジェクト系
	ParticleManager::LoadTexture(9, L"Resources/particle/garakuta8.png");//壁オブジェクト系
	ParticleManager::LoadTexture(10, L"Resources/particle/garakuta9.png");//壁オブジェクト系
	ParticleManager::LoadTexture(11, L"Resources/particle/garakuta10.png");//壁オブジェクト系

	titleCoreExplosion = new Emitter();
	titleCoreExplosion->Create(0);

	playerMove = new Emitter();
	playerMove->Create(2);

	enemeyDead = new Emitter();
	enemeyDead->Create(0);

	playerBulletDelete = new Emitter();
	playerBulletDelete->Create(0);

	connectLine = new Emitter();
	connectLine->Create(1);

	pushEnemy = new Emitter();
	pushEnemy->Create(0);

	for (int i = 0; i < wallTexNum; i++)
	{
		wallBreak[i] = new Emitter();
		wallBreak[i]->Create(2);
	}
}

float StageEffect::SetTitleCoreExplosion(const XMFLOAT3 position)
{
	//出現時間
	const int maxFrame = 30;

	//時間を進める
	explosionTime++;

	//return値が1になった時にエフェクトが全て消えるタイミングに合うようエフェクトを出す
	if (explosionTime <= explosionTimeMax - maxFrame)
	{
		//最大個数
		const int maxParticlNum = 300;
		//開始サイズ
		const XMFLOAT2 startSize = { 3.0f,3.0f };
		//終了サイズ
		const XMFLOAT2 endSize = { 0.5f ,0.5 };
		//開始カラー
		const XMFLOAT4 startColor = { 0.9f,0.0f,0.0f,0.5f };
		//終了カラー
		const XMFLOAT4 endColor = { 0.9f,0.5f,0.0f,0.5f };
		//Ⅰフレームに出る数
		const int maxNum = 10;
		//速度
		XMFLOAT3 velocity = {};

		//Ⅰフレーム分生成する
		for (int i = 0; i < maxNum; i++)
		{
			//速度をランダムでとる
			velocity.x = Randomfloat(-400, 400) / 100.0f;
			velocity.y = Randomfloat(-400, 400) / 100.0f;

			titleCoreExplosion->InEmitter(maxParticlNum, maxFrame, position,
				velocity, NULL_NUMBER, startSize, endSize, startColor, endColor);
		}
	}


	//時間を保存
	int time = explosionTime;

	//explosionTimeMaxと同じならexplosionTimeを初期化する
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
		//最大個数
		const int maxParticlNum = 50;
		//出現時間
		const int maxFrame = 30;
		//開始サイズ
		const XMFLOAT2 startSize = { 3.0f,3.0f };
		//終了サイズ
		const XMFLOAT2 endSize = { 0.5f,0.5f };
		//開始カラー
		const XMFLOAT4 startColor = { 0.0f,0.0f,0.9f,0.5f };
		//終了カラー
		const XMFLOAT4 endColor = { 0.2f,0.5f,0.8f,0.5f };
		//速度
		XMFLOAT3 velocity = {};

		for (int i = 0; i < 5; i++)
		{
			//エフェクトの座標をプレイヤーの後ろにする
			float radian = (rotation.z - 90) * CHANGE_RADIAN;
			//sin cosの保存
			XMFLOAT2 moveAdd = { cosf(radian) ,sinf(radian) };
			//速度
			velocity.x = Randomfloat(-10, 10) / 100.0f;
			velocity.y = Randomfloat(-10, 10) / 100.0f;
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
	const XMFLOAT2 startSize = { 3.0f,3.0f };
	//終了サイズ
	const XMFLOAT2 endSize = { 3.0f,3.0f };
	//開始カラー
	const XMFLOAT4 startColor = { 0.9f,0.0f,0.0f,0.5f };
	//終了カラー
	const XMFLOAT4 endColor = { 0.0f,0.0f,0.9f,0.5f };
	//座標
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//速度
	XMFLOAT3 velocity = {};

	//一度に出る個数
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		//速度をランダムでとる
		velocity.x = Randomfloat(-400, 400) / 100.0f;
		velocity.y = Randomfloat(-400, 400) / 100.0f;
		//加速度
		XMFLOAT3 accel = { -(velocity.x * 2) / maxFrame,
			-(velocity.y * 2) / maxFrame,0 };

		enemeyDead->InEmitter(maxParticlNum, maxFrame, pos,
			velocity, accel, startSize, endSize, startColor, endColor);
	}

	return maxFrame;
}

void StageEffect::SetPlayerBulletDelete(const XMFLOAT3 position, const XMFLOAT4 color)
{
	//最大個数
	const int maxParticlNum = 100;
	//出現時間
	const int maxFrame = 30;
	//開始サイズ
	const XMFLOAT2 startSize = { 5.0f,5.0f };
	//終了サイズ
	const XMFLOAT2 endSize = { 1.0f,1.0f };
	//カラー(変化なしのため変数一つ)
	const XMFLOAT4 S_E_color = { color.x,color.y,color.z,0.5f };
	//座標
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//速度
	XMFLOAT3 velocity = {};

	//一度に出る個数
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		//速度
		velocity.x = Randomfloat(-100, 100) / 1000.0f;

		playerBulletDelete->InEmitter(maxParticlNum, maxFrame, pos,
			velocity, NULL_NUMBER, startSize, endSize, S_E_color, S_E_color);
	}
}

void StageEffect::SetConnectLine(const XMFLOAT3 position_one, const XMFLOAT3 position_two)
{
	//最大個数
	const int maxParticlNum = 50;
	//出現時間
	const int maxFrame = 40;
	//開始サイズ
	const XMFLOAT2 startSize = { 5.0f,5.0f };
	//終了サイズ
	const XMFLOAT2 endSize = { 300.0f, 300.0f };
	//開始カラー
	const XMFLOAT4 startColor = { 1,1,1,0.5f };
	//終了カラー
	const XMFLOAT4 endColor = { 0,0,1,0.5f };

	connectLine->InEmitter(maxParticlNum, maxFrame, { position_one.x,position_one.y,position_one.z - 1 },
		NULL_NUMBER, NULL_NUMBER, startSize, endSize, startColor, endColor);
	connectLine->InEmitter(maxFrame, maxFrame, { position_two.x,position_two.y,position_two.z - 1 },
		NULL_NUMBER, NULL_NUMBER, startSize, endSize, startColor, endColor);
}

void StageEffect::SetPushEnemy(const XMFLOAT3 position, const float radius, const XMFLOAT4 color)
{
	//最大個数
	const int maxParticlNum = 400;
	//出現時間
	const int maxFrame = 30;
	//カラー(変化なしのため変数一つ)
	const XMFLOAT4 S_E_color = { color.x,color.y,color.z,0.5f };
	//サイズ
	const XMFLOAT2 size = { 1.0f,1.0f };
	//座標
	const XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//速度
	XMFLOAT3 velocity = {};
	//角度
	float angle = 0;

	//一度に出る個数
	const int MaxNum = 10;
	//一つの火花に使う個数
	const int oneEffectNum = 5;
	for (int i = 0; i < MaxNum; i++)
	{
		//速度
		float pushAngle = XMConvertToDegrees(radius);
		//発射する角度の反対する
		pushAngle += 270.0f;
		//吹っ飛ぶエフェクトの角度
		const float pushRange = 45;
		//扇形ににするためランダムで角度を設定する
		pushAngle = Randomfloat(pushAngle - pushRange, pushAngle + pushRange);

		//ラジアンに変換
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
	//最大個数
	const int maxParticlNum = 400;
	//出現時間
	const int maxFrame = 30;
	//カラー(変化なしのため変数一つ)
	const XMFLOAT4 S_E_color = { 1,1,1,1 };
	//サイズ
	const XMFLOAT2 size = { 5.0f,5.0f };
	//速度
	XMFLOAT3 velocity = {};
	//角度
	float angle = 0;

	//一度に出る個数
	const int MaxNum = 10;
	//一つの火花に使う個数
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
	enemeyDead->Update();
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