#include "UIFrame.h"
#include "SafeDelete.h"
#include "Easing.h"

UIFrame* UIFrame::Create(int UIFrameTexNum, int startTexNum)
{
	//インスタンスを生成
	UIFrame* instance = new UIFrame();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(UIFrameTexNum, startTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

UIFrame::~UIFrame()
{
	safe_delete(frameSprite);
	safe_delete(startSprite);
}

bool UIFrame::Initialize(int UIFrameTexNum, int startTexNum)
{
	//枠スプライト生成
	frameSprite = Sprite::Create(UIFrameTexNum);
	if (frameSprite == nullptr) {
		return false;
	}
	//座標をセット
	frameSprite->SetPosition({ 640, -100 });
	//テクスチャサイズをセット
	frameSprite->SetTexSize({ 1280, 39 });
	//大きさをセット
	frameSprite->SetSize({ 1280, 39 });
	//スプライト更新
	frameSprite->Update();


	//スタートボタンスプライト生成
	startSprite = Sprite::Create(startTexNum);
	if (startSprite == nullptr) {
		return false;
	}
	//座標をセット
	startSprite->SetPosition({ 1180, -100 });
	//テクスチャサイズをセット
	startSprite->SetTexSize({ 166, 32 });
	//大きさをセット
	startSprite->SetSize({ 132.8f, 25.6f });
	//スプライト更新
	startSprite->Update();

	return true;
}

void UIFrame::Update()
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
	startSprite->Update();
}

void UIFrame::Draw()
{
	//スプライト描画
	frameSprite->Draw();

	if (!isDrawStart) { return; }
	startSprite->Draw();
}

void UIFrame::Reset()
{
	//スタートボタンを描画するか
	isDrawStart = false;
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
	frameSprite->SetPosition({ 640, -100 });
	frameSprite->Update();
	startSprite->SetPosition({ 1180, -100 });
	startSprite->Update();
}

void UIFrame::SetMoveGamePos()
{
	//ゲームシーンの座標に移動する時間タイマーを初期化
	moveGamePosTimer = 0;

	//移動状態にセット
	isMoveGamePos = true;
}

void UIFrame::SetMoveResultPos()
{
	//リザルトシーンの座標に移動する時間タイマーを初期化
	moveResultPosTimer = 0;

	//移動状態にセット
	isMoveResultPos = true;
}

void UIFrame::MoveGamePos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveGamePosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 startPos = startSprite->GetPosition();
	framePos.y = Easing::OutQuint(-100, 20, easeTimer);
	startPos.y = Easing::OutQuint(-100, 20, easeTimer);
	//更新した座標をセット
	frameSprite->SetPosition(framePos);
	startSprite->SetPosition(startPos);

	//タイマーが指定した時間になったら
	if (moveGamePosTimer >= moveTime)
	{
		//移動状態終了
		isMoveGamePos = false;

		//移動完了
		isMoveGamePosEnd = true;
	}
}

void UIFrame::MoveResultPos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveResultPosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 framePos = frameSprite->GetPosition();
	XMFLOAT2 startPos = startSprite->GetPosition();
	framePos.y = Easing::OutQuint(20, -100, easeTimer);
	startPos.y = Easing::OutQuint(20, -100, easeTimer);
	//更新した座標をセット
	frameSprite->SetPosition(framePos);
	startSprite->SetPosition(startPos);

	//タイマーが指定した時間になったら
	if (moveResultPosTimer >= moveTime)
	{
		//移動状態終了
		isMoveResultPos = false;

		//移動完了
		isMoveResultPosEnd = true;
	}
}
