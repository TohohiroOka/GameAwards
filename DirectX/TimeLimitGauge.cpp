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
	frameSprite = Sprite::Create(frameTexNum, { 0.5f, 0 });
	if (frameSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	frameSprite->SetSize({ 100, 100 });
	frameSprite->SetTexSize({ 100, 100 });
	frameSprite->SetPosition({ 640, -100 });
	//スプライト更新
	frameSprite->Update();


	//ポイント表示(バー)スプライト生成
	barSprite = Sprite::Create(barTexNum, { 0.5f, 0 });
	if (barSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	barSprite->SetSize({ 100, 0 });
	barSprite->SetTexSize({ 100, 100 });
	barSprite->SetPosition({ 640, -100 - lengthMax / 2 });
	//スプライト更新
	barSprite->Update();

	return true;
}

void TimeLimitGauge::Update()
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

void TimeLimitGauge::Draw()
{
	//スプライト描画
	barSprite->Draw();
	frameSprite->Draw();
}

void TimeLimitGauge::Reset()
{
	//回復ポイント
	recoveryPoint = 0;
	//バースプライトの長さを変更するか
	isChangeLengthBar = false;
	//バースプライトの長さ変更タイマー
	changeLengthTimer = 0;
	//バースプライトの長さ変更前の長さ
	changeLengthBefore = 0;
	//バースプライトの長さ変更後の長さ
	changeLengthAftar = 0;
	//ゲージが最大か
	isGaugeMax = false;
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

	//バースプライトの長さを初期化
	barSprite->SetSize({ 100, 0 });
	barSprite->SetPosition({ 640, -100 - lengthMax / 2 });
	barSprite->Update();
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

void TimeLimitGauge::SetMoveGamePos()
{
	//ゲームシーンの座標に移動する時間タイマーを初期化
	moveGamePosTimer = 0;

	//移動状態にセット
	isMoveGamePos = true;
}

void TimeLimitGauge::SetMoveResultPos()
{
	//リザルトシーンの座標に移動する時間タイマーを初期化
	moveResultPosTimer = 0;

	//移動状態にセット
	isMoveResultPos = true;
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
	size.y = Easing::OutQuad(changeLengthBefore, changeLengthAftar, easeTimer);
	//更新したサイズをセット
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);
	XMFLOAT2 leftTop = barSprite->GetTexLeftTop();
	leftTop.y = 100 - size.y;
	barSprite->SetTexLeftTop(leftTop);
	XMFLOAT2 pos = barSprite->GetPosition();
	pos.y = (55 - lengthMax / 2) + leftTop.y;
	barSprite->SetPosition(pos);

	//ゲージが最大になったらフラグをtrueに
	if (size.y >= lengthMax)
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
	changeLengthBefore = barSprite->GetSize().y;
	//イージング用に変更後の長さをセット
	changeLengthAftar = lengthMax * ((float)recoveryPoint / recoveryPointMax);

	//バーの長さを変更状態にする
	isChangeLengthBar = true;
}

void TimeLimitGauge::MoveGamePos()
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
	framePos.y = Easing::OutQuint(-100 - lengthMax / 2, 55 - lengthMax / 2, easeTimer);
	XMFLOAT2 size = barSprite->GetSize();
	XMFLOAT2 leftTop = barSprite->GetTexLeftTop();
	leftTop.y = 100 - size.y;
	barPos.y = Easing::OutQuint((-100 - lengthMax / 2) + leftTop.y, (55 - lengthMax / 2) + leftTop.y, easeTimer);
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

void TimeLimitGauge::MoveResultPos()
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
	framePos.y = Easing::OutQuint(55 - lengthMax / 2, -100 - lengthMax / 2, easeTimer);
	XMFLOAT2 size = barSprite->GetSize();
	XMFLOAT2 leftTop = barSprite->GetTexLeftTop();
	leftTop.y = 100 - size.y;
	barPos.y = Easing::OutQuint((55 - lengthMax / 2) + leftTop.y, (-100 - lengthMax / 2) + leftTop.y, easeTimer);
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
