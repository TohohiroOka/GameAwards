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

void ConnectCircle::BigRadius()
{
	//自分から出ている線のカウントを増やす
	countLine++;

	//サイズ変更
	SetChangeRadius();
}

void ConnectCircle::SmallRadius()
{
	//自分から出ている線のカウントを減らす
	countLine--;

	//サイズ変更
	SetChangeRadius();
}

bool ConnectCircle::CheckUseGaruEnemy(GaruEnemy* garuEnemy)
{
	//始点か終点で引数の円を使用していたらtrueを返す
	if (this->garuEnemy == garuEnemy)
	{
		return true;
	}

	//使用していなければfalseを返す
	return false;
}

bool ConnectCircle::CheckUseFixedEnemy(FixedEnemy* fixedEnemy)
{
	//始点か終点で引数の固定敵を使用していたらtrueを返す
	if (this->fixedEnemy == fixedEnemy)
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
	//イージング計算用の時間
	float easeTimer = (float)changeRadiusTimer / changeTime;

	//イージングでサイズ変更
	float newRadius = Easing::OutCubic(changeRadiusStart, changeRadiusEnd, easeTimer);

	//サイズを更新
	radius = newRadius;
	//更新したサイズをセット
	circleObject->SetScale({ radius, radius, 1 });

	//サイズ変更タイマー更新
	changeRadiusTimer++;
	//タイマーが指定した時間になったら
	if (changeRadiusTimer >= changeTime)
	{
		//サイズ変更状態終了
		isChangeRadius = false;
	}
}
