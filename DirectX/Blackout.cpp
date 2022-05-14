#include "Blackout.h"
#include "SafeDelete.h"
#include "Easing.h"

Blackout* Blackout::Create(int plainTexNum)
{
	//インスタンスを生成
	Blackout* instance = new Blackout();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(plainTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Blackout::~Blackout()
{
	safe_delete(blackoutSprite);
}

bool Blackout::Initialize(int plainTexNum)
{
	//暗転用スプライト生成
	blackoutSprite = Sprite::Create(plainTexNum, { 0, 0 });
	if (blackoutSprite == nullptr) {
		return false;
	}
	//座標をセット
	blackoutSprite->SetPosition({ 0, 0 });
	//テクスチャサイズをセット
	blackoutSprite->SetTexSize({ 1, 1 });
	//大きさをセット
	blackoutSprite->SetSize({ 1280, 720 });
	//色をセット
	blackoutSprite->SetColor({ 0, 0, 0, 0 });
	//スプライト更新
	blackoutSprite->Update();

	return true;
}

void Blackout::Update()
{
	//暗転
	if (isBlackout)
	{
		BlackoutUpdate();
	}
	//暗転解除
	if (isReturn)
	{
		BlackoutReturn();
	}

	//スプライト更新
	blackoutSprite->Update();
}

void Blackout::Draw()
{
	//スプライト描画
	blackoutSprite->Draw();
}

void Blackout::Reset()
{
	//真っ暗か
	isAllBlack = false;
	//暗転中か
	isBlackout = false;
	//暗転を戻すか
	isReturn = false;
}

void Blackout::SetBlackout()
{
	//タイマーを初期化する
	blackoutTimer = 0;

	//暗転状態にセット
	isBlackout = true;
}

void Blackout::SetBlackoutReturn()
{
	//タイマーを初期化する
	returnTimer = 0;

	//暗転解除状態にセット
	isReturn = true;
}

void Blackout::BlackoutUpdate()
{
	//暗転を行う時間
	const int blackoutTime = 100;

	//タイマー更新
	blackoutTimer++;

	//イージング計算用の時間
	float easeTimer = (float)blackoutTimer / blackoutTime;

	//イージングで暗転
	float alpha = Easing::OutQuad(0, 1, easeTimer);
	blackoutSprite->SetColor({ 0, 0, 0, alpha });

	//タイマーが指定した時間になったら
	if (blackoutTimer >= blackoutTime)
	{
		//暗転終了
		isBlackout = false;

		//真っ暗
		isAllBlack = true;
	}
}

void Blackout::BlackoutReturn()
{
	//暗転解除を行う時間
	const int returnTime = 100;

	//タイマー更新
	returnTimer++;

	//イージング計算用の時間
	float easeTimer = (float)returnTimer / returnTime;

	//イージングで暗転
	float alpha = Easing::OutQuad(1, 0, easeTimer);
	blackoutSprite->SetColor({ 0, 0, 0, alpha });

	//タイマーが指定した時間になったら
	if (returnTimer >= returnTime)
	{
		//暗転解除終了
		Reset();
	}
}
