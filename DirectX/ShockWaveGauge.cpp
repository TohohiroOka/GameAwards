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
	frameSprite->SetSize({ 165, 30 });
	frameSprite->SetTexSize({ 165, 30 });
	frameSprite->SetPosition({ 850, -100 });
	//スプライト更新
	frameSprite->Update();

	//ゲージ(バー)スプライト生成
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	barSprite->SetSize({ 0, 13 });
	barSprite->SetTexSize({ 134, 13 });
	barSprite->SetPosition({ 850 + 29, -99 });
	//スプライト更新
	barSprite->Update();

	return true;
}

void ShockWaveGauge::Update()
{
	//ゲームシーンの座標に移動
	if (isMoveGamePos)
	{
		MoveGamePos();
	}
	//リザルトシーンの座標に移動
	else if (isMoveResultPos)
	{
		MoveResultPos();
	}

	//スプライト更新
	frameSprite->Update();
	barSprite->Update();
}

void ShockWaveGauge::Draw()
{
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
	//ゲームシーンの座標に移動中か
	isMoveGamePos = false;
	//ゲームシーンの座標に移動終了したか
	isMoveGamePosEnd = false;
	//ゲームシーンの座標に移動する時間タイマー
	moveGamePosTimer = 0;
	//リザルトシーンの座標に移動中か
	isMoveResultPos = false;
	//リザルトシーンの座標に移動終了したか
	isMoveResultPosEnd = false;
	//リザルトシーンの座標に移動する時間タイマー
	moveResultPosTimer = 0;

	//スプライトを初期化
	frameSprite->SetSize({ 165, 30 });
	frameSprite->SetPosition({ 850, -100 });
	frameSprite->Update();
	barSprite->SetSize({ 0, 13 });
	barSprite->SetTexSize({ 134, 13 });
	barSprite->SetPosition({ 850 + 29, -99 });
	barSprite->Update();
}

void ShockWaveGauge::IncreasePoint()
{
	//既にゲージが最大なら抜ける
	if (gaugePoint >= gaugePointMax) { return; }

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
	//既にゲージが0なら抜ける
	if (gaugePoint <= 0) { return; }

	//ポイントを減少
	gaugePoint--;
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

void ShockWaveGauge::SetMoveGamePos()
{
	//ゲームシーンの座標に移動する時間タイマーを初期化
	moveGamePosTimer = 0;

	//移動状態にセット
	isMoveGamePos = true;
}

void ShockWaveGauge::SetMoveResultPos()
{
	//リザルトシーンの座標に移動する時間タイマーを初期化
	moveResultPosTimer = 0;

	//移動状態にセット
	isMoveResultPos = true;
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
}

void ShockWaveGauge::MoveGamePos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveGamePosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 barPos = barSprite->GetPosition();
	framePos.y = Easing::OutQuint(-100, 20, easeTimer);
	barPos.y = Easing::OutQuint(-99, 21, easeTimer);
	//更新した座標をセット
	frameSprite->SetPosition(framePos);
	barSprite->SetPosition(barPos);

	//タイマーが指定した時間になったら
	if (moveGamePosTimer >= moveTime)
	{
		//移動状態終了
		isMoveGamePos = false;

		//移動完了
		isMoveGamePosEnd = true;
	}
}

void ShockWaveGauge::MoveResultPos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveResultPosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 barPos = barSprite->GetPosition();
	framePos.y = Easing::OutQuint(20, -100, easeTimer);
	barPos.y = Easing::OutQuint(21, -99, easeTimer);
	//更新した座標をセット
	frameSprite->SetPosition(framePos);
	barSprite->SetPosition(barPos);

	//タイマーが指定した時間になったら
	if (moveResultPosTimer >= moveTime)
	{
		//移動状態終了
		isMoveResultPos = false;

		//移動完了
		isMoveResultPosEnd = true;
	}
}
