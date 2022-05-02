#include "Straighter.h"


Straighter* Straighter::Create(Model* model, XMFLOAT3 spawnPosition, float moveDegree, int knockBackPowerLevel)
{
	//インスタンスを生成
	Straighter* instance = new Straighter();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, spawnPosition, moveDegree)) {
		delete instance;
		assert(0);
	}

	//親がいた場合、親のノックバックの強さを引き継ぐ
	instance->SetParentKnockBackPowerLevel(knockBackPowerLevel);

	return instance;
}

bool Straighter::Initialize(Model* model, XMFLOAT3 spawnPosition, float moveDegree)
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
	enemyObject->SetScale({ 2, 2, 1 });

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を青くする
	enemyObject->SetColor({ 0, 0, 1, 1 });

	//攻撃力をセット
	power = 2;

	//移動角度をセット
	SetMoveAngle(moveDegree);

	return true;
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
	pos.x += vel.x;
	pos.y += vel.y;

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
}

void Straighter::ReflectionY()
{
	//上下反射用に反射角をセットする
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);
}

void Straighter::SetParentKnockBackPowerLevel(int knockBackPowerLevel)
{
	this->knockBackPowerLevel = knockBackPowerLevel;

	//親の色に合わせて敵の色を変更
	if (knockBackPowerLevel == 0) { enemyObject->SetColor({ 0, 0, 1, 1 }); }
	else if (knockBackPowerLevel == 1) { enemyObject->SetColor({ 0, 1, 0, 1 }); }
	else if (knockBackPowerLevel == 2) { enemyObject->SetColor({ 1, 1, 0, 1 }); }
	else if (knockBackPowerLevel >= 3) { enemyObject->SetColor({ 1, 0, 0, 1 }); }
}
