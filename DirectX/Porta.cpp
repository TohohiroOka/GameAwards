#include "Porta.h"
#include "SafeDelete.h"

DirectX::XMFLOAT2 Porta::reflectionLine = { 97, 53 };

Porta *Porta::Create(Model *model, XMFLOAT3 spawnPosition, float moveDegree)
{
	//インスタンスを生成
	Porta *instance = new Porta();
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

Porta::~Porta()
{
	safe_delete(enemyObject);
}

bool Porta::Initialize(Model *model, XMFLOAT3 spawnPosition, float moveDegree)
{
	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(spawnPosition);
	//大きさをセット
	enemyObject->SetScale({ 2, 2, 1 });

	//移動角度をセット
	SetMoveAngle(moveDegree);

	//モデルをセット
	if (model) {
		enemyObject->SetModel(model);
	}

	//色を赤くする
	enemyObject->SetColor({ 1, 1, 1, 1 });

	return true;
}

void Porta::Update()
{
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

void Porta::Draw()
{
	//敵オブジェクト描画
	enemyObject->Draw();
}

void Porta::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	HP -= damagePower;
}

void Porta::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void Porta::SetDelete()
{
	//削除する
	isDelete = true;
}

void Porta::Move()
{
	//移動速度に移動角度を乗算して座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();

	//枠内にいる場合
	if (isInScreen)
	{
		//枠に当たっていたら反射させる
		/*if (pos.x <= -reflectionLine.x - size.x / 2) { vel = { }; }
		else if (pos.x >= reflectionLine.x + size.x / 2) { vel = { }; }
		if (pos.y <= -reflectionLine.y - size.y / 2) { vel = { }; }
		else if (pos.y >= reflectionLine.y + size.y / 2) { vel = { }; }*/
		if (pos.x <= -reflectionLine.x - size.x / 2) { ReflectionX(); }
		else if (pos.x >= reflectionLine.x + size.x / 2) { ReflectionX(); }
		if (pos.y <= -reflectionLine.y - size.y / 2) { ReflectionY(); }
		else if (pos.y >= reflectionLine.y + size.y / 2) { ReflectionY(); }
	}
	//枠内にいない場合
	else
	{
		//枠内に入ったかチェックする
		if (pos.x > -reflectionLine.x - size.x / 2 && pos.x < reflectionLine.x + size.x / 2 &&
			pos.y > -reflectionLine.y - size.y / 2 && pos.y < reflectionLine.y + size.y / 2)
		{
			isInScreen = true;
		}
	}

	//移動量を座標に加算して移動させる
	pos.x += vel.x;
	pos.y += vel.y;

	//更新した座標をセット
	enemyObject->SetPosition(pos);
}

void Porta::SetMoveAngle(float moveDegree)
{
	//上向きを0にするため90度傾ける
	this->moveDegree = moveDegree;

	//度数をラジアンに直す
	moveAngle = DirectX::XMConvertToRadians(this->moveDegree + 90);

	//オブジェクトの向きを進行方向にセット
	XMFLOAT3 rota = { 0, 0, this->moveDegree };
	enemyObject->SetRotation(rota);

	//移動量をセット
	vel.x = moveSpeed * cosf(moveAngle);
	vel.y = moveSpeed * sinf(moveAngle);
}

void Porta::ReflectionX()
{
	//スピードアップ
	SpeedUp();

	//左右反射用に反射角をセットする
	float reflectionAngle = 360 - moveDegree;
	SetMoveAngle(reflectionAngle);
}

void Porta::ReflectionY()
{
	//スピードアップ
	SpeedUp();

	//上下反射用に反射角をセットする
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);
}

void Porta::SpeedUp()
{
	//上限に達していない場合速度を上げる
	const float maxSpeed = 1.1f;
	const float speedUp = 0.2f;
	if (moveSpeed < maxSpeed)
	{
		moveSpeed += speedUp;
	}
}
