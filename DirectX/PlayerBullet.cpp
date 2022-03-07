#include "PlayerBullet.h"
#include "SafeDelete.h"

PlayerBullet *PlayerBullet::Create(Model *model)
{
	//インスタンスを生成
	PlayerBullet *instance = new PlayerBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PlayerBullet::~PlayerBullet()
{
	safe_delete(bulletObject);
}

bool PlayerBullet::Initialize(Model *model)
{
	//弾オブジェクト生成
	bulletObject = Object3d::Create();
	if (bulletObject == nullptr) {
		return false;
	}

	//大きさをセット
	bulletObject->SetScale({ 0.2f, 0.2f, 0.2f });

	//モデルをセット
	if (model) {
		bulletObject->SetModel(model);
	}

	return true;
}

void PlayerBullet::Update()
{
	//発射中の弾のみ更新
	if (!isAlive) return;

	//弾を動かす
	Move();

	//オブジェクト更新
	bulletObject->Update();
}

void PlayerBullet::Draw()
{
	//発射中の弾のみ描画
	if (!isAlive) return;

	//オブジェクト描画
	bulletObject->Draw();
}

void PlayerBullet::BulletStart(XMFLOAT3 position, XMFLOAT3 rotation)
{
	//発射位置、弾の角度、発射角度を設定
	bulletObject->SetPosition(position);
	bulletObject->SetRotation(rotation);
	//発射角度を設定するために角度をラジアンに直す
	this->angle = DirectX::XMConvertToRadians(rotation.z);
	//this->angle = rotation.z * (3.141592f * 2) / 360;
	this->power = 10;

	//発射状態にする
	isAlive = true;
}

void PlayerBullet::Dead()
{
	//弾を発射状態ではなくする
	isAlive = false;
}

void PlayerBullet::PowerUp()
{
	/*XMFLOAT3 scale = bulletObject->GetScale();
	scale.x += 0.01f;
	scale.y += 0.01f;
	bulletObject->SetScale(scale);*/

	//弾の威力を強くする
	power += 2;
}

void PlayerBullet::Move()
{
	float moveSpeed = 1.0f;
	XMFLOAT3 pos = bulletObject->GetPosition();
	pos.x -= moveSpeed * sinf(angle);
	pos.y += moveSpeed * cosf(angle);
	//更新した座標をセット
	bulletObject->SetPosition(pos);

	//範囲外まで進んだら
	XMFLOAT2 bulletAliveLineMin = { -100.0f, -100.0f };
	XMFLOAT2 bulletAliveLineMax = { 100.0f, 100.0f };
	XMFLOAT3 size = bulletObject->GetScale();
	if (pos.x < bulletAliveLineMin.x - size.x || pos.y < bulletAliveLineMin.y - size.y
		|| pos.x > bulletAliveLineMax.x + size.x || pos.y > bulletAliveLineMax.y + size.y)
	{
		//弾を消す
		Dead();
	}
}
