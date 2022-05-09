#include "PlayerBullet.h"
#include "SafeDelete.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 PlayerBullet::deadPosMin = {};
DirectX::XMFLOAT2 PlayerBullet::deadPosMax = {};

PlayerBullet* PlayerBullet::Create(Model* model)
{
	//インスタンスを生成
	PlayerBullet* instance = new PlayerBullet();
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
	//オブジェクト解放
	safe_delete(bulletObject);
}

bool PlayerBullet::Initialize(Model* model)
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

	bulletObject->SetBloom(true);

	return true;
}

void PlayerBullet::Update()
{
	//生きているときのみ
	if (isAlive)
	{
		//弾を動かす
		Move();

		//生存時間更新
		AliveTimeUpdate();
	}
	else
	{
		//衝撃波を発射しない状態で固定しておく
		isShockWaveStart = false;
	}

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

void PlayerBullet::Reset()
{
	//発射角度
	angle = 0.0f;
	//弾の生存時間タイマー
	aliveTimer = 0;
	//弾が生きているか
	isAlive = false;
	//衝撃波を発射するか
	isShockWaveStart = false;
}

void PlayerBullet::BulletStart(XMFLOAT3 position, float angle)
{
	//発射位置、弾の角度、発射角度を設定
	bulletObject->SetPosition(position);
	XMFLOAT3 rota = { 0, 0, angle };
	bulletObject->SetRotation(rota);
	//発射角度を設定するために角度をラジアンに直す(右方向が0なので上方向にずらす)
	this->angle = DirectX::XMConvertToRadians(angle);

	//生存時間タイマーを初期化
	aliveTimer = 0;

	//発射状態にする
	isAlive = true;

	//衝撃波を発射しない
	isShockWaveStart = false;
}

void PlayerBullet::Dead()
{
	//弾を発射状態ではなくする
	isAlive = false;

	//衝撃波を発射する
	isShockWaveStart = true;
}

void PlayerBullet::Move()
{
	float moveSpeed = 5.0f;
	XMFLOAT3 pos = bulletObject->GetPosition();
	pos.x -= moveSpeed * sinf(angle);
	pos.y += moveSpeed * cosf(angle);
	//更新した座標をセット
	bulletObject->SetPosition(pos);

	//画面外まで進んだら
	XMFLOAT3 size = bulletObject->GetScale();
	if (pos.x <= deadPosMin.x + size.x / 2 || pos.y <= deadPosMin.y + size.y / 2 ||
		pos.x >= deadPosMax.x - size.x / 2 || pos.y >= deadPosMax.y - size.y / 2)
	{
		//死亡
		Dead();
	}
}

void PlayerBullet::AliveTimeUpdate()
{
	//生存可能時間
	const int aliveTime = 10;

	//生存時間更新
	aliveTimer++;

	//生存可能時間まで生きたら
	if (aliveTimer >= aliveTime)
	{
		//死亡
		Dead();
	}
}
