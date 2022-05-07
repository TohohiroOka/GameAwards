#include "BreakScore.h"
#include "SafeDelete.h"
#include "Easing.h"

using namespace DirectX;

BreakScore* BreakScore::Create(int numberTexNum, int breakTexNum)
{
	//インスタンスを生成
	BreakScore* instance = new BreakScore();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(numberTexNum, breakTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

BreakScore::~BreakScore()
{
	for (int i = 0; i < scoreDigits; i++)
	{
		safe_delete(scoreSprite[i]);
	}

	safe_delete(breakSprite);
}

bool BreakScore::Initialize(int numberTexNum, int breakTexNum)
{
	//スコアの桁数分回す
	for (int i = 0; i < scoreDigits; i++)
	{
		//スコア表示用スプライト生成
		scoreSprite[i] = Sprite::Create(numberTexNum);
		if (scoreSprite[i] == nullptr) {
			return false;
		}

		//大きさをセット
		XMFLOAT2 size = { 32, 64 };
		scoreSprite[i]->SetSize(size);

		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 48, 64 };
		scoreSprite[i]->SetTexSize(texSize);

		//座標をセット
		XMFLOAT2 pos = { 400, -100 };
		pos.x -= size.x * i;
		scoreSprite[i]->SetPosition(pos);
		//スプライト更新
		scoreSprite[i]->Update();
	}

	//BREAKスプライト生成
	breakSprite = Sprite::Create(breakTexNum);
	if (breakSprite == nullptr) {
		return false;
	}

	//大きさをセット
	XMFLOAT2 size = { 250, 50 };
	breakSprite->SetSize(size);
	//テクスチャサイズをセット
	XMFLOAT2 texSize = { 234, 64 };
	breakSprite->SetTexSize(texSize);
	//座標をセット
	XMFLOAT2 pos = { 152, -100 };
	breakSprite->SetPosition(pos);
	//スプライト更新
	breakSprite->Update();

	//スコア初期化
	score = 0;

	return true;
}

void BreakScore::Update()
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
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Update();
	}
	breakSprite->Update();
}

void BreakScore::Draw()
{
	//スプライト描画
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Draw();
	}
	breakSprite->Draw();
}

void BreakScore::Reset()
{
	//スコア初期化
	score = 0;
	//表示用スプライトを0に戻す
	ChangeScoreSprite();
	//スプライト更新
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Update();
	}
}

void BreakScore::AddScore()
{	//スコアを更新
	score++;
	//スコアは最大数を越えない
	const int max = 9999;
	if (score >= max)
	{
		score = max;
	}

	//変更前の表示用
	ChangeScoreSprite();
}

void BreakScore::SetMoveGamePos()
{
	//ゲームシーンの座標に移動する時間タイマーを初期化
	moveGamePosTimer = 0;

	//移動状態にセット
	isMoveGamePos = true;
}

void BreakScore::SetMoveResultPos()
{
	//リザルトシーンの座標に移動する時間タイマーを初期化
	moveResultPosTimer = 0;

	//移動状態にセット
	isMoveResultPos = true;
}

void BreakScore::ChangeScoreSprite()
{
	//8桁の数字をそれぞれ出力する
	int digit[scoreDigits];
	digit[0] = score % 10;				//0001
	digit[1] = (score / 10) % 10;		//0010
	digit[2] = (score / 100) % 10;		//0100
	digit[3] = (score / 1000) % 10;		//1000

	//それぞれの桁の数字分スプライトのテクスチャ切り出しをずらす
	for (int i = 0; i < scoreDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = scoreSprite[i]->GetTexSize().x * digit[i];
		scoreSprite[i]->SetTexLeftTop(leftTop);
	}
}

void BreakScore::MoveGamePos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveGamePosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//スプライトの座標を変更
	for (int i = 0; i < scoreDigits; i++)
	{
		XMFLOAT2 scorePos = scoreSprite[i]->GetPosition();
		scorePos.y = Easing::OutQuint(-100, 55, easeTimer);
		scoreSprite[i]->SetPosition(scorePos);
	}
	XMFLOAT2 breakPos = breakSprite->GetPosition();
	breakPos.y = Easing::OutQuint(-100, 55, easeTimer);
	breakSprite->SetPosition(breakPos);

	//タイマーが指定した時間になったら
	if (moveGamePosTimer >= moveTime)
	{
		//移動状態終了
		isMoveGamePos = false;

		//移動完了
		isMoveGamePosEnd = true;
	}
}

void BreakScore::MoveResultPos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveResultPosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//スプライトの座標を変更
	for (int i = 0; i < scoreDigits; i++)
	{
		XMFLOAT2 scorePos = scoreSprite[i]->GetPosition();
		scorePos.y = Easing::OutQuint(55, -100, easeTimer);
		scoreSprite[i]->SetPosition(scorePos);
	}
	XMFLOAT2 breakPos = breakSprite->GetPosition();
	breakPos.y = Easing::OutQuint(55, -100, easeTimer);
	breakSprite->SetPosition(breakPos);

	//タイマーが指定した時間になったら
	if (moveResultPosTimer >= moveTime)
	{
		//移動状態終了
		isMoveResultPos = false;

		//移動完了
		isMoveResultPosEnd = true;
	}
}
