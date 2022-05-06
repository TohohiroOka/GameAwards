#include "Division.h"

Model* Division::divisionModel[Division::modelNum] = { nullptr };


Division* Division::Create(XMFLOAT3 spawnPosition, float moveDegree)
{
	//インスタンスを生成
	Division* instance = new Division();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(spawnPosition, moveDegree)) {
		delete instance;
		assert(0);
	}

	return instance;
}

void Division::SetModel(Model* divisionModel1, Model* divisionModel2, Model* divisionModel3, Model* divisionModel4)
{
	//引数のモデルを共通で使うためセットする
	Division::divisionModel[0] = divisionModel1;
	Division::divisionModel[1] = divisionModel2;
	Division::divisionModel[2] = divisionModel3;
	Division::divisionModel[3] = divisionModel4;
}

bool Division::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//所属グループを設定
	group = EnemyGroup::Division;

	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(spawnPosition);
	//壁内にいるかチェック
	isInScreen = CheckInScreen();

	//大きさをセット
	enemyObject->SetScale({ 8, 8, 1 });

	//モデルをセット
	if (divisionModel[0]) {
		enemyObject->SetModel(divisionModel[0]);
	}

	//攻撃力をセット
	power = 7;

	//移動角度をセット
	SetMoveAngle(moveDegree);

	return true;
}

void Division::Update()
{
	//生きているとき
	if (isAlive)
	{
		//生存時間更新
		AliveTimeUpdate();
	}

	BaseEnemy::Update();
}

void Division::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	//生存した時間を初期値に戻す
	aliveTimer = 0;

	BaseEnemy::SetKnockBack(angle, powerLevel, shockWaveGroup);

	//敵のモデルを変更
	if (enemyObject->GetModel() != divisionModel[3])
	{
		if (knockBackPowerLevel == 1) { enemyObject->SetModel(divisionModel[1]); }
		else if (knockBackPowerLevel == 2) { enemyObject->SetModel(divisionModel[2]); }
		else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(divisionModel[3]); }
	}
}

void Division::Move()
{
	//移動速度に移動角度を乗算して座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();

	//壁内にいる場合
	if (isInScreen)
	{
		//枠に当たっていたら反射させる
		if (pos.x <= -wallLine.x + size.x / 2) { ReflectionX(); }
		else if (pos.x >= wallLine.x - size.x / 2) { ReflectionX(); }
		if (pos.y <= -wallLine.y + size.y / 2) { ReflectionY(); }
		else if (pos.y >= wallLine.y - size.y / 2) { ReflectionY(); }
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

bool Division::CheckInScreen()
{
	//壁内にいるか
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x > -wallLine.x + size.x / 2 && pos.x < wallLine.x - size.x / 2 &&
		pos.y > -wallLine.y + size.y / 2 && pos.y < wallLine.y - size.y / 2)
	{
		return true;
	}

	return false;
}

void Division::ReflectionX()
{
	//左右反射用に反射角をセットする
	float reflectionAngle = 360 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//速度を変更する
	moveSpeed = 1.5f;
}

void Division::ReflectionY()
{
	//上下反射用に反射角をセットする
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//速度を変更する
	moveSpeed = 1.5f;
}

void Division::AliveTimeUpdate()
{
	//生存可能時間
	const int aliveTime = 300;

	//タイマー更新
	aliveTimer++;

	//タイマーが指定時間に到達したら
	if (aliveTimer >= aliveTime)
	{
		//削除する
		SetDelete();

		//新たに敵を生成する
		isCreateEnemy = true;
	}
}
