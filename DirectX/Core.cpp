#include "Core.h"
#include "SafeDelete.h"
#include "Easing.h"

Core* Core::Create(Model* model)
{
	//インスタンスを生成
	Core* instance = new Core();
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

Core::~Core()
{
	safe_delete(coreObject);
}

bool Core::Initialize(Model* model)
{
	//コアオブジェクト生成
	coreObject = Object3d::Create();
	if (coreObject == nullptr) {
		return false;
	}

	//初期地点と大きさをセット
	coreObject->SetPosition({ 0, -1000, 0 });
	coreObject->SetScale({ 5, 5, 1 });

	//コアのモデルをセット
	if (coreObject) {
		coreObject->SetModel(model);
	}

	return true;
}

void Core::Update()
{
	//スポーン中の処理
	if (isDuringSpawn)
	{
		//コアをスポーン
		Spawn();
	}

	//オブジェクト描更新
	coreObject->Update();
}

void Core::Draw()
{
	//オブジェクト描画
	coreObject->Draw();
}

void Core::Damage(int damagePower)
{
	//強化されていない攻撃は無効化する
	if (damagePower <= 10) { return; }

	//無効化できなかった場合死亡させる
	Dead();

	coreObject->SetColor({ 1, 0, 0, 1 });
}

void Core::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void Core::SetSpawn(XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//スポーン時の座標と移動後の座標をセット
	this->spawnPosition = spawnPosition;
	this->stayPosition = stayPosition;

	//スポーン状態にする
	isDuringSpawn = true;
}

void Core::Spawn()
{
	//スポーンを行う時間
	const int spawnTime = 150;

	//スポーンタイマー更新
	spawnTimer++;

	//イージング計算用の時間
	float easeTimer = (float)spawnTimer / spawnTime;
	//スポーン時の画面外からの座標移動
	XMFLOAT3 pos = {};
	pos.x = Easing::OutCubic(spawnPosition.x, stayPosition.x, easeTimer);
	pos.y = Easing::OutCubic(spawnPosition.y, stayPosition.y, easeTimer);
	//更新した座標をセット
	coreObject->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (spawnTimer >= spawnTime)
	{
		//スポーン終了
		isDuringSpawn = false;
	}
}
