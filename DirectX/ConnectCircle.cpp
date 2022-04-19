#include "ConnectCircle.h"
#include "Easing.h"
#include "SafeDelete.h"

ConnectCircle::~ConnectCircle()
{
	safe_delete(circleObject);
}

void ConnectCircle::Update()
{
	//サイズ変更状態なら
	if (isChangeRadius)
	{
		//大きさを変更
		ChangeRadius();
	}

	//オブジェクト更新
	circleObject->Update();
}

void ConnectCircle::Draw()
{
	//オブジェクト描画
	circleObject->Draw();
}

bool ConnectCircle::CheckUsePin(Pin* pin)
{
	//始点か終点で引数のピンを使用していたらtrueを返す
	if (this->pin == pin)
	{
		return true;
	}

	//使用していなければfalseを返す
	return false;
}

void ConnectCircle::SetDelete()
{
	//削除する
	isDelete = true;
}

void ConnectCircle::SetChangeRadius()
{
	//変更前の円のサイズをセット
	changeRadiusStart = radius;
	//変更後の円のサイズをセット
	changeRadiusEnd = baseRadius + ((float)countLine / 3);
	//サイズ変更タイマーを初期化
	changeRadiusTimer = 0;
	//サイズを変更中にする
	isChangeRadius = true;
}

void ConnectCircle::ChangeRadius()
{
	//サイズ変更を行う時間
	const int changeTime = 20;

	//サイズ変更タイマー更新
	changeRadiusTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeRadiusTimer / changeTime;

	//イージングでサイズ変更
	float newRadius = Easing::OutCubic(changeRadiusStart, changeRadiusEnd, easeTimer);

	//サイズを更新
	radius = newRadius;
	//更新したサイズをセット
	circleObject->SetScale({ radius, radius, 1 });

	//タイマーが指定した時間になったら
	if (changeRadiusTimer >= changeTime)
	{
		//サイズ変更状態終了
		isChangeRadius = false;
	}
}
