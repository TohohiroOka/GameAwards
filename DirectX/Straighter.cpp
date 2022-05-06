#include "Straighter.h"

Model* Straighter::straighterModel[Straighter::modelNum] = { nullptr };


Straighter* Straighter::Create(XMFLOAT3 spawnPosition, float moveDegree, int knockBackPowerLevel)
{
	//インスタンスを生成
	Straighter* instance = new Straighter();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(spawnPosition, moveDegree)) {
		delete instance;
		assert(0);
	}

	//親がいた場合、親のノックバックの強さを引き継ぐ
	instance->SetParentKnockBackPowerLevel(knockBackPowerLevel);

	return instance;
}

void Straighter::SetModel(Model* straighterModel1, Model* straighterModel2, Model* straighterModel3, Model* straighterModel4)
{
	//引数のモデルを共通で使うためセットする
	Straighter::straighterModel[0] = straighterModel1;
	Straighter::straighterModel[1] = straighterModel2;
	Straighter::straighterModel[2] = straighterModel3;
	Straighter::straighterModel[3] = straighterModel4;
}

bool Straighter::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//所属グループを設定
	group = EnemyGroup::Straighter;

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
	enemyObject->SetScale({ 5, 5, 1 });

	//モデルをセット
	if (straighterModel[0]) {
		enemyObject->SetModel(straighterModel[0]);
	}

	//攻撃力をセット
	power = 2;

	//移動角度をセット
	SetMoveAngle(moveDegree);

	return true;
}

void Straighter::SetKnockBack(float angle, int powerLevel, int shockWaveGroup)
{
	BaseEnemy::SetKnockBack(angle, powerLevel, shockWaveGroup);

	//敵のモデルを変更
	if (enemyObject->GetModel() != straighterModel[3])
	{
		if (knockBackPowerLevel == 1) { enemyObject->SetModel(straighterModel[1]); }
		else if (knockBackPowerLevel == 2) { enemyObject->SetModel(straighterModel[2]); }
		else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(straighterModel[3]); }
	}
}

void Straighter::Move()
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

bool Straighter::CheckInScreen()
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

void Straighter::ReflectionX()
{
	//左右反射用に反射角をセットする
	float reflectionAngle = 360 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//速度を変更する
	moveSpeed = 1.5f;
}

void Straighter::ReflectionY()
{
	//上下反射用に反射角をセットする
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//速度を変更する
	moveSpeed = 1.5f;
}

void Straighter::SetParentKnockBackPowerLevel(int knockBackPowerLevel)
{
	//親の吹っ飛び威力を引き継ぐ
	this->knockBackPowerLevel = knockBackPowerLevel;

	//吹っ飛び威力に合わせて敵のモデルを変更
	if (knockBackPowerLevel == 0) { enemyObject->SetModel(straighterModel[0]); }
	else if (knockBackPowerLevel == 1) { enemyObject->SetModel(straighterModel[1]); }
	else if (knockBackPowerLevel == 2) { enemyObject->SetModel(straighterModel[2]); }
	else if (knockBackPowerLevel >= 3) { enemyObject->SetModel(straighterModel[3]); }
}
