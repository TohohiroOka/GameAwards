#include "TimeLimitGauge.h"
#include "SafeDelete.h"
#include "Easing.h"

TimeLimitGauge* TimeLimitGauge::Create(int timeTexNum, int frameTexNum, int barTexNum)
{
	//インスタンスを生成
	TimeLimitGauge* instance = new TimeLimitGauge();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(timeTexNum, frameTexNum, barTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TimeLimitGauge::~TimeLimitGauge()
{
	safe_delete(timeSprite);
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool TimeLimitGauge::Initialize(int timeTexNum, int frameTexNum, int barTexNum)
{
	//TIMEスプライト生成
	timeSprite = Sprite::Create(timeTexNum);
	if (timeSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	timeSprite->SetSize({ 75, 30 });
	timeSprite->SetTexSize({ 51, 20 });
	timeSprite->SetPosition({ 83, -100 });
	//スプライト更新
	timeSprite->Update();

	//ポイント表示(枠)スプライト生成
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	frameSprite->SetSize({ 900, 19 });
	frameSprite->SetTexSize({ 900, 19 });
	frameSprite->SetPosition({ 125, -100 });
	//スプライト更新
	frameSprite->Update();


	//ポイント表示(バー)スプライト生成
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	barSprite->SetSize({ 896, 15 });
	barSprite->SetTexSize({ 896, 15 });
	barSprite->SetPosition({ 127, -100 });
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

	//回復状態
	if (isRecovery)
	{
		RecoveryLengthBar();
	}
	//カウントダウン
	else if (isCountDown)
	{
		CountDown();
	}

	//スプライト更新
	timeSprite->Update();
	barSprite->Update();
	frameSprite->Update();
}

void TimeLimitGauge::Draw()
{
	//スプライト描画
	timeSprite->Draw();
	barSprite->Draw();
	frameSprite->Draw();
}

void TimeLimitGauge::Reset()
{
	//時間計測タイマー
	timer = timeLimitMax;
	//カウントダウンするか
	isCountDown = false;
	//回復中か
	isRecovery = false;
	//カウントダウンが最後まで行ったか
	isCountDownEnd = false;


	//回復中バーの長さ変更タイマー
	recoveryLengthTimer = 0;
	//回復中バーの長さ変更前の長さ
	recoveryLengthBefore = 0;
	//回復中バーの長さ変更後の長さ
	recoveryLengthAfter = 0;

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

	//スプライト初期化
	timeSprite->SetPosition({ 83, -100 });
	timeSprite->Update();
	frameSprite->SetPosition({ 125, -100 });
	frameSprite->Update();
	barSprite->SetSize({ 896, 13 });
	barSprite->SetTexSize({ 896, 13 });
	barSprite->SetPosition({ 127, -100 });
	barSprite->Update();
}

void TimeLimitGauge::Recovery(int second)
{
	//引数の秒数、制限時間を回復する
	timer += second * 60;

	//最大以上を回復しない
	if (timer >= timeLimitMax)
	{
		timer = timeLimitMax;
	}

	//バーの長さ変更変更をセット
	SetRecoveryLengthBar();

	//回復状態
	isRecovery = true;
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

void TimeLimitGauge::CountDown()
{
	//毎フレームタイマーをカウントダウン
	timer--;

	//0以下になったら
	if (timer <= 0)
	{
		//0以下にならない
		timer = 0;

		//カウントダウン終了
		isCountDown = false;
		isCountDownEnd = true;
	}

	//カウントダウンによるバーの長さ変更
	CountDownLengthBar();
}

void TimeLimitGauge::CountDownLengthBar()
{
	//イージング用に変更後の長さをセット
	float length = lengthMax * ((float)timer / timeLimitMax);

	//スプライトのサイズを変更
	XMFLOAT2 size = barSprite->GetSize();
	size.x = length;
	//更新したサイズをセット
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);
}

void TimeLimitGauge::RecoveryLengthBar()
{
	//回復を行う時間
	const int changeTime = 240;

	//長さ変更タイマー更新
	recoveryLengthTimer++;

	//イージング計算用の時間
	float easeTimer = (float)recoveryLengthTimer / changeTime;

	//スプライトのサイズを変更
	XMFLOAT2 size = barSprite->GetSize();
	size.x = Easing::OutQuint(recoveryLengthBefore, recoveryLengthAfter, easeTimer);
	//更新したサイズをセット
	barSprite->SetSize(size);
	barSprite->SetTexSize(size);

	//タイマーが指定した時間になったら
	if (recoveryLengthTimer >= changeTime)
	{
		//回復状態終了
		isRecovery = false;
	}
}

void TimeLimitGauge::SetRecoveryLengthBar()
{
	//回復中バーの長さ変更タイマーを初期化
	recoveryLengthTimer = 0;
	//イージング用に変更前の長さをセット
	recoveryLengthBefore = barSprite->GetSize().x;
	//イージング用に変更後の長さをセット
	recoveryLengthAfter = lengthMax * ((float)timer / timeLimitMax);
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
	XMFLOAT2 timePos = timeSprite->GetPosition();
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 barPos = barSprite->GetPosition();
	timePos.y = Easing::OutQuint(-100, 20, easeTimer);
	framePos.y = Easing::OutQuint(-100, 20, easeTimer);
	barPos.y = Easing::OutQuint(-100, 20, easeTimer);
	//更新した座標をセット
	timeSprite->SetPosition(timePos);
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
	XMFLOAT2 timePos = frameSprite->GetPosition();
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 barPos = barSprite->GetPosition();
	timePos.y = Easing::OutQuint(20, -100, easeTimer);
	framePos.y = Easing::OutQuint(20, -100, easeTimer);
	barPos.y = Easing::OutQuint(20, -100, easeTimer);
	//更新した座標をセット
	timeSprite->SetPosition(timePos);
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
