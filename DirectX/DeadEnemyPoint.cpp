#include "DeadEnemyPoint.h"
#include "Easing.h"
#include "SafeDelete.h"

DeadEnemyPoint *DeadEnemyPoint::Create(Model *model, XMFLOAT3 position, float radius)
{
	//インスタンスを生成
	DeadEnemyPoint *instance = new DeadEnemyPoint();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position, radius)) {
		delete instance;
		assert(0);
	}

	return instance;
}

DeadEnemyPoint::~DeadEnemyPoint()
{
	safe_delete(deadPointObject);
}

bool DeadEnemyPoint::Initialize(Model *model, XMFLOAT3 position, float radius)
{
	//死んだ敵の位置オブジェクト生成
	deadPointObject = Object3d::Create();
	if (deadPointObject == nullptr) {
		return false;
	}

	//座標をセット
	deadPointObject->SetPosition(position);

	//基準のサイズをセット
	BaseRadius = radius;

	//変更前の円のサイズをセット
	changeRadiusStart = 0;
	//変更後の円のサイズをセット
	changeRadiusEnd = BaseRadius;
	deadPointObject->SetScale({ changeRadiusStart, changeRadiusStart, 1 });

	//円のサイズを変更状態にする
	isChangeRadius = true;

	if (model) {
		deadPointObject->SetModel(model);
	}

	//色を緑にする
	deadPointObject->SetColor({ 0.2f, 1, 0.2f, 1 });

	return true;
}

void DeadEnemyPoint::Update()
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
	deadPointObject->SetScale({ radius, radius, 1 });

	//サイズ変更タイマー更新
	changeRadiusTimer++;
	//タイマーが指定した時間になったら
	if (changeRadiusTimer >= changeTime)
	{
		//サイズ変更状態終了
		isChangeRadius = false;
	}

	//オブジェクト更新
	deadPointObject->Update();
}

void DeadEnemyPoint::Draw()
{
	//オブジェクト描画
	deadPointObject->Draw();
}

void DeadEnemyPoint::ChangeRadius()
{
	//自分から出ている線のカウントを更新
	countLine++;
	//変更前の円のサイズをセット
	changeRadiusStart = radius;
	//変更後の円のサイズをセット
	changeRadiusEnd = BaseRadius + ((float)countLine / 3);
	//サイズ変更タイマーを初期化
	changeRadiusTimer = 0;
	//サイズを変更中にする
	isChangeRadius = true;
}
