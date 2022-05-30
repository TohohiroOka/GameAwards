#include "Rush.h"
#include "SafeDelete.h"
#include "Easing.h"

Rush* Rush::Create(int rushTexNum)
{
	//インスタンスを生成
	Rush* instance = new Rush();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(rushTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Rush::~Rush()
{
	safe_delete(rushSprite);
}

bool Rush::Initialize(int rushTexNum)
{
	//ゲーム説明スプライト生成
	rushSprite = Sprite::Create(rushTexNum);
	if (rushSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	rushSprite->SetTexSize({ 574, 183 });
	rushSprite->SetSize({ 287, 91.5f });
	rushSprite->SetPosition({ 1500, 190 });
	//スプライト更新
	rushSprite->Update();

	return true;
}

void Rush::Update()
{
	//画面内に移動状態
	if (isMoveInScreen)
	{
		MoveInScreen();
	}
	//画面外に移動状態
	else if (isMoveOutScreen)
	{
		MoveOutScreen();
	}

	//色変更
	if (isChangeColor)
	{
		ChangeColor();
	}

	//スプライト更新
	rushSprite->Update();
}

void Rush::Draw()
{
	//スプライト描画
	rushSprite->Draw();
}

void Rush::Reset()
{
	//画面内に移動中か
	isMoveInScreen = false;
	//画面内に移動する時間タイマー
	moveInScreenTimer = 0;
	//画面外に移動中か
	isMoveOutScreen = false;
	//画面外に移動する時間タイマー
	moveOutScreenTimer = 0;
	//画面内にいるか
	isInScreen = false;
	//色を変えるか
	isChangeColor = false;
	//色を変える時間
	changeColorTimer = 0;
	//薄くするか
	isAlphaDown = true;

	//スプライト初期化
	rushSprite->SetPosition({ 1500, 190 });
	rushSprite->SetColor({ 1,1,1,1 });
	rushSprite->Update();
}

void Rush::SetMoveInScreen()
{
	//タイマーを初期化
	moveInScreenTimer = 0;
	//画面内に移動中
	isMoveInScreen = true;
	//画面内にいる
	isInScreen = true;

	//色を変える時間タイマー初期化
	changeColorTimer = 0;
	//薄くする
	isAlphaDown = true;
	//スプライトの色を初期化
	rushSprite->SetColor({ 1,1,1,1 });
	rushSprite->Update();
}

void Rush::SetMoveOutScreen()
{
	//タイマーを初期化
	moveOutScreenTimer = 0;
	//画面外に移動中
	isMoveOutScreen = true;
	//画面内にいない
	isInScreen = false;
	//色変更終了
	isChangeColor = false;
}

void Rush::MoveInScreen()
{
	//移動を行う時間
	const int moveTime = 30;

	//タイマーを更新
	moveInScreenTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveInScreenTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 pos = rushSprite->GetPosition();
	pos.x = Easing::OutQuint(1500, 1000, easeTimer);
	//更新した座標をセット
	rushSprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (moveInScreenTimer >= moveTime)
	{
		//画面内に移動状態終了
		isMoveInScreen = false;

		//色変更開始
		isChangeColor = true;
	}
}

void Rush::MoveOutScreen()
{
	//移動を行う時間
	const int moveTime = 30;

	//タイマーを更新
	moveOutScreenTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveOutScreenTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 pos = rushSprite->GetPosition();
	pos.x = Easing::InQuint(1000, 1500, easeTimer);
	//更新した座標をセット
	rushSprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (moveOutScreenTimer >= moveTime)
	{
		//画面内に移動状態終了
		isMoveOutScreen = false;
	}
}

void Rush::ChangeColor()
{
	//色を変える時間
	const int changeTime = 100;

	//タイマーを更新
	changeColorTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeColorTimer / changeTime;

	//スプライトの色を変更
	XMFLOAT4 color = rushSprite->GetColor();

	//薄くする場合
	if (isAlphaDown)
	{
		color.w = Easing::OutQuint(1, 0.2f, easeTimer);
	}
	//濃くする場合
	else
	{
		color.w = Easing::OutQuint(0.2f, 1, easeTimer);
	}
	//更新した色をセット
	rushSprite->SetColor(color);

	//タイマーが指定した時間に到達したら
	if (changeColorTimer >= changeTime)
	{
		//濃くするのと薄くするのを反転
		if (isAlphaDown)
		{
			isAlphaDown = false;
		}
		else
		{
			isAlphaDown = true;
		}

		//タイマーを初期化
		changeColorTimer = 0;
	}
}
