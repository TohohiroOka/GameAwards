#include "Energy.h"
#include "SafeDelete.h"
#include "Easing.h"

Energy* Energy::Create(int frameTexNum, int barTexNum)
{
	//インスタンスを生成
	Energy* instance = new Energy();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(frameTexNum, barTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Energy::~Energy()
{
	safe_delete(frameSprite);
	safe_delete(barSprite);
}

bool Energy::Initialize(int frameTexNum, int barTexNum)
{
	//ポイント表示(枠)スプライト生成
	frameSprite = Sprite::Create(frameTexNum, { 0, 0.5f });
	if (frameSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	frameSprite->SetSize({ 568, 52 });
	frameSprite->SetTexSize({ 568, 52 });
	frameSprite->SetPosition({ 290 - frameSprite->GetSize().x / 2, 55 });


	//ポイント表示(バー)スプライト生成
	barSprite = Sprite::Create(barTexNum, { 0, 0.5f });
	if (barSprite == nullptr) {
		return false;
	}
	//初期座標をセット
	barSprite->SetSize({ 0, 40 });
	barSprite->SetTexSize({ 0, 40 });
	barSprite->SetPosition({ 290 - lengthMax / 2, 55 });

	return true;
}

void Energy::Update()
{
	//長さを変更
	if (isChangeLengthBar)
	{
		ChangeLengthBar();
	}

	//スプライト更新
	frameSprite->Update();
	barSprite->Update();
}

void Energy::Draw()
{
	//スプライト描画
	frameSprite->Draw();
	barSprite->Draw();
}

void Energy::Reset()
{
}

void Energy::AddEnergyPoint(const int addPoint)
{
	//引数の数を加算する
	energyPoint += addPoint;

	//エネルギーポイントの上限を越えないようにする
	if (energyPoint >= energyPointMax)
	{
		energyPoint = energyPointMax;
	}

	//バーの長さ変更状態にする
	SetChangeLength();
}

void Energy::LoseEnergyPoint()
{
	energyPoint -= 10;

	//エネルギーポイントは0以下にならない
	if (energyPoint <= 0)
	{
		energyPoint = 0;
	}

	//バーの長さ変更状態にする
	SetChangeLength();
}

void Energy::UseTwentyPercent()
{
	//最大値の20%のポイント分減らす
	energyPoint -= energyPointMax / 5;

	if (energyPoint <= 0)
	{
		energyPoint = 0;
	}

	//バーの長さ変更状態にする
	SetChangeLength();
}

bool Energy::CheckPercent(int percent)
{
	//最大値の○○%のポイントを持っていたらtrueを返す
	float pointPer = ((float)energyPoint / energyPointMax);
	float checkPer = ((float)percent / 100);
	if (pointPer >= checkPer)
	{
		return true;
	}

	return false;
}

void Energy::ChangeLengthBar()
{
	//変更を行う時間
	const int changeTime = 60;

	//長さ変更タイマー更新
	changeLengthTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeLengthTimer / changeTime;

	//スプライトのサイズを変更
	XMFLOAT2 size = barSprite->GetSize();
	XMFLOAT2 texSize = barSprite->GetTexSize();
	size.x = Easing::OutQuint(changeLengthBefore, changeLengthAftar, easeTimer);
	texSize.x = Easing::OutQuint(changeLengthBefore, changeLengthAftar, easeTimer);
	//更新したサイズをセット
	barSprite->SetSize(size);
	barSprite->SetTexSize(texSize);

	//タイマーが指定した時間になったら
	if (changeLengthTimer >= changeTime)
	{
		//長さ変更終了
		isChangeLengthBar = false;
	}
}

void Energy::SetChangeLength()
{
	//バーの長さ変更タイマーを初期化
	changeLengthTimer = 0;
	//イージング用に変更前の長さをセット
	changeLengthBefore = barSprite->GetSize().x;
	//イージング用に変更後の長さをセット
	changeLengthAftar = lengthMax * ((float)energyPoint / energyPointMax);

	//バーの長さを変更状態にする
	isChangeLengthBar = true;
}
