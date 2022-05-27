#include "Division.h"

Model* Division::divisionModel = nullptr;


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

void Division::SetModel(Model* divisionModel)
{
	//引数のモデルを共通で使うためセットする
	Division::divisionModel = divisionModel;
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
	if (divisionModel) {
		enemyObject->SetModel(divisionModel);
	}

	enemyObject->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

	//基準のダメージ量をセット
	baseDamagePower = 5;

	//移動角度をセット
	SetMoveAngle(moveDegree);

	return true;
}

void Division::Update()
{
	//生きているとき
	if (isAlive && !isResultMove)
	{
		//生存時間更新
		AliveTimeUpdate();
	}

	BaseEnemy::Update();
}

void Division::SetKnockBack(float angle, int powerLevel, float powerMagnification)
{
	//生存した時間を初期値に戻す
	aliveTimer = 0;

	BaseEnemy::SetKnockBack(angle, powerLevel, powerMagnification);
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
		if (pos.x <= wallLineMin.x + size.x / 2)
		{
			ReflectionX();
			//壁にめり込まないように押し戻す
			pos.x = wallLineMin.x + size.x / 2;
		}
		else if (pos.x >= wallLineMax.x - size.x / 2)
		{
			ReflectionX();
			//壁にめり込まないように押し戻す
			pos.x = wallLineMax.x - size.x / 2;
		}
		if (pos.y <= wallLineMin.y + size.y / 2)
		{
			ReflectionY();
			//壁にめり込まないように押し戻す
			pos.y = wallLineMin.y + size.y / 2;
		}
		else if (pos.y >= wallLineMax.y - size.y / 2)
		{
			ReflectionY();
			//壁にめり込まないように押し戻す
			pos.y = wallLineMax.y - size.y / 2;
		}
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

void Division::ResultMove()
{
	//移動速度に移動角度を乗算して座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();

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
	if (pos.x > wallLineMin.x + size.x / 2 && pos.x < wallLineMax.x - size.x / 2 &&
		pos.y > wallLineMin.y + size.y / 2 && pos.y < wallLineMax.y - size.y / 2)
	{
		return true;
	}

	return false;
}

void Division::AliveTimeUpdate()
{
	//生存可能時間
	const int aliveTime = 180;

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
