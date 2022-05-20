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
	frameSprite->SetSize({ 165, 30 });
	frameSprite->SetTexSize({ 165, 30 });
	frameSprite->SetPosition({ 850, -100 });
	//スプライト更新
	frameSprite->Update();

	//ポイント表示(バー)スプライト生成
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

void BigShockWaveGauge::Update()
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
	barSprite->Draw();
	frameSprite->Draw();
}

void BigShockWaveGauge::Reset()
{
	//ポイント
	point = 0;
	//ゲージレベル
	gaugeLevel = 0;
	//バースプライトの長さを変更するか
	isChangeLengthBar = false;
	//バースプライトの長さ変更タイマー
	changeLengthTimer = 0;
	//バースプライトの長さ変更前の長さ
	changeLengthBefore = 0;
	//バースプライトの長さ変更後の長さ
	changeLengthAftar = 0;
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

void BigShockWaveGauge::AddPoint()
{
	//ポイントを1加算
	point++;

	//最大ポイント数は越えない
	if (point > maxPoint)
	{
		point = maxPoint;

		return;
	}

	//ゲージレベルを更新
	gaugeLevel = point / (maxPoint / 3);

	//ゲージを長さを変更状態にする
	SetChangeLength();
}

void BigShockWaveGauge::UsePoint()
{
	//ポイントを0にする
	point = 0;

	//ゲージレベルを0にする
	gaugeLevel = 0;

	//ゲージを長さを変更状態にする
	SetChangeLength();
}

void BigShockWaveGauge::SetMoveGamePos()
{
	//ゲームシーンの座標に移動する時間タイマーを初期化
	moveGamePosTimer = 0;

	//移動状態にセット
	isMoveGamePos = true;
}

void BigShockWaveGauge::SetMoveResultPos()
{
	//リザルトシーンの座標に移動する時間タイマーを初期化
	moveResultPosTimer = 0;

	//移動状態にセット
	isMoveResultPos = true;
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
	barSprite->SetTexSize(size);

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
	changeLengthAftar = lengthMax * ((float)point / maxPoint);

	//バーの長さを変更状態にする
	isChangeLengthBar = true;
}

void BigShockWaveGauge::MoveGamePos()
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

void BigShockWaveGauge::MoveResultPos()
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
