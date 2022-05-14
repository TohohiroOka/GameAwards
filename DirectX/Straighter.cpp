#include "Straighter.h"

Model* Straighter::straighterModel = nullptr;


Straighter* Straighter::Create(XMFLOAT3 spawnPosition, float moveDegree)
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

	return instance;
}

void Straighter::SetModel(Model* straighterModel)
{
	//引数のモデルを共通で使うためセットする
	Straighter::straighterModel = straighterModel;
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
	if (straighterModel) {
		enemyObject->SetModel(straighterModel);
	}

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

void Straighter::ResultMove()
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

bool Straighter::CheckInScreen()
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
