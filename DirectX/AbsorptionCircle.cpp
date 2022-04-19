#include "AbsorptionCircle.h"
#include "SafeDelete.h"
#include "Easing.h"

AbsorptionCircle* AbsorptionCircle::Create(Model* model, XMFLOAT3 position)
{
	//インスタンスを生成
	AbsorptionCircle* instance = new AbsorptionCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

AbsorptionCircle::~AbsorptionCircle()
{
	safe_delete(circleObject);
}

bool AbsorptionCircle::Initialize(Model* model, XMFLOAT3 position)
{
	//円オブジェクト生成
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//座標をセット
	circleObject->SetPosition(position);

	if (model) {
		circleObject->SetModel(model);
	}


	//色を青にする
	circleObject->SetColor({ 0.2f, 0.2f, 0.2f, 0.4f });

	return true;
}

void AbsorptionCircle::Update()
{
	//円を広げる
	if (action == ActionNumber::Spread)
	{
		SpreadCircle();
	}
	//一時停止
	else if (action == ActionNumber::Stay)
	{
		StayCircle();
	}
	//円を縮める
	else if (action == ActionNumber::Shrink)
	{
		ShrinkCircle();
	}

	//オブジェクト更新
	circleObject->Update();
}

void AbsorptionCircle::Draw()
{
	//オブジェクト描画
	circleObject->Draw();
}

void AbsorptionCircle::SetDelete()
{
	//削除する
	isDelete = true;
}

void AbsorptionCircle::SpreadCircle()
{
	//拡大を行う時間
	const int spreadTime = 30;

	//円のサイズを広げるタイマー更新
	spreadTimer++;

	//イージング計算用の時間
	float easeTimer = (float)spreadTimer / spreadTime;

	//イージングでサイズ変更
	float newRadius = Easing::OutCubic(0.0f, 20.0f, easeTimer);

	//更新したサイズをセット
	circleObject->SetScale({ newRadius, newRadius, 1 });

	//タイマーが指定した時間になったら
	if (spreadTimer >= spreadTime)
	{
		//円を一時停止させる
		action = ActionNumber::Stay;
	}
}

void AbsorptionCircle::StayCircle()
{
	//一時停止を行う時間
	const int stayTime = 120;

	//一時停止タイマー更新
	stayTimer++;

	//タイマーが指定した時間になったら
	if (stayTimer >= stayTime)
	{
		//円を縮める
		action = ActionNumber::Shrink;
	}
}

void AbsorptionCircle::ShrinkCircle()
{
	//縮小を行う時間
	const int shrinkTime = 30;

	//円のサイズを縮めるタイマー更新
	shrinkTimer++;

	//イージング計算用の時間
	float easeTimer = (float)shrinkTimer / shrinkTime;

	//イージングでサイズ変更
	float newRadius = Easing::OutCubic(20.0f, 0.0f, easeTimer);

	//更新したサイズをセット
	circleObject->SetScale({ newRadius, newRadius, 1 });

	//タイマーが指定した時間になったら
	if (shrinkTimer >= shrinkTime)
	{
		//削除する
		SetDelete();
	}
}
