#include "ShockWaveGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

ShockWaveGauge* ShockWaveGauge::Create(int gaugeTexNum)
{
	//インスタンスを生成
	ShockWaveGauge* instance = new ShockWaveGauge();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(gaugeTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ShockWaveGauge::~ShockWaveGauge()
{
	safe_delete(gaugeSprite);
}

bool ShockWaveGauge::Initialize(int gaugeTexNum)
{
	//ゲージスプライト生成
	gaugeSprite = Sprite::Create(gaugeTexNum, { 0.5f, 0 });
	if (gaugeSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	gaugeSprite->SetSize({ 191, 0 });
	gaugeSprite->SetTexSize({ 191, 192 });
	gaugeSprite->SetPosition({ 642.5f, 393 - lengthMax / 2 });
	//スプライト更新
	gaugeSprite->Update();

	return true;
}

void ShockWaveGauge::Update()
{
	//更新しない場合抜ける
	if (!isUpdate) { return; }

	//ゲージリセット
	if (isGaugeReset)
	{
		GaugeReset();
	}

	//スプライト更新
	gaugeSprite->Update();
}

void ShockWaveGauge::Draw()
{
	//更新しない場合抜ける
	if (!isUpdate) { return; }
	//スプライト描画
	gaugeSprite->Draw();
}

void ShockWaveGauge::Reset()
{
	//ゲージポイント
	gaugePoint = 0;
	//ゲージレベル
	gaugeLevel = 0;
	//更新するか
	isUpdate = false;

	//スプライトを初期化
	gaugeSprite->SetSize({ 191, 0 });
	gaugeSprite->SetTexSize({ 191, 192 });
	gaugeSprite->SetPosition({ 642.5f, 393 - lengthMax / 2 });
	gaugeSprite->Update();
}

void ShockWaveGauge::SetGaugeReset()
{
	//ゲージポイント
	gaugePoint = 0;
	//ゲージレベル
	gaugeLevel = 0;

	//ゲージリセットにセット
	isGaugeReset = true;
	//タイマーを初期化
	gaugeResetTimer = 0;

	//リセット前の長さを記録しておく
	resetGaugeBeforeLength = gaugeSprite->GetSize();

	//スプライトを初期化
	gaugeSprite->SetSize({ 191, 0 });
	gaugeSprite->SetTexSize({ 191, 192 });
	gaugeSprite->SetPosition({ 642.5f, 393 - lengthMax / 2 });
	gaugeSprite->Update();
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
}

void ShockWaveGauge::ChangeLengthBar()
{
	//ゲージポイントに応じてゲージの長さをセット
	float length = lengthMax * ((float)gaugePoint / gaugePointMax);

	//スプライトのサイズを変更
	XMFLOAT2 size = gaugeSprite->GetSize();
	size.y = length;
	//更新したサイズをセット
	gaugeSprite->SetSize(size);
	gaugeSprite->SetTexSize(size);
	XMFLOAT2 leftTop = gaugeSprite->GetTexLeftTop();
	leftTop.y = lengthMax - size.y;
	gaugeSprite->SetTexLeftTop(leftTop);
	XMFLOAT2 pos = gaugeSprite->GetPosition();
	pos.y = (393 - lengthMax / 2) + (lengthMax - size.y);
	gaugeSprite->SetPosition(pos);
}

void ShockWaveGauge::ChangeGaugeLevel()
{
	//ゲージが1/3以下のときはゲージレベル1
	if (gaugePoint < gaugePointMax / 3) { gaugeLevel = 1; }
	//ゲージが2/3以下のときはゲージレベル2
	else if (gaugePoint < (gaugePointMax / 3) * 2) { gaugeLevel = 2; }
	//それ以外(2/3から最大)ときはゲージレベル3
	else { gaugeLevel = 3; }
}

void ShockWaveGauge::GaugeReset()
{
	//ゲージリセットする時間
	const int resetTime = 100;

	//タイマーを更新
	gaugeResetTimer++;

	//イージング計算用の時間
	float easeTimer = (float)gaugeResetTimer / resetTime;

	//スプライトのサイズを変更
	XMFLOAT2 size = gaugeSprite->GetSize();
	size.y = Easing::OutCubic(resetGaugeBeforeLength.y, 0, easeTimer);
	//更新したサイズをセット
	gaugeSprite->SetSize(size);
	gaugeSprite->SetTexSize(size);
	XMFLOAT2 leftTop = gaugeSprite->GetTexLeftTop();
	leftTop.y = lengthMax - size.y;
	gaugeSprite->SetTexLeftTop(leftTop);
	XMFLOAT2 pos = gaugeSprite->GetPosition();
	pos.y = (393 - lengthMax / 2) + (lengthMax - size.y);
	gaugeSprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (gaugeResetTimer >= resetTime)
	{
		//リセット終了
		isGaugeReset = false;
	}
}
