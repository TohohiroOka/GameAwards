#include "ShockWaveGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

ShockWaveGauge* ShockWaveGauge::Create(int frameTexNum, int barTexNum)
{
	//インスタンスを生成
	ShockWaveGauge* instance = new ShockWaveGauge();
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

ShockWaveGauge::~ShockWaveGauge()
{
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool ShockWaveGauge::Initialize(int frameTexNum, int barTexNum)
{
	//ゲージ(枠)スプライト生成
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	frameSprite->SetSize({ 229, 36 });
	frameSprite->SetTexSize({ 229, 36 });
	XMFLOAT2 frameSize = frameSprite->GetSize();
	frameSprite->SetPosition({ 640 - frameSize.x / 2, 260 });
	//スプライト更新
	frameSprite->Update();

	//ゲージ(バー)スプライト生成
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	barSprite->SetSize({ 0, 36 });
	barSprite->SetTexSize({ 0, 36 });
	barSprite->SetPosition({ 640 - frameSize.x / 2, 260 });
	//スプライト更新
	barSprite->Update();

	return true;
}

void ShockWaveGauge::Update()
{
	//更新しない場合抜ける
	if (!isUpdate) { return; }

	//描画時間をカウント
	if (isDrawTimeCount)
	{
		CountDrawTimer();
	}
	//透過
	else if (isTransparent)
	{
		Transparent();
	}

	//スプライト更新
	frameSprite->Update();
	barSprite->Update();
}

void ShockWaveGauge::Draw()
{
	//更新しない場合抜ける
	if (!isUpdate) { return; }
	//描画しない場合抜ける
	if (!isDraw) { return; }
	//スプライト描画
	barSprite->Draw();
	frameSprite->Draw();
}

void ShockWaveGauge::Reset()
{
	//ゲージポイント
	gaugePoint = 0;
	//ゲージレベル
	gaugeLevel = 0;
	//前のフレームのゲージレベル
	oldGaugeLevel = 0;
	//前のフレームとゲージレベルが違うか
	isChangeGaugeLevel = false;
	//更新するか
	isUpdate = false;
	//描画するか
	isDraw = false;
	//描画時間カウントするか
	isDrawTimeCount = false;
	//描画時間カウントタイマー
	drawTimer = 0;
	//透過させるか
	isTransparent = false;
	//透過させる時間
	transparentTimer = 0;

	//スプライトを初期化
	frameSprite->SetColor({ 1, 1, 1, 0 });
	frameSprite->Update();
	barSprite->SetSize({ 0, 36 });
	barSprite->SetTexSize({ 0, 36 });
	barSprite->SetColor({ 1, 1, 1, 0 });
	barSprite->Update();
}

void ShockWaveGauge::GaugeReset()
{
	//ゲージポイント
	gaugePoint = 0;
	//ゲージレベル
	gaugeLevel = 0;
	//前のフレームのゲージレベル
	oldGaugeLevel = 0;
	//前のフレームとゲージレベルが違うか
	isChangeGaugeLevel = false;
	//描画するか
	isDraw = false;
	//描画時間カウントするか
	isDrawTimeCount = false;
	//描画時間カウントタイマー
	drawTimer = 0;
	//透過させるか
	isTransparent = false;
	//透過させる時間
	transparentTimer = 0;

	//スプライトを初期化
	barSprite->SetSize({ 0, 36 });
	barSprite->SetTexSize({ 0, 36 });
	barSprite->Update();
}

void ShockWaveGauge::IncreasePoint()
{
	//更新しない場合抜ける
	if (!isUpdate) { return; }

	//ポイントを加算
	gaugePoint += 3;
	//最大ポイント数は越えない
	if (gaugePoint > gaugePointMax)
	{
		gaugePoint = gaugePointMax;
	}

	//ゲージを長さを変更
	ChangeLengthBar();
	//ゲージレベルを変更
	ChangeGaugeLevel();
	//描画する
	DrawStart();
}

void ShockWaveGauge::DecreasePoint()
{
	//更新しない場合抜ける
	if (!isUpdate) { return; }

	//既にゲージが0なら抜ける
	if (gaugePoint <= 0) { return; }

	//ポイントを減少
	gaugePoint -= 2;
	//0以下にならない
	if (gaugePoint < 0)
	{
		gaugePoint = 0;
	}

	//ゲージを長さを変更
	ChangeLengthBar();
	//ゲージレベルを変更
	ChangeGaugeLevel();
	//ゲージレベルが変更していたら描画する
	if (isChangeGaugeLevel)
	{
		DrawStart();
	}
}

void ShockWaveGauge::ChangeLengthBar()
{
	//ゲージポイントに応じてゲージの長さをセット
	float length = lengthMax * ((float)gaugePoint / gaugePointMax);

	//スプライトのサイズを変更
	XMFLOAT2 size = barSprite->GetSize();
	size.x = length;
	//更新したサイズをセット
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);
}

void ShockWaveGauge::ChangeGaugeLevel()
{
	//ゲージが1/3以下のときはゲージレベル1
	if (gaugePoint < gaugePointMax / 3) { gaugeLevel = 1; }
	//ゲージが2/3以下のときはゲージレベル2
	else if (gaugePoint < (gaugePointMax / 3) * 2) { gaugeLevel = 2; }
	//それ以外(2/3から最大)ときはゲージレベル3
	else { gaugeLevel = 3; }

	//毎フレーム変更はないのでフラグを下げておく
	isChangeGaugeLevel = false;
	//前のフレームとゲージレベルが違ったらフラグを立てる
	if (gaugeLevel != oldGaugeLevel) { isChangeGaugeLevel = true; }
	//次のフレーム用にoldを更新
	oldGaugeLevel = gaugeLevel;
}

void ShockWaveGauge::DrawStart()
{
	//描画する
	isDraw = true;
	//描画時間カウントするか
	isDrawTimeCount = true;
	//透過しない
	isTransparent = false;

	//タイマーを初期化
	drawTimer = 0;
	transparentTimer = 0;

	//透過していた場合戻す
	XMFLOAT4 color = barSprite->GetColor();
	color.w = 1;
	frameSprite->SetColor(color);
	barSprite->SetColor(color);
}

void ShockWaveGauge::CountDrawTimer()
{
	//描画する時間
	const int drawCountTime = 60;

	//タイマーを更新
	drawTimer++;

	//タイマーが指定した時間に到達したら
	if (drawTimer >= drawCountTime)
	{
		//描画時間カウント終了
		isDrawTimeCount = false;
		//透過開始
		isTransparent = true;
	}
}

void ShockWaveGauge::Transparent()
{
	//透過させる時間
	const int transparentTime = 60;

	//タイマーを更新
	transparentTimer++;

	//イージング計算用の時間
	float easeTimer = (float)transparentTimer / transparentTime;

	//イージングで透過させる
	XMFLOAT4 color = barSprite->GetColor();
	color.w = Easing::Lerp(1.0f, 0.1f, easeTimer);;
	frameSprite->SetColor(color);
	barSprite->SetColor(color);

	//タイマーが指定した時間に到達したら
	if (transparentTimer >= transparentTime)
	{
		//透過終了
		isTransparent = false;
		//描画終了
		isDraw = false;
	}
}
