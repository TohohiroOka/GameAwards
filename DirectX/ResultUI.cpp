#include "ResultUI.h"
#include "SafeDelete.h"
#include "Easing.h"

ResultUI* ResultUI::Create(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int maxComboTexNum, int retryTexNum, int pressATexNum)
{
	//インスタンスを生成
	ResultUI* instance = new ResultUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(plainTexNum, resultTexNum, scoreTexNum, numberTexNum, maxComboTexNum, retryTexNum, pressATexNum)) {
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
	safe_delete(maxComboSprite);
	for (int i = 0; i < maxComboDigits; i++)
	{
		safe_delete(maxComboNumSprite[i]);
	}
	safe_delete(retrySprite);
	safe_delete(pressASprite);
}

bool ResultUI::Initialize(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int maxComboTexNum, int retryTexNum, int pressATexNum)
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


	//MAXCOMBOスプライト生成
	maxComboSprite = Sprite::Create(maxComboTexNum, { 1, 0.5f });
	if (maxComboSprite == nullptr) {
		return false;
	}
	//座標をセット
	maxComboSprite->SetPosition({ -100, 400 });
	//テクスチャサイズをセット
	maxComboSprite->SetTexSize({ 391, 63 });
	//大きさをセット
	maxComboSprite->SetSize({ 391, 63 });
	//スプライト更新
	maxComboSprite->Update();

	//最大コンボ数スプライト生成
	//数の桁数分回す
	for (int i = 0; i < maxComboDigits; i++)
	{
		//最大コンボ数スプライト生成
		maxComboNumSprite[i] = Sprite::Create(numberTexNum);
		if (maxComboNumSprite[i] == nullptr) {
			return false;
		}
		//大きさをセット
		XMFLOAT2 size = { 32, 64 };
		maxComboNumSprite[i]->SetSize(size);
		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 48, 64 };
		maxComboNumSprite[i]->SetTexSize(texSize);
		//座標をセット
		XMFLOAT2 pos = { 1400, 400 };
		pos.x -= size.x * i;
		maxComboNumSprite[i]->SetPosition(pos);
		//スプライト更新
		maxComboNumSprite[i]->Update();
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

	//PRESS Aスプライト生成
	pressASprite = Sprite::Create(pressATexNum, { 0, 0.5f });
	if (pressASprite == nullptr) {
		return false;
	}
	//座標をセット
	pressASprite->SetPosition({ 1400, 550 });
	//テクスチャサイズをセット
	pressASprite->SetTexSize({ 318, 83 });
	//大きさをセット
	pressASprite->SetSize({ 318, 83 });
	//スプライト更新
	pressASprite->Update();

	//暗転状態にセットしておく
	SetBlackOut();

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
	//最大コンボ数スプライトを動かす
	if (isMoveMaxComboSprite)
	{
		MoveMaxComboSprite();
	}
	//リトライスプライトを動かす
	if (isMoveRetrySprite)
	{
		MoveRetrySprite();
	}

	//スプライト更新
	blackoutSprite->Update();
	resultSprite->Update();
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Update();
	}
	maxComboSprite->Update();
	for (int i = 0; i < maxComboDigits; i++)
	{
		maxComboNumSprite[i]->Update();
	}
	retrySprite->Update();
	pressASprite->Update();
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
	maxComboSprite->Draw();
	for (int i = 0; i < maxComboDigits; i++)
	{
		maxComboNumSprite[i]->Draw();
	}
	retrySprite->Draw();
	pressASprite->Draw();
}

void ResultUI::Reset()
{
	//暗転を解除する
	blackoutSprite->SetColor({ 0, 0, 0, 0 });
	//フラグをfalseに
	isDrawAll = false;
	//スプライト更新
	blackoutSprite->Update();
	resultSprite->Update();
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Update();
	}
	retrySprite->Update();
}

void ResultUI::SetBreakWallNum(int breakWallNum)
{
	this->breakWallNum = breakWallNum;

	//スプライトの数字を更新
	ChangeBreakNumSprite();
}

void ResultUI::SetMaxCombo(int maxCombo)
{
	this->maxCombo = maxCombo;

	//スプライトの数字を更新
	ChangeMaxComboSprite();
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

void ResultUI::SetMoveMaxComboSprite()
{
	//タイマーを初期化する
	moveMaxComboSpriteTimer = 0;

	//最大コンボ数スプライトを動かす状態にセット
	isMoveMaxComboSprite = true;
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
	digit[0] = breakWallNum % 10;			//001
	digit[1] = (breakWallNum / 10) % 10;	//010
	digit[2] = (breakWallNum / 100) % 10;	//100
	digit[3] = (breakWallNum / 100) % 10;	//100

	//それぞれの桁の数字分スプライトのテクスチャ切り出しをずらす
	for (int i = 0; i < maxComboDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = breakNumSprite[i]->GetTexSize().x * digit[i];
		breakNumSprite[i]->SetTexLeftTop(leftTop);
	}
}

void ResultUI::ChangeMaxComboSprite()
{
	//数字をそれぞれ出力する
	int digit[maxComboDigits];
	digit[0] = maxCombo % 10;			//001
	digit[1] = (maxCombo / 10) % 10;	//010
	digit[2] = (maxCombo / 100) % 10;	//100
	digit[3] = (maxCombo / 100) % 10;	//100

	//それぞれの桁の数字分スプライトのテクスチャ切り出しをずらす
	for (int i = 0; i < maxComboDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = maxComboNumSprite[i]->GetTexSize().x * digit[i];
		maxComboNumSprite[i]->SetTexLeftTop(leftTop);
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

	//イージングで枠のライン変更
	float alpha = Easing::OutQuad(0, 0.6f, easeTimer);

	//枠オブジェクトの大きさを更新
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

		//最大コンボ数スプライトを動かす状態にセット
		SetMoveMaxComboSprite();
	}
}

void ResultUI::MoveMaxComboSprite()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveMaxComboSpriteTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveMaxComboSpriteTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 maxComboPos = maxComboSprite->GetPosition();
	maxComboPos.x = Easing::OutQuint(-50, 600, easeTimer);
	//更新した座標をセット
	maxComboSprite->SetPosition(maxComboPos);

	for (int i = 0; i < maxComboDigits; i++)
	{
		XMFLOAT2 maxComboPos = maxComboNumSprite[i]->GetPosition();
		XMFLOAT2 maxComboSize = maxComboNumSprite[i]->GetSize();
		maxComboPos.x = Easing::OutQuint(1330 - i * maxComboSize.x, 800 - i * maxComboSize.x, easeTimer);
		//更新した座標をセット
		maxComboNumSprite[i]->SetPosition(maxComboPos);
	}

	//タイマーが指定した時間になったら
	if (moveMaxComboSpriteTimer >= moveTime)
	{
		//移動状態終了
		isMoveMaxComboSprite = false;

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

	XMFLOAT2 pressAPos = pressASprite->GetPosition();
	pressAPos.x = Easing::OutQuint(1330, 680, easeTimer);
	//更新した座標をセット
	pressASprite->SetPosition(pressAPos);

	//タイマーが指定した時間になったら
	if (moveRetrySpriteTimer >= moveTime)
	{
		//移動状態終了
		isMoveRetrySprite = false;

		//全ての描画完了
		isDrawAll = true;
	}
}
