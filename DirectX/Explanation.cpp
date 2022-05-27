#include "Explanation.h"
#include "SafeDelete.h"
#include "Easing.h"

Explanation* Explanation::Create(int explanationTexNum, int XBunttonTexNum)
{
	//インスタンスを生成
	Explanation* instance = new Explanation();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(explanationTexNum, XBunttonTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Explanation::~Explanation()
{
	safe_delete(explanationSprite);
	safe_delete(XButtonSprite);
}

bool Explanation::Initialize(int explanationTexNum, int XBunttonTexNum)
{
	//ゲーム説明スプライト生成
	explanationSprite = Sprite::Create(explanationTexNum);
	if (explanationSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	explanationSprite->SetTexSize({ 746, 75 });
	explanationSprite->SetSize({ 373, 38 });
	explanationSprite->SetPosition({ 1500, 190 });
	//スプライト更新
	explanationSprite->Update();


	//Xボタンスプライト生成
	XButtonSprite = Sprite::Create(XBunttonTexNum);
	if (explanationSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	XButtonSprite->SetTexSize({ 300, 112 });
	XButtonSprite->SetSize({ 150, 66 });
	XButtonSprite->SetPosition({ 1500, 260 });
	//スプライト更新
	XButtonSprite->Update();

	return true;
}

void Explanation::Update()
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

	//スプライト更新
	explanationSprite->Update();
	XButtonSprite->Update();
}

void Explanation::Draw()
{
	//スプライト描画
	explanationSprite->Draw();
	XButtonSprite->Draw();
}

void Explanation::Reset()
{
	//画面内に移動中か
	isMoveInScreen = false;
	//画面内に移動する時間タイマー
	moveInScreenTimer = 0;
	//画面外に移動中か
	isMoveOutScreen = false;
	//画面外に移動する時間タイマー
	moveOutScreenTimer = 0;

	//スプライト初期化
	explanationSprite->SetPosition({ 1500, 190 });
	explanationSprite->Update();
	XButtonSprite->SetPosition({ 1500, 260 });
	XButtonSprite->Update();
}

void Explanation::SetMoveInScreen()
{
	//タイマーを初期化
	moveInScreenTimer = 0;

	//画面内に移動中
	isMoveInScreen = true;
}

void Explanation::SetMoveOutScreen()
{
	//タイマーを初期化
	moveOutScreenTimer = 0;

	//画面外に移動中
	isMoveOutScreen = true;
}

void Explanation::MoveInScreen()
{
	//移動を行う時間
	const int moveTime = 30;

	//タイマーを更新
	moveInScreenTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveInScreenTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 explanationPos = explanationSprite->GetPosition();
	explanationPos.x = Easing::OutQuint(1500, 1000, easeTimer);
	XMFLOAT2 XButtonPos = XButtonSprite->GetPosition();
	XButtonPos.x = Easing::OutQuint(1500, 1000, easeTimer);
	//更新した座標をセット
	explanationSprite->SetPosition(explanationPos);
	XButtonSprite->SetPosition(XButtonPos);

	//タイマーが指定した時間になったら
	if (moveInScreenTimer >= moveTime)
	{
		//画面内に移動状態終了
		isMoveInScreen = false;
	}
}

void Explanation::MoveOutScreen()
{
	//移動を行う時間
	const int moveTime = 30;

	//タイマーを更新
	moveOutScreenTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveOutScreenTimer / moveTime;

	//スプライトの座標を変更
	XMFLOAT2 explanationPos = explanationSprite->GetPosition();
	explanationPos.x = Easing::InQuint(1000, 1500, easeTimer);
	XMFLOAT2 XButtonPos = XButtonSprite->GetPosition();
	XButtonPos.x = Easing::InQuint(1000, 1500, easeTimer);
	//更新した座標をセット
	explanationSprite->SetPosition(explanationPos);
	XButtonSprite->SetPosition(XButtonPos);

	//タイマーが指定した時間になったら
	if (moveOutScreenTimer >= moveTime)
	{
		//画面内に移動状態終了
		isMoveOutScreen = false;
	}
}
