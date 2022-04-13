#include "ResultUI.h"
#include "SafeDelete.h"
#include "Easing.h"

ResultUI* ResultUI::Create(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int pressButtonTexNum)
{
	//インスタンスを生成
	ResultUI* instance = new ResultUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(plainTexNum, resultTexNum, scoreTexNum, numberTexNum, pressButtonTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ResultUI::~ResultUI()
{
	safe_delete(blackoutSprite);
	safe_delete(resultSprite);
	safe_delete(SCORESprite);

	for (int i = 0; i < scoreDigits; i++)
	{
		safe_delete(finalScoreSprite[i]);
	}
	safe_delete(pressButtonSprite);
}

bool ResultUI::Initialize(int plainTexNum, int resultTexNum, int scoreTexNum, int numberTexNum, int pressButtonTexNum)
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


	//リザルトスプライト生成
	resultSprite = Sprite::Create(resultTexNum);
	if (resultSprite == nullptr) {
		return false;
	}
	//座標をセット
	resultSprite->SetPosition({ 640, 150 });
	//テクスチャサイズをセット
	resultSprite->SetTexSize({ 178, 38 });
	//大きさをセット
	resultSprite->SetSize({ 356, 76 });


	//SCORE:スプライト生成
	SCORESprite = Sprite::Create(scoreTexNum);
	if (SCORESprite == nullptr) {
		return false;
	}
	//座標をセット
	SCORESprite->SetPosition({ 500, 350 });
	//テクスチャサイズをセット
	SCORESprite->SetTexSize({ 178, 38 });
	//大きさをセット
	SCORESprite->SetSize({ 178, 38 });


	//最終スコアスプライト生成
	//スコアの桁数分回す
	for (int i = 0; i < scoreDigits; i++)
	{
		//スコア表示用スプライト生成
		finalScoreSprite[i] = Sprite::Create(numberTexNum);
		if (finalScoreSprite[i] == nullptr) {
			return false;
		}

		//大きさをセット
		XMFLOAT2 size = { 32, 64 };
		finalScoreSprite[i]->SetSize(size);

		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 32, 64 };
		finalScoreSprite[i]->SetTexSize(texSize);

		//座標をセット
		XMFLOAT2 pos = { 852, 350 };
		pos.x -= size.x * i;
		finalScoreSprite[i]->SetPosition(pos);
	}


	//pressButtonスプライト生成
	pressButtonSprite = Sprite::Create(pressButtonTexNum);
	if (pressButtonSprite == nullptr) {
		return false;
	}
	//座標をセット
	pressButtonSprite->SetPosition({ 640, 500 });
	//テクスチャサイズをセット
	pressButtonSprite->SetTexSize({ 284, 38 });
	//大きさをセット
	pressButtonSprite->SetSize({ 284, 38 });


	//スプライト更新しておく
	blackoutSprite->Update();
	resultSprite->Update();
	SCORESprite->Update();
	for (int i = 0; i < scoreDigits; i++)
	{
		finalScoreSprite[i]->Update();
	}
	pressButtonSprite->Update();

	return true;
}

void ResultUI::Update()
{
	if (drawScene == DrawScene::BlackOut)
	{
		//暗転
		BlackOutUpdate();
	}
	else if (drawScene == DrawScene::ResultDraw)
	{
		//一定時間経ったら次の描画を始める
		TimeCount();
	}
	else if (drawScene == DrawScene::FinalScoreDraw)
	{
		//表示用スコアの加算演出
		IncreaseDisplayScore();
		//スプライトを表示用スコアに合わせる
		IncreaseScoreSprite();
	}
	else if (drawScene == DrawScene::Stay)
	{
		//一定時間経ったら次の描画を始める
		TimeCount();
	}
	else if (drawScene == DrawScene::PressButtonDraw)
	{
		isDrawAll = true;
	}

	//スプライト更新
	blackoutSprite->Update();
	if (drawScene >= DrawScene::ResultDraw)
	{
		resultSprite->Update();
	}
	if (drawScene >= DrawScene::FinalScoreDraw)
	{
		SCORESprite->Update();

		for (int i = 0; i < scoreDigits; i++)
		{
			finalScoreSprite[i]->Update();
		}
	}
	if (drawScene >= DrawScene::PressButtonDraw)
	{
		pressButtonSprite->Update();
	}
}

void ResultUI::Draw()
{
	//スプライト描画
	blackoutSprite->Draw();
	if (drawScene >= DrawScene::ResultDraw)
	{
		resultSprite->Draw();
	}
	if (drawScene >= DrawScene::FinalScoreDraw)
	{
		SCORESprite->Draw();

		for (int i = 0; i < scoreDigits; i++)
		{
			finalScoreSprite[i]->Draw();
		}
	}
	if (drawScene >= DrawScene::PressButtonDraw)
	{
		pressButtonSprite->Draw();
	}
}

void ResultUI::Reset()
{
	//暗転を解除する
	blackoutSprite->SetColor({ 0, 0, 0, 0 });
	//最終スコア初期化
	finalScore = 0;
	//表示用スコア初期化
	displayScore = 0;
	//表示用スプライトを0に戻す
	IncreaseScoreSprite();
	//時間計測タイマー初期化
	timer = 0;
	//シーンを初期化
	drawScene = DrawScene::BlackOut;
	//フラグをfalseに
	isDrawAll = false;
	//スプライト更新
	blackoutSprite->Update();
	resultSprite->Update();
	SCORESprite->Update();
	for (int i = 0; i < scoreDigits; i++)
	{
		finalScoreSprite[i]->Update();
	}
	pressButtonSprite->Update();
}

void ResultUI::SetFinalScore(int finalScore)
{
	this->finalScore = finalScore;
}

void ResultUI::BlackOutUpdate()
{
	//暗転を行う時間
	const int blackoutTime = 200;

	//タイマー更新
	timer++;

	//イージング計算用の時間
	float easeTimer = (float)timer / blackoutTime;

	//イージングで枠のライン変更
	float alpha = Easing::OutQuad(0, 0.6f, easeTimer);

	//枠オブジェクトの大きさを更新
	blackoutSprite->SetColor({ 0, 0, 0, alpha });

	//タイマーが指定した時間になったら
	if (timer >= blackoutTime)
	{
		//タイマーを初期化
		timer = 0;

		//次の描画を開始
		drawScene++;
	}
}

void ResultUI::TimeCount()
{
	//経過時間を更新する
	timer++;

	//指定した時間になったら
	const int maxTime = 60;
	if (timer >= maxTime)
	{
		//タイマーを初期化
		timer = 0;

		//次の描画を開始
		drawScene++;
	}
}

void ResultUI::IncreaseDisplayScore()
{
	//表示用スコアを更新
	displayScore += 952672;
	//表示用スコアは最終スコアを越えない
	if (displayScore >= finalScore)
	{
		displayScore = finalScore;

		//次の描画を開始
		drawScene++;
	}
}

void ResultUI::IncreaseScoreSprite()
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
		leftTop.x = finalScoreSprite[i]->GetTexSize().x * digit[i];
		finalScoreSprite[i]->SetTexLeftTop(leftTop);
	}
}

