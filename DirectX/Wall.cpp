#include "Wall.h"
#include "SafeDelete.h"
#include "Easing.h"

Wall* Wall::Create(Model* model)
{
	//インスタンスを生成
	Wall* instance = new Wall();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Wall::~Wall()
{
	safe_delete(wallObject);
}

bool Wall::Initialize(Model* model)
{
	//壁オブジェクト生成
	wallObject = Object3d::Create();
	if (wallObject == nullptr) {
		return false;
	}

	//初期地点と大きさと色をセット
	wallObject->SetPosition({ 0, 0, 0 });
	wallObject->SetScale({ 11.8f, 12.4f, 1 });
	wallObject->SetColor({ 0.8f, 0.3f, 0.1f, 1.0f });

	//モデルをセット
	if (model) {
		wallObject->SetModel(model);
	}

	//最初のHPを設定
	maxHP = baseMaxHP;
	HP = maxHP;

	return true;
}

void Wall::Update()
{
	//リザルトシーン用に動かす
	if (isChangeResult) 
	{
		ChangeResult();
	}

	//休憩中
	if (isBreakTime) 
	{
		BreakTime();
	}
	//壁生成中
	else if (isCreate)
	{
		CreateWall();
	}

	//オブジェクト更新
	wallObject->Update();
}

void Wall::Draw()
{
	if (isBreakTime) { return; }

	//オブジェクト描画
	wallObject->Draw();
}

void Wall::Reset()
{
	//壁の最大HP
	maxHP = 20;
	//壁のHP
	HP = maxHP;
	//休憩時間タイマー
	breakTimer = 0;
	//休憩中か
	isBreakTime = true;
	//壁生成タイマー
	createTimer = 0;
	//壁生成回数
	createCount = 0;
	//壁生成中か
	isCreate = false;
	//リザルトシーン用に動かしす時間タイマー
	changeResultTimer = 0;
	//壁をリザルトシーン用に動かしているか
	isChangeResult = false;
	//壊されたか
	isBreak = false;
	//生きているか
	isAlive = false;
}

void Wall::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	HP -= damagePower;

	//残りHPに応じて色変更
	ChangeColor();

	//HPが0以下になったら破壊
	if (HP <= 0)
	{
		Break();

		//生きていない
		isAlive = false;
	}
}

void Wall::Break()
{
	//破壊する
	isBreak = true;

	//休憩状態にする
	SetBreakTime();
}

void Wall::SetBreakTime()
{
	//休憩時間タイマーを初期化
	breakTimer = 0;

	//休憩状態にする
	isBreakTime = true;
}

void Wall::SetCreateWall()
{
	//最大HPを設定
	createCount++;
	maxHP = baseMaxHP + (10 * createCount);
	HP = maxHP;

	//壁生成初期サイズをセット
	createStartScale = { 12.1f, 12.7f, 1 };
	//壁生成最終サイズをセット
	createEndScale = { 11.8f, 12.4f, 1 };

	//生成中の色にセット
	wallObject->SetColor({ 0.3f, 0.3f, 0.3f, 1.0f });

	//壁生成タイマーを初期化
	createTimer = 0;

	//壁生成状態にする
	isCreate = true;
}

bool Wall::GetTriggerBreak()
{
	//破壊されていたら
	if (isBreak) 
	{
		//トリガーなのでfalseに戻す
		isBreak = false;

		return true;
	}

	return false;
}

void Wall::SetChangeResult()
{
	//リザルトシーン用に動かすタイマーを初期化
	changeResultTimer = 0;

	//リザルトシーン用に動かす状態にセット
	isChangeResult = true;
}

void Wall::BreakTime()
{
	//休憩時間
	const int breakTime = 80;

	//タイマーを更新
	breakTimer++;

	//タイマーが指定した時間になったら
	if (breakTimer >= breakTime)
	{
		//壁生成状態にする
		SetCreateWall();

		//休憩終了
		isBreakTime = false;
	}
}

void Wall::CreateWall()
{
	//壁生成時間
	const int createTime = 100;

	//壁生成タイマーを更新
	createTimer++;

	//イージング計算用の時間
	float easeTimer = (float)createTimer / createTime;

	//イージングで壁オブジェクトの大きさ変更
	XMFLOAT3 scale = wallObject->GetScale();
	scale.x = Easing::OutQuint(createStartScale.x, createEndScale.x, easeTimer);
	scale.y = Easing::OutQuint(createStartScale.y, createEndScale.y, easeTimer);
	//壁オブジェクトの大きさを更新
	wallObject->SetScale(scale);

	//タイマーが指定した時間になったら
	if (createTimer >= createTime)
	{
		//HP最大時の色にセット
		wallObject->SetColor({ 0.8f, 0.3f, 0.1f, 1.0f });

		//生きている
		isAlive = true;

		//壁生成終了
		isCreate = false;
	}
}

void Wall::ChangeColor()
{
	//残りHPが最大HPの20%以下の場合
	if (HP <= maxHP / 5) { wallObject->SetColor({ 0.3f, 0.1f, 0.1f, 1.0f }); }
	//残りHPが最大HPの50%以下の場合
	else if (HP <= maxHP / 2) { wallObject->SetColor({ 0.5f, 0.2f, 0.2f, 1.0f }); }
}

void Wall::ChangeResult()
{
	//壁を動かす時間
	const int changeTime = 100;

	//タイマーを更新
	changeResultTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeResultTimer / changeTime;

	//イージングで壁オブジェクトの大きさ変更
	XMFLOAT3 scale = wallObject->GetScale();
	scale.x = Easing::OutQuint(createEndScale.x, 12.8f, easeTimer);
	scale.y = Easing::OutQuint(createEndScale.y, 13.4f, easeTimer);
	//壁オブジェクトの大きさを更新
	wallObject->SetScale(scale);

	//タイマーが指定した時間になったら
	if (changeResultTimer >= changeTime)
	{
		//壁を動かす終了
		isChangeResult = false;
	}
}
