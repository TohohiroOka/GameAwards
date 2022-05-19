#include "ResultUI.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "Input.h"
#include "XInputManager.h"
#include "Audio.h"

ResultUI* ResultUI::Create(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int retryTexNum, int backTitleTexNum)
{
	//インスタンスを生成
	ResultUI* instance = new ResultUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(plainTexNum, resultTexNum, scoreTexNum, numberTexNum, retryTexNum, backTitleTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ResultUI::~ResultUI()
{
	safe_delete(blackoutSprite);
	safe_delete(resultSprite);
	safe_delete(breakSprite);
	for (int i = 0; i < breakDigits; i++)
	{
		safe_delete(breakNumSprite[i]);
	}
	safe_delete(retrySprite);
	safe_delete(backTitleSprite);
}

bool ResultUI::Initialize(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int retryTexNum, int backTitleTexNum)
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


	//リザルトスプライト生成
	resultSprite = Sprite::Create(resultTexNum);
	if (resultSprite == nullptr) {
		return false;
	}
	//座標をセット
	resultSprite->SetPosition({ -200, 125 });
	//テクスチャサイズをセット
	resultSprite->SetTexSize({ 268, 64 });
	//大きさをセット
	resultSprite->SetSize({ 268, 64 });
	//スプライト更新
	resultSprite->Update();


	//BREAKスプライト生成
	breakSprite = Sprite::Create(scoreTexNum, { 1, 0.5f });
	if (breakSprite == nullptr) {
		return false;
	}
	//座標をセット
	breakSprite->SetPosition({ -100, 300 });
	//テクスチャサイズをセット
	breakSprite->SetTexSize({ 234, 64 });
	//大きさをセット
	breakSprite->SetSize({ 234, 64 });
	//スプライト更新
	breakSprite->Update();

	//壊した数スプライト生成
	//数の桁数分回す
	for (int i = 0; i < breakDigits; i++)
	{
		//壊した数スプライト生成
		breakNumSprite[i] = Sprite::Create(numberTexNum);
		if (breakNumSprite[i] == nullptr) {
			return false;
		}
		//大きさをセット
		XMFLOAT2 size = { 32, 64 };
		breakNumSprite[i]->SetSize(size);
		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 48, 64 };
		breakNumSprite[i]->SetTexSize(texSize);
		//座標をセット
		XMFLOAT2 pos = { 1400, 300 };
		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
		//スプライト更新
		breakNumSprite[i]->Update();
	}


	//リトライスプライト生成
	retrySprite = Sprite::Create(retryTexNum, { 1, 0.5f });
	if (retrySprite == nullptr) {
		return false;
	}
	//座標をセット
	retrySprite->SetPosition({ -100, 550 });
	//テクスチャサイズをセット
	retrySprite->SetTexSize({ 225, 65 });
	//大きさをセット
	retrySprite->SetSize({ 225, 65 });
	//スプライト更新
	retrySprite->Update();

	//タイトルに戻るスプライト生成
	backTitleSprite = Sprite::Create(backTitleTexNum, { 0, 0.5f });
	if (backTitleSprite == nullptr) {
		return false;
	}
	//座標をセット
	backTitleSprite->SetPosition({ 1400, 550 });
	//テクスチャサイズをセット
	backTitleSprite->SetTexSize({ 207, 63 });
	//大きさをセット
	backTitleSprite->SetSize({ 207, 63 });
	//スプライト更新
	backTitleSprite->Update();

	//暗転状態にセットしておく
	SetBlackOut();

	//サウンドの読み込み
	Audio* audio = Audio::GetInstance();
	sound[0] = audio->SoundLoadWave("Resources/sound/select.wav");//カーソル移動音

	return true;
}

void ResultUI::Update()
{
	//暗転
	if (isBlackout)
	{
		BlackOut();
	}
	//リザルトスプライトを動かす
	if (isMoveResultSprite)
	{
		MoveResultSprite();
	}
	//壁破壊数スプライトを動かす
	if (isMoveBreakSprite)
	{
		MoveBreakSprite();
	}
	//リトライスプライトを動かす
	if (isMoveRetrySprite)
	{
		MoveRetrySprite();
	}
	//全て描画し終えたら
	if (isDrawAll)
	{
		//リトライするかタイトルシーンに戻るか選択
		SelectRetry();
	}

	//スプライト更新
	blackoutSprite->Update();
	resultSprite->Update();
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Update();
	}
	retrySprite->Update();
	backTitleSprite->Update();
}

void ResultUI::Draw()
{
	//スプライト描画
	blackoutSprite->Draw();
	resultSprite->Draw();
	breakSprite->Draw();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Draw();
	}
	retrySprite->Draw();
	backTitleSprite->Draw();
}

void ResultUI::Reset()
{
	//壁破壊枚数
	breakWallNum = 0;

	//暗転中か
	isBlackout = false;
	//暗転する時間タイマー
	blackoutTimer = 0;
	//リザルトスプライトを動かすか
	isMoveResultSprite = false;
	//リザルトスプライトを動かす時間タイマー
	moveResultSpriteTimer = 0;
	//壁破壊数スプライトを動かすか
	isMoveBreakSprite = false;
	//壁破壊数スプライトを動かす時間タイマー
	moveBreakSpriteTimer = 0;
	//リトライスプライトを動かすか
	isMoveRetrySprite = false;
	//リトライスプライトを動かす時間タイマー
	moveRetrySpriteTimer = 0;

	//全て描画したか
	isDrawAll = false;

	//リトライ状態か
	isRetry = true;
	//確定したか
	isSelect = false;

	//座標をセット
	blackoutSprite->SetPosition({ 0, 0 });
	//色をセット
	blackoutSprite->SetColor({ 0, 0, 0, 0 });
	//スプライト更新
	blackoutSprite->Update();

	//座標をセット
	resultSprite->SetPosition({ -200, 125 });
	//スプライト更新
	resultSprite->Update();

	//座標をセット
	breakSprite->SetPosition({ -100, 300 });
	//スプライト更新
	breakSprite->Update();

	for (int i = 0; i < breakDigits; i++)
	{
		//大きさをセット
		XMFLOAT2 size = breakNumSprite[i]->GetSize();;

		//座標をセット
		XMFLOAT2 pos = { 1400, 300 };
		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
		//スプライト更新
		breakNumSprite[i]->Update();
	}

	//座標をセット
	retrySprite->SetPosition({ -100, 550 });
	retrySprite->SetColor({ 1, 1, 1, 1 });
	//スプライト更新
	retrySprite->Update();

	//座標をセット
	backTitleSprite->SetPosition({ 1400, 550 });
	backTitleSprite->SetColor({ 1, 1, 1, 1 });
	//スプライト更新
	backTitleSprite->Update();

	//背景暗転状態にセットしておく
	SetBlackOut();
}

void ResultUI::SetBreakWallNum(int breakWallNum)
{
	this->breakWallNum = breakWallNum;

	//スプライトの数字を更新
	ChangeBreakNumSprite();
}

void ResultUI::SetBlackOut()
{
	//タイマーを初期化する
	blackoutTimer = 0;

	//暗転状態にセット
	isBlackout = true;
}

void ResultUI::SetMoveResultSprite()
{
	//タイマーを初期化する
	moveBreakSpriteTimer = 0;

	//リザルトスプライトを動かす状態にセット
	isMoveResultSprite = true;
}

void ResultUI::SetMoveBreakSprite()
{
	//タイマーを初期化する
	moveBreakSpriteTimer = 0;

	//壁破壊数スプライトを動かす状態にセット
	isMoveBreakSprite = true;
}

void ResultUI::SetMoveRetrySprite()
{
	//タイマーを初期化する
	moveRetrySpriteTimer = 0;

	//リトライスプライトを動かす状態にセット
	isMoveRetrySprite = true;
}

void ResultUI::ChangeBreakNumSprite()
{
	//数字をそれぞれ出力する
	int digit[breakDigits];
	digit[0] = breakWallNum % 10;			//0001
	digit[1] = (breakWallNum / 10) % 10;	//0010
	digit[2] = (breakWallNum / 100) % 10;	//0100
	digit[3] = (breakWallNum / 1000) % 10;	//1000

	//それぞれの桁の数字分スプライトのテクスチャ切り出しをずらす
	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = breakNumSprite[i]->GetTexSize().x * digit[i];
		breakNumSprite[i]->SetTexLeftTop(leftTop);
	}
}

void ResultUI::BlackOut()
{
	//暗転を行う時間
	const int blackoutTime = 200;

	//タイマー更新
	blackoutTimer++;

	//イージング計算用の時間
	float easeTimer = (float)blackoutTimer / blackoutTime;

	//イージングで枠の暗転
	float alpha = Easing::OutQuad(0, 0.6f, easeTimer);
	blackoutSprite->SetColor({ 0, 0, 0, alpha });

	//タイマーが指定した時間になったら
	if (blackoutTimer >= blackoutTime)
	{
		//暗転終了
		isBlackout = false;

		//リザルトスプライトを動かす状態にセット
		SetMoveResultSprite();
	}
}

void ResultUI::MoveResultSprite()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveResultSpriteTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveResultSpriteTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 resultPos = resultSprite->GetPosition();
	resultPos.x = Easing::OutQuint(-50, 640, easeTimer);
	//更新した座標をセット
	resultSprite->SetPosition(resultPos);

	//タイマーが指定した時間になったら
	if (moveResultSpriteTimer >= moveTime)
	{
		//移動状態終了
		isMoveResultSprite = false;

		//壁破壊数スプライトを動かす状態にセット
		SetMoveBreakSprite();
	}
}

void ResultUI::MoveBreakSprite()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveBreakSpriteTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveBreakSpriteTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 breakPos = breakSprite->GetPosition();
	breakPos.x = Easing::OutQuint(-50, 600, easeTimer);
	//更新した座標をセット
	breakSprite->SetPosition(breakPos);

	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 breakNumPos = breakNumSprite[i]->GetPosition();
		XMFLOAT2 breakNumSize = breakNumSprite[i]->GetSize();
		breakNumPos.x = Easing::OutQuint(1330 - i * breakNumSize.x, 800 - i * breakNumSize.x, easeTimer);
		//更新した座標をセット
		breakNumSprite[i]->SetPosition(breakNumPos);
	}

	//タイマーが指定した時間になったら
	if (moveBreakSpriteTimer >= moveTime)
	{
		//移動状態終了
		isMoveBreakSprite = false;

		//リトライスプライトを動かす状態にセット
		SetMoveRetrySprite();
	}
}

void ResultUI::MoveRetrySprite()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveRetrySpriteTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveRetrySpriteTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 retryPos = retrySprite->GetPosition();
	retryPos.x = Easing::OutQuint(-50, 600, easeTimer);
	//更新した座標をセット
	retrySprite->SetPosition(retryPos);

	XMFLOAT2 backTitlePos = backTitleSprite->GetPosition();
	backTitlePos.x = Easing::OutQuint(1400, 680, easeTimer);
	//更新した座標をセット
	backTitleSprite->SetPosition(backTitlePos);

	//タイマーが指定した時間になったら
	if (moveRetrySpriteTimer >= moveTime)
	{
		//移動状態終了
		isMoveRetrySprite = false;

		//全ての描画完了
		isDrawAll = true;

		//リトライを選択状態にするためリトライスプライトの色を変更
		retrySprite->SetColor({ 1, 0, 0, 1 });
	}
}

void ResultUI::SelectRetry()
{
	//確定していたら抜ける
	if (isSelect) { return; }

	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();
	Audio* audio = Audio::GetInstance();

	//リトライを選択しているとき
	if (isRetry)
	{
		//右方向にスティックを倒すと
		if (input->TriggerKey(DIK_RIGHT) || Xinput->TriggerLeftStickX(false))
		{
			//サウンドの再生
			audio->SoundPlayWava(sound[0], false);
			//タイトルシーンに戻る状態に変更
			isRetry = false;

			retrySprite->SetColor({ 1, 1, 1, 1 });
			backTitleSprite->SetColor({ 1, 0, 0, 1 });
		}
	}
	//タイトルシーンに戻るを選択しているとき
	else
	{
		//左方向にスティックを倒すと
		if (input->TriggerKey(DIK_LEFT) || Xinput->TriggerLeftStickX(true))
		{
			//サウンドの再生
			audio->SoundPlayWava(sound[0], false);
			//リトライする状態に変更
			isRetry = true;

			retrySprite->SetColor({ 1, 0, 0, 1 });
			backTitleSprite->SetColor({ 1, 1, 1, 1 });
		}
	}
}
