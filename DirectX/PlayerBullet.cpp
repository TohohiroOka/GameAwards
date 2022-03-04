#include "PlayerBullet.h"
#include "SafeDelete.h"

PlayerBullet *PlayerBullet::Create(UINT texNumber)
{
	//インスタンスを生成
	PlayerBullet *instance = new PlayerBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(texNumber)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PlayerBullet::~PlayerBullet()
{
	safe_delete(bulletSprite);
}

bool PlayerBullet::Initialize(UINT texNumber)
{
	//プレイヤースプライト生成
	bulletSprite = Sprite::Create(texNumber);
	if (bulletSprite == nullptr) {
		return false;
	}

	//大きさをセット
	bulletSprite->SetSize({ 20.0f, 20.0f });

	return true;
}

void PlayerBullet::Update()
{
	//発射中の弾のみ更新
	if(!isAlive) return;

	//弾を動かす
	float moveSpeed = 15.0f;
	XMFLOAT2 pos = bulletSprite->GetPosition();
	pos.x += moveSpeed * sinf(angle);
	pos.y -= moveSpeed * cosf(angle);
	//更新した座標をセット
	bulletSprite->SetPosition(pos);

	//範囲外まで進んだら弾を消す
	XMFLOAT2 bulletAliveLineMin = { -10.0f, -10.0f };
	XMFLOAT2 bulletAliveLineMax = { 1280.0f, 720.0f };
	XMFLOAT2 size = bulletSprite->GetSize();
	if (pos.x < bulletAliveLineMin.x - size.x || pos.y < bulletAliveLineMin.y - size.y
		|| pos.x > bulletAliveLineMax.x + size.x || pos.y > bulletAliveLineMax.y + size.y)
	{
		isAlive = false;
	}

	//スプライト更新
	bulletSprite->Update();
}

void PlayerBullet::Draw()
{
	//発射中の弾のみ描画
	if(!isAlive) return;

	//スプライト描画
	bulletSprite->Draw();
}

void PlayerBullet::BulletStart(XMFLOAT2 position, float rotation)
{
	//発射位置、弾の角度、発射角度を設定
	bulletSprite->SetPosition(position);
	bulletSprite->SetRotation(rotation);
	this->angle = rotation * (3.141592f * 2) / 360;

	//発射状態にする
	isAlive = true;
}

void PlayerBullet::Dead()
{
	//弾を発射状態ではなくする
	isAlive = false;
}
