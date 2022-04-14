#include "Charo.h"
#include "SafeDelete.h"
#include "Easing.h"

Charo* Charo::Create(Model* model, XMFLOAT3 spawnPosition)
{
	//インスタンスを生成
	Charo* instance = new Charo();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, spawnPosition)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Charo::~Charo()
{
	safe_delete(enemyObject);
}

bool Charo::Initialize(Model* model, XMFLOAT3 spawnPosition)
{
	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(spawnPosition);
	//大きさをセット
	enemyObject->SetScale({ 4, 4, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を赤くする
	enemyObject->SetColor({ 1, 1, 1, 1 });

	return true;
}

void Charo::Update(XMFLOAT3 targetPosition)
{
	//進行方向をセット
	SetMoveAngle(targetPosition);
	//移動処理
	Move();

	//敵が死亡していたら削除状態にする
	if (!isAlive)
	{
		SetDelete();
	}

	//敵オブジェクト更新
	enemyObject->Update();
}

void Charo::Draw()
{
	//敵オブジェクト描画
	enemyObject->Draw();
}

void Charo::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	HP -= damagePower;
}

void Charo::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void Charo::SetDelete()
{
	//削除する
	isDelete = true;
}

void Charo::SetMoveAngle(XMFLOAT3 targetPosition)
{
	//移動角度を設定する（標的に向かって一直線）
	XMFLOAT3 position = enemyObject->GetPosition();
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	moveAngle = radian;

	//オブジェクトの向きを進行方向にセット ラジアンを角度に直し上向きを0に調整する
	float degree = DirectX::XMConvertToDegrees(radian);
	XMFLOAT3 rota = { 0, 0, degree - 90 };
	enemyObject->SetRotation(rota);
}

void Charo::ChangeMoveSpeed()
{
	//1ループを行う時間
	const int roopTime = 50;

	//移動速度変更タイマー更新
	moveSpeedTimer++;

	//移動を行う時間
	const int moveSpeedTime = 40;
	if (moveSpeedTimer <= moveSpeedTime)
	{
		//イージング計算用の時間
		float easeTimer = (float)moveSpeedTimer / moveSpeedTime;
		//移動速度変更基準の速度
		moveSpeed = Easing::OutQuint(3.0f, 0, easeTimer);
	}
	//停止を行う
	else
	{
		//タイマーが指定した時間になったら
		if (moveSpeedTimer >= roopTime)
		{
			//タイマーをを元に戻す
			moveSpeedTimer = 0;
		}
	}
}

void Charo::Move()
{
	//移動速度変更
	ChangeMoveSpeed();

	//移動速度に移動角度を乗算して座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += moveSpeed * cosf(moveAngle);
	pos.y += moveSpeed * sinf(moveAngle);

	//更新した座標をセット
	enemyObject->SetPosition(pos);
}
