#include "Zakorin.h"
#include "Easing.h"

Zakorin *Zakorin::Create(Model *model, XMFLOAT3 position)
{
	//インスタンスを生成
	Zakorin *instance = new Zakorin();
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

bool Zakorin::Initialize(Model *model, XMFLOAT3 position)
{
	//ザコリンオブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(position);
	//大きさをセット
	enemyObject->SetScale({ 4, 4, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	return true;
}

void Zakorin::Update()
{
	////存在すらなくなったら更新しない
	//if (!isExistence) return;

	//通常時の移動
	if (isAlive)
	{
		XMFLOAT3 pos = enemyObject->GetPosition();
		pos.x += 0.01f;
		pos.y -= 0.01f;
		enemyObject->SetPosition(pos);
	}
	//ノックバックでの移動
	else
	{
		KnockBack();
	}

	//オブジェクト更新
	enemyObject->Update();
}

void Zakorin::KnockBack()
{
	//ノックバックを行う時間
	const int knockBackTime = 20;
	//イージング計算用の時間
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//ノックバック基準の速度
	const float knockBackStartSpeed = 1.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);
	int power = killBulletPower / 10;

	//座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x -= knockBackSpeed * sinf(knockBackAngle) * power;
	pos.y += knockBackSpeed * cosf(knockBackAngle) * power;
	//更新した座標をセット
	enemyObject->SetPosition(pos);

	//ノックバックタイマー更新
	knockBackTimer++;
	//タイマーが指定した時間になったら
	if (knockBackTimer > knockBackTime)
	{
		//色を薄くする
		enemyObject->SetColor({ 1, 1, 1, 0.8f });

		//存在すら終了
		isExistence = false;
	}
}
