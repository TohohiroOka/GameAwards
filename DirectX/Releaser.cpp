#include "Releaser.h"


Releaser* Releaser::Create(Model* model, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//インスタンスを生成
	Releaser* instance = new Releaser();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, spawnPosition, 0)) {
		delete instance;
		assert(0);
	}

	//停止座標をセット
	instance->SetStayPos(stayPosition);

	return instance;
}

bool Releaser::Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
{
	//所属グループを設定
	group = EnemyGroup::Releaser;

	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(spawnPosition);

	//大きさをセット
	enemyObject->SetScale({ 7, 7, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を青くする
	enemyObject->SetColor({ 0, 0, 1, 1 });

	//攻撃力をセット
	power = 8;


	return true;
}

void Releaser::Update()
{
	//生きているとき
	if (isAlive)
	{
		if (isReleaseMode)
		{
			//放出モード
			ReleaseMode();
		}
	}

	BaseEnemy::Update();
}

void Releaser::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	//放出タイマーを初期値に戻す
	releaseTimer = 0;

	BaseEnemy::SetKnockBack(angle, powerLevel, shockWaveGroup);
}

void Releaser::Move()
{
	//放出モードの場合移動しないので、抜ける
	if (isReleaseMode) { return; }

	//移動速度に移動角度を乗算して座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();

	//移動量を座標に加算して移動させる
	pos.x += vel.x;
	pos.y += vel.y;

	//更新した座標をセット
	enemyObject->SetPosition(pos);

	//停止座標に到達したら
	if (pos.x >= stayPos.x - 1 && pos.x <= stayPos.x + 1 && pos.y >= stayPos.y - 1 && pos.y <= stayPos.y + 1)
	{
		//放出モードに切り替え
		isReleaseMode = true;
	}
}

void Releaser::SetStayPosAngle()
{
	XMFLOAT3 enemyPos = enemyObject->GetPosition();
	float angle = atan2f(stayPos.y - enemyPos.y, stayPos.x - enemyPos.x);

	//移動方向をセット
	moveAngle = angle;

	//オブジェクトの向きを進行方向にセット
	XMFLOAT3 rota = { 0, 0, DirectX::XMConvertToDegrees(angle) - 90 };
	enemyObject->SetRotation(rota);

	//移動量をセット
	vel.x = moveSpeed * cosf(moveAngle);
	vel.y = moveSpeed * sinf(moveAngle);
}

void Releaser::SetStayPos(XMFLOAT3 stayPosition)
{
	//停止座標をセットする
	stayPos = stayPosition;

	//移動方向を停止座標に向けセット
	SetStayPosAngle();
}

void Releaser::KnockBack()
{
	BaseEnemy::KnockBack();

	//ノックバックされても停止座標を向くようにする
	SetStayPosAngle();
}

void Releaser::ReleaseMode()
{
	//放出時間を設定
	const int releaseTime = 60;

	//放出タイマーを更新
	releaseTimer++;

	//毎フレーム放出するわけではないのでfalseに戻しておく
	isCreateEnemy = false;

	//タイマーが指定時間に到達したら
	if (releaseTimer >= releaseTime)
	{
		//敵放出
		Release();
	}
}

void Releaser::Release()
{
	//放出タイマーを初期化
	releaseTimer = 0;

	//敵生成
	isCreateEnemy = true;

	//敵のサイズを一回り小さくする
	XMFLOAT3 scale = enemyObject->GetScale();
	scale.x--;
	scale.y--;
	enemyObject->SetScale(scale);

	//敵の攻撃力を1下げる
	power--;

	//放出回数を更新
	releaseCount++;

	//放出を6回行ったら
	if (releaseCount >= 6)
	{
		//削除
		SetDelete();
	}
}
