#include "TimeLimit.h"
#include "SafeDelete.h"

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
		XMFLOAT2 size = { 32, 64 };
		timeSprite[i]->SetSize(size);

		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 32, 64 };
		timeSprite[i]->SetTexSize(texSize);

		//座標をセット
		XMFLOAT2 pos = { 656, 55 };
		pos.x -= size.x * i;
		timeSprite[i]->SetPosition(pos);
	}

	return true;
}

void TimeLimit::Update()
{
	//毎フレーム制限時間を減らしていく
	CountDown();
	ChangeTimeSprite();

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
