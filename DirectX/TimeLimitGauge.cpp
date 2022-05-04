#include "TimeLimitGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

TimeLimitGauge* TimeLimitGauge::Create(int frameTexNum, int barTexNum)
{
	//インスタンスを生成
	TimeLimitGauge* instance = new TimeLimitGauge();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(frameTexNum, barTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TimeLimitGauge::~TimeLimitGauge()
{
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool TimeLimitGauge::Initialize(int frameTexNum, int barTexNum)
{
	//ポイント表示(枠)スプライト生成
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	frameSprite->SetSize({ 568 / 5, 104 });
	frameSprite->SetTexSize({ 568, 52 });
	frameSprite->SetPosition({ 640, 120 });
	frameSprite->SetRotation(270);


	//ポイント表示(バー)スプライト生成
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	barSprite->SetSize({ 0, 80 });
	barSprite->SetTexSize({ 556, 40 });
	barSprite->SetPosition({ 640, 120 });
	barSprite->SetRotation(270);
	barSprite->SetColor({ 0, 1, 0, 1 });

	return true;
}

void TimeLimitGauge::Update()
{
	//長さを変更
	if (isChangeLengthBar)
	{
		ChangeLengthBar();
	}

	//スプライト更新
	frameSprite->Update();
	barSprite->Update();
}

void TimeLimitGauge::Draw()
{
	//スプライト描画
	frameSprite->Draw();
	barSprite->Draw();
}

void TimeLimitGauge::Reset()
{
}

void TimeLimitGauge::AddPoint(int point)
{
	//回復ポイントを加算
	recoveryPoint += point;
	if (recoveryPoint >= recoveryPointMax)
	{
		recoveryPoint = recoveryPointMax;
	}

	//ゲージの長さを変更状態にする
	SetChangeLength();
}

void TimeLimitGauge::UsePoint()
{
	//回復ポイントを加算
	recoveryPoint = 0;

	//ゲージの長さを変更状態にする
	SetChangeLength();
}

void TimeLimitGauge::ChangeLengthBar()
{
	//変更を行う時間
	const int changeTime = 20;

	//長さ変更タイマー更新
	changeLengthTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeLengthTimer / changeTime;

	//スプライトのサイズを変更
	XMFLOAT2 size = barSprite->GetSize();
	size.x = Easing::OutQuad(changeLengthBefore, changeLengthAftar, easeTimer);
	//更新したサイズをセット
	barSprite->SetSize(size);

	//ゲージが最大になったらフラグをtrueに
	if (size.x >= lengthMax)
	{
		isGaugeMax = true;
	}
	//それ以外はfalseに
	else 
	{
		isGaugeMax = false;
	}

	//タイマーが指定した時間になったら
	if (changeLengthTimer >= changeTime)
	{
		//長さ変更終了
		isChangeLengthBar = false;
	}
}

void TimeLimitGauge::SetChangeLength()
{
	//バーの長さ変更タイマーを初期化
	changeLengthTimer = 0;
	//イージング用に変更前の長さをセット
	changeLengthBefore = barSprite->GetSize().x;
	//イージング用に変更後の長さをセット
	changeLengthAftar = lengthMax * ((float)recoveryPoint / recoveryPointMax);

	//バーの長さを変更状態にする
	isChangeLengthBar = true;
}
