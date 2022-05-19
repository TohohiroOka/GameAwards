#include "ReadyGo.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "Audio.h"

ReadyGo* ReadyGo::Create(int readyTexNum, int goTexNum)
{
	//インスタンスを生成
	ReadyGo* instance = new ReadyGo();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(readyTexNum, goTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ReadyGo::~ReadyGo()
{
	safe_delete(readySprite);
	safe_delete(goSprite);
}

bool ReadyGo::Initialize(int readyTexNum, int goTexNum)
{
	//Readyスプライト生成
	readySprite = Sprite::Create(readyTexNum);
	if (readySprite == nullptr) {
		return false;
	}
	//座標をセット
	readySprite->SetPosition({ 640, -150 });
	//テクスチャサイズをセット
	readySprite->SetTexSize({ 900, 195 });
	//大きさをセット
	readySprite->SetSize({ 900, 195 });
	//スプライト更新
	readySprite->Update();


	//Goスプライト生成
	goSprite = Sprite::Create(goTexNum);
	if (readySprite == nullptr) {
		return false;
	}
	//座標をセット
	goSprite->SetPosition({ 640, -150 });
	//テクスチャサイズをセット
	goSprite->SetTexSize({ 384, 192 });
	//大きさをセット
	goSprite->SetSize({ 384, 192 });
	//スプライト更新
	goSprite->Update();

	//Readyスプライトを動かす状態でセットしておく
	SetReadySpriteMove();

	//サウンドの読み込み
	Audio* audio = Audio::GetInstance();
	sound[0] = audio->SoundLoadWave("Resources/sound/go.wav");//GO音

	return true;
}

void ReadyGo::Update()
{
	if (!isReadySpriteMove && !isGoSpriteMove) { return; }

	//Readyスプライトを動かす
	if (isReadySpriteMove)
	{
		ReadySpriteMove();
	}
	//Goスプライトを動かす
	if (isGoSpriteMove)
	{
		GoSpriteMove();
	}

	//スプライト更新
	readySprite->Update();
	goSprite->Update();
}

void ReadyGo::Draw()
{
	if (!isReadySpriteMove && !isGoSpriteMove) { return; }

	//スプライト描画
	readySprite->Draw();
	goSprite->Draw();
}

void ReadyGo::Reset()
{
	//Readyスプライトを動かすか
	isReadySpriteMove = false;
	//Goスプライトを動かすか
	isGoSpriteMove = false;
	//Readyスプライトを動かす時間タイマー
	readySpriteMoveTimer = 0;
	//Goスプライトを動かす時間タイマー
	goSpriteMoveTimer = 0;
	//ReadyGoが終わったか
	isReadyGoEnd = false;

	//Readyスプライトを動かす状態でセットしておく
	SetReadySpriteMove();
}

void ReadyGo::SetReadySpriteMove()
{
	//Readyスプライトを動かす状態にセット
	isReadySpriteMove = true;
}

void ReadyGo::SetGoSpriteMove()
{
	//Goスプライトを動かす状態にセット
	isGoSpriteMove = true;
}

void ReadyGo::ReadySpriteMove()
{
	Audio* audio = Audio::GetInstance();

	//動かす時間
	const int moveTime = 120;

	//タイマーを更新
	readySpriteMoveTimer++;

	//イージングで動かす
	if (readySpriteMoveTimer < moveTime / 3 * 2)
	{
		//イージング計算用の時間
		const int fallTime = moveTime / 3 * 2;
		float easeTimer = (float)readySpriteMoveTimer / fallTime;

		//イージングで壁オブジェクトの大きさ変更
		XMFLOAT2 pos = readySprite->GetPosition();
		pos.y = Easing::OutQuint(-150, 340, easeTimer);
		//Readyスプライトを動かす
		readySprite->SetPosition(pos);
	}
	else if (readySpriteMoveTimer > moveTime / 3)
	{
		//イージング計算用の時間
		const int endTime = moveTime;
		float easeTimer = (float)(readySpriteMoveTimer - moveTime / 3 * 2) / endTime;

		//イージングで壁オブジェクトの大きさ変更
		XMFLOAT2 pos = readySprite->GetPosition();
		pos.y = Easing::OutQuint(340, 870, easeTimer);
		//Readyスプライトを動かす
		readySprite->SetPosition(pos);
	}

	//タイマーが指定の時間に到達したら
	if (readySpriteMoveTimer >= moveTime)
	{
		//移動終了
		isReadySpriteMove = false;

		//Goスプライトを動かす
		isGoSpriteMove = true;

		//サウンドの再生
		audio->SoundPlayWava(sound[0], false);
	}
}

void ReadyGo::GoSpriteMove()
{
	//動かす時間
	const int moveTime = 120;

	//タイマーを更新
	goSpriteMoveTimer++;

	//イージングで動かす
	if (goSpriteMoveTimer < moveTime / 3 * 2)
	{
		//イージング計算用の時間
		const int fallTime = moveTime / 3 * 2;
		float easeTimer = (float)goSpriteMoveTimer / fallTime;

		//イージングで壁オブジェクトの大きさ変更
		XMFLOAT2 pos = goSprite->GetPosition();
		pos.y = Easing::OutQuint(-150, 340, easeTimer);
		//Readyスプライトを動かす
		goSprite->SetPosition(pos);
	}
	else if (goSpriteMoveTimer > moveTime / 3)
	{
		//イージング計算用の時間
		const int endTime = moveTime;
		float easeTimer = (float)(goSpriteMoveTimer - moveTime / 3 * 2) / endTime;

		//イージングで壁オブジェクトの大きさ変更
		XMFLOAT2 pos = goSprite->GetPosition();
		pos.y = Easing::OutQuint(340, 870, easeTimer);
		//Readyスプライトを動かす
		goSprite->SetPosition(pos);
	}

	//タイマーが指定の時間に到達したら
	if (goSpriteMoveTimer >= moveTime)
	{
		//移動終了
		isGoSpriteMove = false;

		//ReadyGo終了
		isReadyGoEnd = true;
	}
}
