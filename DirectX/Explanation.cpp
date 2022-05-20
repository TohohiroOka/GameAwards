#include "Explanation.h"
#include "SafeDelete.h"
#include "Easing.h"

Explanation* Explanation::Create(int explanationTexNum)
{
	//インスタンスを生成
	Explanation* instance = new Explanation();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(explanationTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Explanation::~Explanation()
{
	safe_delete(explanationSprite);
}

bool Explanation::Initialize(int explanationTexNum)
{
	//ゲーム説明スプライト生成
	explanationSprite = Sprite::Create(explanationTexNum, { 0, 0.5f });
	if (explanationSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	explanationSprite->SetPosition({ 1280, 250 });
	explanationSprite->SetSize({ 373, 38 });
	explanationSprite->SetTexSize({ 746, 75 });
	//スプライト更新
	explanationSprite->Update();

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
}

void Explanation::Draw()
{
	//スプライト描画
	explanationSprite->Draw();
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
	explanationSprite->SetPosition({ 1280, 250 });
	explanationSprite->Update();
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
	XMFLOAT2 pos = explanationSprite->GetPosition();
	pos.x = Easing::OutQuint(1280, 820, easeTimer);
	//更新した座標をセット
	explanationSprite->SetPosition(pos);

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
	XMFLOAT2 pos = explanationSprite->GetPosition();
	pos.x = Easing::InQuint(820, 1280, easeTimer);
	//更新した座標をセット
	explanationSprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (moveOutScreenTimer >= moveTime)
	{
		//画面内に移動状態終了
		isMoveOutScreen = false;
	}
}
