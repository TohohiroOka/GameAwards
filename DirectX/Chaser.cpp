#include "Chaser.h"
#include "Easing.h"

Model* Chaser::chaserModel = nullptr;

Chaser* Chaser::Create(XMFLOAT3 spawnPosition, float moveDegree, bool isParent)
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

	if (isParent) {
		instance->SetSpawnParent(moveDegree);
	}

	return instance;
}

void Chaser::SetModel(Model* chaserModel)
{
	//引数のモデルを共通で使うためセットする
	Chaser::chaserModel = chaserModel;
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
	enemyObject->SetScale({ 5, 5, 1 });

	//モデルをセット
	if (chaserModel) {
		enemyObject->SetModel(chaserModel);
	}

	return true;
}

void Chaser::SetSpawnParent(float moveDegree)
{
	//親から生まれた動きをする
	isParentMove = true;

	//移動角度をセット
	BaseEnemy::SetMoveAngle(moveDegree);
}

void Chaser::Move()
{
	//親から生まれた動き(直進)
	if (isParentMove)
	{
		MoveStraight();

		//直進する時間を計測
		CountMoveStraightTime();
	}
	//ターゲットに追従
	else
	{
		MoveChase();
	}
}

void Chaser::ResultMove()
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
}

bool Chaser::CheckInScreen()
{
	//壁内にいるか
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x > wallLineMin.x + size.x / 2 && pos.x < wallLineMax.x - size.x / 2 &&
		pos.y > wallLineMin.y + size.y / 2 && pos.y < wallLineMax.y - size.y / 2)
	{
		return true;
	}

	return false;
}

void Chaser::MoveStraight()
{
	//移動速度に移動角度を乗算して座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();

	//壁内にいる場合
	if (isInScreen)
	{
		//枠に当たっていたら反射させる
		if (pos.x <= wallLineMin.x + size.x / 2) { ReflectionX(); }
		else if (pos.x >= wallLineMax.x - size.x / 2) { ReflectionX(); }
		if (pos.y <= wallLineMin.y + size.y / 2) { ReflectionY(); }
		else if (pos.y >= wallLineMax.y - size.y / 2) { ReflectionY(); }
	}
	//壁内にいない場合
	else
	{
		//壁内に入ったかチェックする
		isInScreen = CheckInScreen();
	}

	//移動量を座標に加算して移動させる
	pos.x += vel.x * moveSpeed;
	pos.y += vel.y * moveSpeed;

	//だんだん遅くする
	moveSpeed -= 0.005f;
	const float moveSpeedMin = 0.7f;
	if (moveSpeed <= moveSpeedMin)
	{
		moveSpeed = moveSpeedMin;
	}

	//更新した座標をセット
	enemyObject->SetPosition(pos);
}

void Chaser::CountMoveStraightTime()
{
	//直進する時間
	const int moveStraightTime = 30;

	//直進タイマーを更新
	moveStraightTimer++;

	//タイマーが指定した時間に到達したら追従を始める
	if (moveStraightTimer >= moveStraightTime)
	{
		isParentMove = false;
	}
}

void Chaser::MoveChase()
{
	//ターゲットに向けて追従
	SetAngleForTarget(targetPos);

	//移動速度変更
	ChangeMoveSpeed();

	//移動速度に移動角度を乗算して座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();

	//ターゲット座標付近に居なければ追従で動かす
	const float chaseDistance = 1.0f;
	if (!(targetPos.x - chaseDistance < pos.x && targetPos.x + chaseDistance > pos.x &&
		targetPos.y - chaseDistance < pos.y && targetPos.y + chaseDistance > pos.y))
	{
		pos.x += moveSpeed * cosf(moveAngle);
		pos.y += moveSpeed * sinf(moveAngle);

		//更新した座標をセット
		enemyObject->SetPosition(pos);
	}

	//オブジェクトの向きを進行方向にセット ラジアンを角度に直し上向きを0に調整する
	float degree = DirectX::XMConvertToDegrees(moveAngle);
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
