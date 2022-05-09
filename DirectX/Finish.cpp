#include "Finish.h"
#include "SafeDelete.h"
#include "Easing.h"

Finish* Finish::Create(int finishTexNum)
{
	//インスタンスを生成
	Finish* instance = new Finish();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(finishTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Finish::~Finish()
{
	safe_delete(finishSprite);
}

bool Finish::Initialize(int finishTexNum)
{
	//Finishスプライト生成
	finishSprite = Sprite::Create(finishTexNum);
	if (finishSprite == nullptr) {
		return false;
	}
	//座標をセット
	finishSprite->SetPosition({ 640, -150 });
	//テクスチャサイズをセット
	finishSprite->SetTexSize({ 255, 63 });
	//大きさをセット
	finishSprite->SetSize({ 255, 63 });
	//スプライト更新
	finishSprite->Update();

	//Finishスプライトを動かす状態でセットしておく
	SetFinishSpriteMove();

	return true;
}

void Finish::Update()
{
	if (!isFinishSpriteMove) { return; }

	//Finishスプライトを動かす
	if (isFinishSpriteMove)
	{
		FinishSpriteMove();
	}

	//スプライト更新
	finishSprite->Update();
}

void Finish::Draw()
{
	if (!isFinishSpriteMove) { return; }

	//スプライト描画
	finishSprite->Draw();
}

void Finish::Reset()
{
	//Finishスプライトを動かすか
	isFinishSpriteMove = false;
	//Finishが終わったか
	isFinishSpriteMoveEnd = false;
	//Finishスプライトを動かす時間タイマー
	finishSpriteMoveTimer = 0;

	//Finishスプライトを動かす状態でセットしておく
	SetFinishSpriteMove();
}

void Finish::SetFinishSpriteMove()
{
	//Finishスプライトを動かす状態にセット
	isFinishSpriteMove = true;
}

void Finish::FinishSpriteMove()
{
	//動かす時間
	const int moveTime = 180;

	//タイマーを更新
	finishSpriteMoveTimer++;

	//イージングで動かす
	if (finishSpriteMoveTimer < moveTime / 3 * 2)
	{
		//イージング計算用の時間
		const int fallTime = moveTime / 3 * 2;
		float easeTimer = (float)finishSpriteMoveTimer / fallTime;

		//イージングで壁オブジェクトの大きさ変更
		XMFLOAT2 pos = finishSprite->GetPosition();
		pos.y = Easing::OutQuint(-150, 340, easeTimer);
		//Finishスプライトを動かす
		finishSprite->SetPosition(pos);
	}
	else if (finishSpriteMoveTimer > moveTime / 3)
	{
		//イージング計算用の時間
		const int endTime = moveTime;
		float easeTimer = (float)(finishSpriteMoveTimer - moveTime / 3 * 2) / endTime;

		//イージングで壁オブジェクトの大きさ変更
		XMFLOAT2 pos = finishSprite->GetPosition();
		pos.y = Easing::OutQuint(340, 870, easeTimer);
		//Finishスプライトを動かす
		finishSprite->SetPosition(pos);
	}

	//タイマーが指定の時間に到達したら
	if (finishSpriteMoveTimer >= moveTime)
	{
		//移動状態終了
		isFinishSpriteMove = false;

		//移動完了
		isFinishSpriteMoveEnd = true;
	}
}
