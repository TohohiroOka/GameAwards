#include "Score.h"
#include "SafeDelete.h"
#include "Easing.h"

using namespace DirectX;

Score* Score::Create(int textureNum)
{
	//インスタンスを生成
	Score* instance = new Score();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(textureNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Score::~Score()
{
	for (int i = 0; i < scoreDigits; i++)
	{
		safe_delete(scoreSprite[i]);
	}
}

bool Score::Initialize(int textureNum)
{
	//スコアの桁数分回す
	for (int i = 0; i < scoreDigits; i++)
	{
		//スコア表示用スプライト生成
		scoreSprite[i] = Sprite::Create(textureNum);
		if (scoreSprite[i] == nullptr) {
			return false;
		}

		//大きさをセット
		XMFLOAT2 size = { 32, 64 };
		scoreSprite[i]->SetSize(size);

		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 32, 64 };
		scoreSprite[i]->SetTexSize(texSize);

		//座標をセット
		XMFLOAT2 pos = { 752, 55 };
		pos.x -= size.x * i;
		scoreSprite[i]->SetPosition(pos);
	}

	//スコア初期化
	score = 0;

	return true;
}

void Score::Update()
{
	//表示用スコア変更状態なら
	if (isChangeDisplayScore)
	{
		//スコア変更
		ChangeScore();
		ChangeScoreSprite();
	}

	//スプライト更新
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Update();
	}
}

void Score::Draw()
{
	//スプライト描画
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Draw();
	}
}

void Score::Reset()
{
	//スコア初期化
	score = 0;
	//表示用スコア初期化
	displayScore = 0;
	//表示用スプライトを0に戻す
	ChangeScoreSprite();
	//変更前の表示用スコア初期化
	changeDisplayScoreStart = 0;
	//変更後の表示用スコア初期化
	changeDisplayScoreEnd = 0;
	//表示用スコア変更用のタイマー初期化
	changeDisplayScoreTimer = 0;
	//表示用スコア変更中ではない
	isChangeDisplayScore = false;
	//スプライト更新
	for (int i = 0; i < scoreDigits; i++)
	{
		scoreSprite[i]->Update();
	}
}

void Score::SetAddScore(int addScore)
{	//スコアを更新
	score += addScore;
	//スコアは最大数を越えない
	const int max = 99999999;
	if (score >= max)
	{
		score = max;
	}

	//既に表示用スコアがカンストしていたら処理を飛ばす
	if (displayScore >= max) { return; }
	//既に変更後の表示用スコアがカンストしていたら処理を飛ばす
	if (changeDisplayScoreEnd >= max) { return; }

	//変更前の表示用スコアをセット
	changeDisplayScoreStart = displayScore;
	//変更後の表示用スコアをセット
	changeDisplayScoreEnd = score;


	//表示用スコア変更タイマーを初期化
	changeDisplayScoreTimer = 0;
	//表示用スコアを変更中にする
	isChangeDisplayScore = true;
}

void Score::ChangeScore()
{
	//表示用スコア変更を行う時間
	const int changeTime = 30;

	//表示用スコア変更タイマー更新
	changeDisplayScoreTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeDisplayScoreTimer / changeTime;

	//イージングでスコア変更
	float newScore = Easing::Lerp((float)changeDisplayScoreStart, (float)changeDisplayScoreEnd, easeTimer);

	//表示用スコアを更新
	displayScore = (int)newScore;
	//表示用スコアは最大数を越えない
	if (displayScore >= changeDisplayScoreEnd)
	{
		displayScore = changeDisplayScoreEnd;
	}

	//タイマーが指定した時間になったら
	if (changeDisplayScoreTimer >= changeTime)
	{
		//表示用スコアのずれを修正
		if (displayScore != changeDisplayScoreEnd)
		{
			displayScore = changeDisplayScoreEnd;
		}

		//表示用スコア変更状態終了
		isChangeDisplayScore = false;
	}
}

void Score::ChangeScoreSprite()
{
	//8桁の数字をそれぞれ出力する
	int digit[scoreDigits];
	digit[0] = displayScore % 10;				//00000001
	digit[1] = (displayScore / 10) % 10;		//00000010
	digit[2] = (displayScore / 100) % 10;		//00000100
	digit[3] = (displayScore / 1000) % 10;		//00001000
	digit[4] = (displayScore / 10000) % 10;		//00010000
	digit[5] = (displayScore / 100000) % 10;	//00100000
	digit[6] = (displayScore / 1000000) % 10;	//01000000
	digit[7] = (displayScore / 10000000) % 10;	//10000000

	//それぞれの桁の数字分スプライトのテクスチャ切り出しをずらす
	for (int i = 0; i < scoreDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = scoreSprite[i]->GetTexSize().x * digit[i];
		scoreSprite[i]->SetTexLeftTop(leftTop);
	}
}
