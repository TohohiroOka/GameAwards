#include "TimeLimit.h"
#include "SafeDelete.h"
#include "Easing.h"

TimeLimit* TimeLimit::Create(int textureNum)
{
	//インスタンスを生成
	TimeLimit* instance = new TimeLimit();
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

TimeLimit::~TimeLimit()
{
	for (int i = 0; i < timeDigits; i++)
	{
		safe_delete(timeSprite[i]);
	}
}

bool TimeLimit::Initialize(int textureNum)
{
	//桁数分回す
	for (int i = 0; i < timeDigits; i++)
	{
		//制限時間用スプライト生成
		timeSprite[i] = Sprite::Create(textureNum);
		if (timeSprite[i] == nullptr) {
			return false;
		}

		//大きさをセット
		XMFLOAT2 size = { 32, 48 };
		timeSprite[i]->SetSize(size);

		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 48, 64 };
		timeSprite[i]->SetTexSize(texSize);

		//座標をセット
		XMFLOAT2 pos = { 640 + size.x / 2, -100 };
		pos.x -= size.x * i;
		timeSprite[i]->SetPosition(pos);
	}

	//スプライト更新
	ChangeTimeSprite();
	for (int i = 0; i < timeDigits; i++)
	{
		timeSprite[i]->Update();
	}

	return true;
}

void TimeLimit::Update()
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

	if (isCountDown)
	{
		//毎フレーム制限時間を減らしていく
		CountDown();
		ChangeTimeSprite();
	}

	//スプライト更新
	for (int i = 0; i < timeDigits; i++)
	{
		timeSprite[i]->Update();
	}
}

void TimeLimit::Draw()
{
	//スプライト描画
	for (int i = 0; i < timeDigits; i++)
	{
		timeSprite[i]->Draw();
	}
}

void TimeLimit::Reset()
{
	//時間計測タイマー
	timer = timeLimitMax;
	//カウントダウンするか
	isCountDown = false;
	//ゲームシーンの座標に移動中か
	isMoveGamePos = false;
	//ゲームシーンの座標に移動終了したか
	isMoveGamePosEnd = false;
	//ゲームシーンの座標に移動する時間タイマー
	moveGamePosTimer = 0;
	//リザルトシーンの座標に移動中か
	isMoveResultPos = false;
	//リザルトシーンの座標に移動終了したか
	isMoveResultPosEnd = false;
	//リザルトシーンの座標に移動する時間タイマー
	moveResultPosTimer = 0;
	//表示用スプライトを0に戻す
	ChangeTimeSprite();
	//スプライト更新
	for (int i = 0; i < timeDigits; i++)
	{
		timeSprite[i]->Update();
	}
}

void TimeLimit::Recovery(int second)
{
	//引数の秒数、制限時間を回復する
	timer += second * 60;

	//最大以上を回復しない
	if (timer >= timeLimitMax)
	{
		timer = timeLimitMax;
	}
}

void TimeLimit::SetMoveGamePos()
{
	//ゲームシーンの座標に移動する時間タイマーを初期化
	moveGamePosTimer = 0;

	//移動状態にセット
	isMoveGamePos = true;
}

void TimeLimit::SetMoveResultPos()
{
	//リザルトシーンの座標に移動する時間タイマーを初期化
	moveResultPosTimer = 0;

	//移動状態にセット
	isMoveResultPos = true;
}

void TimeLimit::CountDown()
{
	//毎フレームタイマーをカウントダウン
	timer--;

	//0以下にはならないようにしておく
	if (timer <= 0)
	{
		timer = 0;
	}
}

void TimeLimit::ChangeTimeSprite()
{
	//桁の数字をそれぞれ出力する
	int digit[timeDigits];
	digit[0] = timer / 60 % 10;			//01
	digit[1] = timer / (60 * 10) % 10;	//10

	//それぞれの桁の数字分スプライトのテクスチャ切り出しをずらす
	for (int i = 0; i < timeDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = timeSprite[i]->GetTexSize().x * digit[i];
		timeSprite[i]->SetTexLeftTop(leftTop);
	}
}

void TimeLimit::MoveGamePos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveGamePosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//スプライトの座標を変更
	for (int i = 0; i < timeDigits; i++)
	{
		XMFLOAT2 timePos = timeSprite[i]->GetPosition();
		timePos.y = Easing::OutQuint(-83, 72, easeTimer);
		timeSprite[i]->SetPosition(timePos);
	}

	//タイマーが指定した時間になったら
	if (moveGamePosTimer >= moveTime)
	{
		//移動状態終了
		isMoveGamePos = false;

		//移動完了
		isMoveGamePosEnd = true;
	}
}

void TimeLimit::MoveResultPos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveResultPosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//スプライトの座標を変更
	for (int i = 0; i < timeDigits; i++)
	{
		XMFLOAT2 timePos = timeSprite[i]->GetPosition();
		timePos.y = Easing::OutQuint(72, -83, easeTimer);
		timeSprite[i]->SetPosition(timePos);
	}

	//タイマーが指定した時間になったら
	if (moveResultPosTimer >= moveTime)
	{
		//移動状態終了
		isMoveResultPos = false;

		//移動完了
		isMoveResultPosEnd = true;
	}
}
