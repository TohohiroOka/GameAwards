#include "StageEffect.h"
#include "SafeDelete.h"
#include "Camera.h"
#include <random>
#include <cstdlib>

using namespace DirectX;

Emitter* StageEffect::generalEffect = nullptr;
int StageEffect::playerMoveContro = 0;
const XMFLOAT3 NULL_NUMBER = { 0,0,0 };//0を入れる時の変数
std::array<Emitter*, StageEffect::wallTexNum> StageEffect::wallEffect;
Emitter* StageEffect::smash = nullptr;
std::forward_list<StageEffect::SMASH> StageEffect::smashInfo;

/// <summary>
/// 乱数生成
/// 0から範囲までの乱数を出力
/// </summary>
/// <param name="after">範囲</param>
/// <returns>乱数</returns>
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
	safe_delete(smash);
}

void StageEffect::Initialize()
{
	ParticleManager::LoadTexture(0, L"Resources/particle/effect1.png");//汎用エフェクト
	ParticleManager::LoadTexture(1, L"Resources/particle/garakuta1.png");//壁オブジェクト系
	ParticleManager::LoadTexture(2, L"Resources/particle/garakuta2.png");//壁オブジェクト系
	ParticleManager::LoadTexture(3, L"Resources/particle/garakuta3.png");//壁オブジェクト系
	ParticleManager::LoadTexture(4, L"Resources/particle/star.png");//スマッシュ時のエフェクト

	generalEffect = new Emitter();
	generalEffect->Create(0);
	generalEffect->SetBloom();

	for (int i = 0; i < wallTexNum; i++)
	{
		wallEffect[i] = new Emitter();
		wallEffect[i]->Create(1 + i);
	}

	smash = new Emitter();
	smash->Create(4);
}

void StageEffect::SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation)
{
	if (playerMoveContro == 0)
	{
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
			float radian = XMConvertToRadians(rotation.z - 90);
			//sin cosの保存
			XMFLOAT2 moveAdd = { cosf(radian) ,sinf(radian) };
			//速度
			velocity.x = (Randomfloat(20) - 10.0f) / 100.0f;
			velocity.y = (Randomfloat(20) - 10.0f) / 100.0f;
			//座標
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

void StageEffect::SetPushEnemy(const XMFLOAT3 position, const unsigned char power)
{
	//出現時間
	const int maxFrame = 30;
	//開始カラー
	XMFLOAT4 startColor = { 1,1,1,1 };
	startColor.y = 0.25f * power;
	startColor.z = 0.25f * power;

	//終了カラー
	const XMFLOAT4 endColor = { 0.0f,0.0f,0.0f,1.0f };
	//サイズ
	const XMFLOAT2 size = { 1.0f,1.0f };
	//座標
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//速度
	XMFLOAT3 velocity = { 0,0,0 };

	//一度に出る個数
	const int MaxNum = 3;
	for (int i = 0; i < MaxNum; i++)
	{
		//速度
		velocity.x = (Randomfloat(20) - 10.0f) / 100.0f;
		velocity.y = (Randomfloat(20) - 10.0f) / 100.0f;
		//座標
		pos.x += (Randomfloat(20) - 10.0f) / 100.0f;
		pos.y += (Randomfloat(20) - 10.0f) / 100.0f;

		generalEffect->InEmitter(maxFrame, pos,
			velocity, NULL_NUMBER, size, size, startColor, endColor);
	}
}

void StageEffect::SetWallBreak(const XMFLOAT3 position)
{
	//300超えていたら追加しない
	int count = wallEffect[0]->GetCount() + wallEffect[1]->GetCount() + wallEffect[2]->GetCount();
	if (count > 300) { return; }

	//出現時間
	const int maxFrame = 20;
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
	pos.x += Randomfloat(10) - 5.0f;
	pos.y += Randomfloat(10) - 5.0f;

	velocity.x = (Randomfloat(100) - 50.0f) / 300.0f;
	velocity.y = (Randomfloat(100) - 50.0f) / 300.0f;

	int useTex = (int)Randomfloat(wallTexNum - 1);

	wallEffect[useTex]->InEmitter(maxFrame, pos,
		velocity, NULL_NUMBER, size, size, S_E_color, S_E_color);
}

void StageEffect::SetSmash(const XMFLOAT3 position, const  unsigned int power)
{
	//リストに要素を追加
	smashInfo.emplace_front();
	//追加した要素の参照
	StageEffect::SMASH& add = smashInfo.front();
	add.position = position;
	add.position.z = -1.0f;
	add.velocity.x = -position.x / 150.0f;
	add.velocity.y = -position.y / 150.0f;
	if (power == 1) { add.power = 1; } else if (power == 2) { add.power = 3; } else if (power == 3) { add.power = 5; }
	add.maxTime = power * 10;
	add.time = 0;
}

void StageEffect::smashUpdate()
{
	//300超えていたら追加しない
	int count = smash->GetCount();
	if (count > 600) { return; }

	//出現時間
	int maxFrame = 20;
	//開始カラー
	const XMFLOAT4 startColor = { 1,1,1,1 };
	//終了カラー
	const XMFLOAT4 endColor = { 0,0,0,1 };
	//サイズ
	const XMFLOAT2 size = { 10.0f,10.0f };
	//速度
	XMFLOAT3 velocity = {};
	//角度
	float angle = 0;

	for (std::forward_list<StageEffect::SMASH>::iterator it = smashInfo.begin();
		it != smashInfo.end(); it++)
	{
		maxFrame = it->power * 10;
		XMFLOAT3 pos = it->position;
		pos.x += Randomfloat(10) - 5.0f;
		pos.y += Randomfloat(10) - 5.0f;

		velocity = it->velocity;

		smash->InEmitter(maxFrame, pos,
			velocity, NULL_NUMBER, size, size, startColor, endColor);
		it->time++;
	}

	//表示時間をが過ぎたパーティクルを削除
	smashInfo.remove_if([](StageEffect::SMASH& x) {
		return x.time >= x.maxTime;
		}
	);
}

void StageEffect::SetDeleteEnemey(const XMFLOAT3 position)
{
	//出現時間
	const int maxFrame = 30;
	//開始カラー
	const XMFLOAT4 startColor = { 1.0f,0.25f,0.25f,1.0f };
	//終了カラー
	const XMFLOAT4 endColor = { 0.0f,0.0f,0.0f,1.0f };
	//開始サイズ
	const XMFLOAT2 startSize = { 3.0f,3.0f };
	//終了サイズ
	const XMFLOAT2 endSize = { 0.5f,0.5f };
	//座標
	XMFLOAT3 pos = { position.x,position.y,position.z - 1 };
	//速度
	XMFLOAT3 velocity = { 0,0,0 };

	//一度に出る個数
	const int MaxNum = 20;
	for (int i = 0; i < MaxNum; i++)
	{
		//速度
		velocity.x = (Randomfloat(100) - 50.0f) / 800.0f;
		velocity.y = (Randomfloat(100) - 50.0f) / 800.0f;
		//座標
		pos.x += (Randomfloat(20) - 10.0f) / 100.0f;
		pos.y += (Randomfloat(20) - 10.0f) / 100.0f;

		generalEffect->InEmitter(maxFrame, pos,
			velocity, NULL_NUMBER, startSize, endSize, startColor, endColor);
	}
}

void StageEffect::Update(Camera* camera)
{
	ParticleManager::SetCamera(camera);
	generalEffect->Update();
	for (int i = 0; i < wallTexNum; i++)
	{
		wallEffect[i]->Update();
	}
	//リストが0以上なら更新
	if ((UINT)std::distance(smashInfo.begin(), smashInfo.end()) > 0)
	{
		smashUpdate();
	}
	smash->Update();
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
	if (smash->GetCount() != 0)
	{
		smash->Draw();
	}
	ParticleManager::PostDraw();
}