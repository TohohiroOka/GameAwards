#include "BigShockWaveGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

BigShockWaveGauge* BigShockWaveGauge::Create(int frameTexNum, int barTexNum)
{
	//インスタンスを生成
	BigShockWaveGauge* instance = new BigShockWaveGauge();
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

BigShockWaveGauge::~BigShockWaveGauge()
{
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool BigShockWaveGauge::Initialize(int frameTexNum, int barTexNum)
{
	//ポイント表示(枠)スプライト生成
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	frameSprite->SetSize({ 568 / 4, 52 });
	frameSprite->SetTexSize({ 568, 52 });
	frameSprite->SetPosition({ 1150 - frameSprite->GetSize().x / 2, 55 });


	//ポイント表示(バー)スプライト生成
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	barSprite->SetSize({ 0, 40 });
	barSprite->SetTexSize({ 556, 40 });
	barSprite->SetPosition({ 1150 - lengthMax / 2, 55 });


	return true;
}

void BigShockWaveGauge::Update(int combo)
{
	//コンボ数が変更されていた場合コンボ数を取得
	if (this->combo != combo)
	{
		this->combo = combo;

		//ゲージの長さを変更状態にする
		SetChangeLength();
	}

	//長さを変更
	if (isChangeLengthBar)
	{
		ChangeLengthBar();
	}

	//スプライト更新
	frameSprite->Update();
	barSprite->Update();
}

void BigShockWaveGauge::Draw()
{
	//スプライト描画
	frameSprite->Draw();
	barSprite->Draw();
}

void BigShockWaveGauge::Reset()
{
}

void BigShockWaveGauge::ChangeLengthBar()
{
	//変更を行う時間
	const int changeTime = 40;

	//長さ変更タイマー更新
	changeLengthTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeLengthTimer / changeTime;

	//スプライトのサイズを変更
	XMFLOAT2 size = barSprite->GetSize();
	size.x = Easing::OutQuint(changeLengthBefore, changeLengthAftar, easeTimer);
	//更新したサイズをセット
	barSprite->SetSize(size);

	//タイマーが指定した時間になったら
	if (changeLengthTimer >= changeTime)
	{
		//長さ変更終了
		isChangeLengthBar = false;
	}
}

void BigShockWaveGauge::SetChangeLength()
{
	//バーの長さ変更タイマーを初期化
	changeLengthTimer = 0;
	//イージング用に変更前の長さをセット
	changeLengthBefore = barSprite->GetSize().x;
	//イージング用に変更後の長さをセット
	int gaugeCombo = combo;
	const int gaugeComboMax = 16;
	if (gaugeCombo >= gaugeComboMax)
	{
		gaugeCombo = gaugeComboMax;
	}
	changeLengthAftar = lengthMax * ((float)gaugeCombo / gaugeComboMax);

	//バーの長さを変更状態にする
	isChangeLengthBar = true;
}
