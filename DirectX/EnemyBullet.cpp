#include "EnemyBullet.h"
#include "SafeDelete.h"

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

void EnemyBullet::BulletStart(XMFLOAT3 position, XMFLOAT3 targetPosition)
{
	//発射位置、弾の角度、発射角度を設定
	bulletObject->SetPosition(position);

	//発射角度を設定する（標的に向かって一直線）
	float radian = atan2f(targetPosition.y - position.y, targetPosition.x - position.x);
	angle = radian;

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
