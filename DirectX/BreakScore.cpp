#include "BreakScore.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "Input.h"
#include "XInputManager.h"

BreakScore* BreakScore::Create(int breakTexNum, int numberTexNum)
{
	//インスタンスを生成
	BreakScore* instance = new BreakScore();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(breakTexNum, numberTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

BreakScore::~BreakScore()
{
	safe_delete(breakSprite);
	for (int i = 0; i < breakDigits; i++)
	{
		safe_delete(breakNumSprite[i]);
	}
}

bool BreakScore::Initialize(int breakTexNum, int numberTexNum)
{
	//BREAKスプライト生成
	breakSprite = Sprite::Create(breakTexNum);
	if (breakSprite == nullptr) {
		return false;
	}
	//座標をセット
	breakSprite->SetPosition({ 600, -100 });
	//テクスチャサイズをセット
	breakSprite->SetTexSize({ 234, 64 });
	//大きさをセット
	breakSprite->SetSize({ 175.5f, 48 });
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
		XMFLOAT2 size = { 36, 48 };
		breakNumSprite[i]->SetSize(size);
		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 48, 64 };
		breakNumSprite[i]->SetTexSize(texSize);
		//座標をセット
		XMFLOAT2 pos = { 700, -100 };
		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
		//スプライト更新
		breakNumSprite[i]->Update();
	}

	return true;
}

void BreakScore::Update()
{
	//スプライトを動かす
	if (isMoveSprite)
	{
		//降下
		if (moveSpriteScene == MoveSpriteScene::Fall)
		{
			SpriteFall();
		}
		//停止
		else if (moveSpriteScene == MoveSpriteScene::Stay)
		{
			SpriteStay();
		}
		//上昇
		else if (moveSpriteScene == MoveSpriteScene::Rize)
		{
			SpriteRize();
		}
	}

	//スプライト更新
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		breakNumSprite[i]->Update();
	}
}

void BreakScore::Draw()
{
	//スプライト描画
	breakSprite->Draw();

	if (breakScore >= 100)
	{
		breakNumSprite[2]->Draw();
	}
	if (breakScore >= 10)
	{
		breakNumSprite[1]->Draw();
	}
	breakNumSprite[0]->Draw();
}

void BreakScore::Reset()
{
	//壁破壊枚数
	breakScore = 0;

	//スプライトを動かすか
	isMoveSprite = false;
	//スプライトの行動
	moveSpriteScene = MoveSpriteScene::None;
	//スプライト降下タイマー
	spriteFallTimer = 0;
	//スプライト停止タイマー
	spriteStayTimer = 0;
	//スプライト上昇タイマー
	spriteRizeTimer = 0;

	//スプライト初期化
	//スプライトの数字を壁破壊数に合わせる
	ChangeBreakNumSprite();
	breakSprite->SetPosition({ 600, -100 });
	breakSprite->Update();
	for (int i = 0; i < breakDigits; i++)
	{
		//大きさをセット
		XMFLOAT2 size = breakNumSprite[i]->GetSize();
		//座標をセット
		XMFLOAT2 pos = { 700, -100 };
		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
		//スプライト更新
		breakNumSprite[i]->Update();
	}
}

void BreakScore::AddScore()
{
	//壁破壊数を1増やす
	breakScore++;
	//最大スコア数は越えない
	const int breakScoreMax = 999;
	if (breakScore > breakScoreMax)
	{
		breakScore = breakScoreMax;
	}
	//スプライトの数字を壁破壊数に合わせる
	ChangeBreakNumSprite();

	//スプライトを動かす状態にする
	SetMoveSprite();
}

void BreakScore::ChangeBreakNumSprite()
{
	//数字をそれぞれ出力する
	int digit[breakDigits];
	digit[0] = breakScore % 10;			//0001
	digit[1] = (breakScore / 10) % 10;	//0010
	digit[2] = (breakScore / 100) % 10;	//0100

	//それぞれの桁の数字分スプライトのテクスチャ切り出しをずらす
	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = breakNumSprite[i]->GetTexSize().x * digit[i];
		breakNumSprite[i]->SetTexLeftTop(leftTop);

		//表示桁数により座標をずらす
		XMFLOAT2 pos = breakNumSprite[i]->GetPosition();
		XMFLOAT2 size = breakNumSprite[i]->GetSize();
		const float basePosX = 720;
		if (breakScore >= 100) { pos.x = basePosX + size.x * 2; }
		else if (breakScore >= 10) { pos.x = basePosX + size.x; }
		else { pos.x = basePosX; }

		pos.x -= size.x * i;
		breakNumSprite[i]->SetPosition(pos);
	}
}

void BreakScore::SetMoveSprite()
{
	//タイマーを初期化
	//スプライト降下タイマー
	spriteFallTimer = 0;
	//スプライト停止タイマー
	spriteStayTimer = 0;
	//スプライト上昇タイマー
	spriteRizeTimer = 0;

	//行動を降下状態にセット
	moveSpriteScene = MoveSpriteScene::Fall;

	//動かす状態にセット
	isMoveSprite = true;
}

void BreakScore::SpriteFall()
{
	//降下を行う時間
	const int fallTime = 60;

	//タイマーを更新
	spriteFallTimer++;

	//イージング計算用の時間
	float easeTimer = (float)spriteFallTimer / fallTime;

	//スプライトの座標を変更
	XMFLOAT2 breakPos = breakSprite->GetPosition();
	breakPos.y = Easing::OutQuint(-100, 170, easeTimer);
	//更新した座標をセット
	breakSprite->SetPosition(breakPos);

	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 breakNumPos = breakNumSprite[i]->GetPosition();
		breakNumPos.y = Easing::OutQuint(-100, 170, easeTimer);
		//更新した座標をセット
		breakNumSprite[i]->SetPosition(breakNumPos);
	}

	//タイマーが指定した時間になったら
	if (spriteFallTimer >= fallTime)
	{
		//停止状態にする
		moveSpriteScene = MoveSpriteScene::Stay;
	}
}

void BreakScore::SpriteStay()
{
	//停止を行う時間
	const int stayTime = 90;

	//タイマーを更新
	spriteStayTimer++;

	//タイマーが指定した時間になったら
	if (spriteStayTimer >= stayTime)
	{
		//上昇状態にする
		moveSpriteScene = MoveSpriteScene::Rize;
	}
}

void BreakScore::SpriteRize()
{
	//上昇を行う時間
	const int rizeTime = 60;

	//タイマーを更新
	spriteRizeTimer++;

	//イージング計算用の時間
	float easeTimer = (float)spriteRizeTimer / rizeTime;

	//スプライトの座標を変更
	XMFLOAT2 breakPos = breakSprite->GetPosition();
	breakPos.y = Easing::OutQuint(170, -100, easeTimer);
	//更新した座標をセット
	breakSprite->SetPosition(breakPos);

	for (int i = 0; i < breakDigits; i++)
	{
		XMFLOAT2 breakNumPos = breakNumSprite[i]->GetPosition();
		breakNumPos.y = Easing::OutQuint(170, -100, easeTimer);
		//更新した座標をセット
		breakNumSprite[i]->SetPosition(breakNumPos);
	}

	//タイマーが指定した時間になったら
	if (spriteRizeTimer >= rizeTime)
	{
		//スプライトを動かす状態を解除
		isMoveSprite = false;

		//行動を停止
		moveSpriteScene = MoveSpriteScene::None;
	}
}
