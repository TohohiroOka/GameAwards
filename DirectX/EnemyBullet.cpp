#include "EnemyBullet.h"
#include "SafeDelete.h"

DirectX::XMFLOAT2 EnemyBullet::deadPos = { 110, 60 };

EnemyBullet *EnemyBullet::Create(Model *model)
{
	//インスタンスを生成
	EnemyBullet *instance = new EnemyBullet();
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

EnemyBullet::~EnemyBullet()
{
	//オブジェクト解放
	safe_delete(bulletObject);
}

bool EnemyBullet::Initialize(Model *model)
{
	//弾オブジェクト生成
	bulletObject = Object3d::Create();
	if (bulletObject == nullptr) {
		return false;
	}

	//大きさをセット
	bulletObject->SetScale({ 2, 2, 1 });

	//モデルをセット
	if (model) {
		bulletObject->SetModel(model);
	}

	//色を黄色にする
	bulletObject->SetColor({ 1, 1, 0, 1 });

	return true;
}

void EnemyBullet::Update()
{
	//弾を動かす
	Move();

	//オブジェクト更新
	bulletObject->Update();
}

void EnemyBullet::Draw()
{
	//発射中の弾のみ描画
	if (!isAlive) { return; }

	//オブジェクト描画
	bulletObject->Draw();
}

void EnemyBullet::AimBulletStart(XMFLOAT3 position, XMFLOAT3 targetPosition)
{
	//発射位置を設定
	bulletObject->SetPosition(position);
	//発射角度を設定する（標的に向かって一直線）
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	angle = radian;
	//オブジェクトの角度を設定
	float degree = DirectX::XMConvertToDegrees(radian);
	XMFLOAT3 rotation = { 0, 0, degree - 90 };
	bulletObject->SetRotation(rotation);

	//発射状態にする
	isAlive = true;
}

void EnemyBullet::StraightBulletStart(XMFLOAT3 position, float angle)
{
	//発射位置、弾の角度、発射角度を設定
	bulletObject->SetPosition(position);
	XMFLOAT3 rota = { 0, 0, angle };
	bulletObject->SetRotation(rota);
	//発射角度を設定するために角度をラジアンに直す(右方向が0なので上方向にずらす)
	this->angle = DirectX::XMConvertToRadians(angle + 90);

	//発射状態にする
	isAlive = true;
}

void EnemyBullet::Dead()
{
	//弾を発射状態ではなくする
	isAlive = false;
}

void EnemyBullet::Move()
{
	float moveSpeed = 0.5f;
	XMFLOAT3 pos = bulletObject->GetPosition();
	pos.x += moveSpeed * cosf(angle);
	pos.y += moveSpeed * sinf(angle);
	//更新した座標をセット
	bulletObject->SetPosition(pos);

	//画面外まで進んだら
	XMFLOAT3 size = bulletObject->GetScale();
	if (pos.x <= -deadPos.x || pos.y <= -deadPos.y || pos.x >= deadPos.x || pos.y >= deadPos.y)
	{
		//弾を消す
		Dead();
	}
}
