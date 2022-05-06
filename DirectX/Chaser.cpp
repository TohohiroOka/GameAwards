#include "Chaser.h"
#include "Easing.h"

Model* Chaser::chaserModel[Chaser::modelNum] = { nullptr };

Chaser* Chaser::Create(XMFLOAT3 spawnPosition)
{
	//インスタンスを生成
	Chaser* instance = new Chaser();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(spawnPosition, 0)) {
		delete instance;
		assert(0);
	}

	return instance;
}

void Chaser::SetModel(Model* chaserModel1, Model* chaserModel2, Model* chaserModel3, Model* chaserModel4)
{
	//引数のモデルを共通で使うためセットする
	Chaser::chaserModel[0] = chaserModel1;
	Chaser::chaserModel[1] = chaserModel2;
	Chaser::chaserModel[2] = chaserModel3;
	Chaser::chaserModel[3] = chaserModel4;
}

bool Chaser::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//所属グループを設定
	group = EnemyGroup::Chaser;

	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(spawnPosition);
	//大きさをセット
	enemyObject->SetScale({ 8, 8, 1 });

	//モデルをセット
	if (chaserModel[0]) {
		enemyObject->SetModel(chaserModel[0]);
	}

	//攻撃力をセット
	power = 8;

	return true;
}

void Chaser::Update()
{
	if (isAlive)
	{
		if (!isKnockBack)
		{
			//ターゲットに向けて追従
			SetAngleForTarget(targetPos);
		}
	}

	BaseEnemy::Update();
}

void Chaser::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	BaseEnemy::SetKnockBack(angle, powerLevel, shockWaveGroup);

	//敵のモデルを変更
	if (enemyObject->GetModel() != chaserModel[3])
	{
		if (knockBackPowerLevel == 1) { enemyObject->SetModel(chaserModel[1]); }
		else if (knockBackPowerLevel == 2) { enemyObject->SetModel(chaserModel[2]); }
		else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(chaserModel[3]); }
	}
}

void Chaser::Move()
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

void Chaser::SetAngleForTarget(XMFLOAT3 targetPosition)
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

void Chaser::ChangeMoveSpeed()
{
	//1ループを行う時間
	const int roopTime = 60;

	//移動速度変更タイマー更新
	moveSpeedTimer++;

	//移動を行う時間
	const int moveSpeedTime = 55;
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
