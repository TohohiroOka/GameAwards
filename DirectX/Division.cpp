#include "Division.h"


Division* Division::Create(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
{
	//インスタンスを生成
	Division* instance = new Division();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, spawnPosition, moveDegree)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Division::Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
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
	enemyObject->SetScale({ 4, 4, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を青くする
	enemyObject->SetColor({ 0, 0, 1, 1 });

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

void Division::SetKnockBack(float angle, int powerLevel)
{
	//生存した時間を初期値に戻す
	aliveTimer = 0;

	BaseEnemy::SetKnockBack(angle, powerLevel);
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
	pos.x += vel.x;
	pos.y += vel.y;

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
}

void Division::ReflectionY()
{
	//上下反射用に反射角をセットする
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);
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
