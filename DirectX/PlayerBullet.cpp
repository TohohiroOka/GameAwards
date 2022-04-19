#include "PlayerBullet.h"
#include "SafeDelete.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 PlayerBullet::deadPos = { 110, 60 };

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
	//弾が知っている線のリスト解放
	alreadyLines.clear();

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

void PlayerBullet::BulletStart(XMFLOAT3 position, XMFLOAT3 rotation, int power)
{
	//発射位置、弾の角度、発射角度を設定
	bulletObject->SetPosition(position);
	bulletObject->SetRotation(rotation);
	//発射角度を設定するために角度をラジアンに直す
	this->angle = DirectX::XMConvertToRadians(rotation.z);

	//弾の威力をセット
	this->power = power;
	if (power == 10) { bulletObject->SetColor({ 0.2f, 0.2f, 0.9f, 1 }); }
	else if (power == 20) { bulletObject->SetColor({ 0.2f, 0.9f, 0.9f, 1 }); }
	else if (power == 30) { bulletObject->SetColor({ 0.2f, 0.9f, 0.2f, 1 }); }
	else if (power == 40) { bulletObject->SetColor({ 0.9f, 0.9f, 0.2f, 1 }); }
	else if (power == 50) { bulletObject->SetColor({ 0.9f, 0.2f, 0.2f, 1 }); }
	else if (power == 60) { bulletObject->SetColor({ 0.9f, 0.9f, 0.9f, 1 }); }

	//発射状態にする
	isAlive = true;
}

void PlayerBullet::Dead()
{
	//弾が知っている線のリスト解放
	alreadyLines.clear();

	//弾を発射状態ではなくする
	isAlive = false;
}

bool PlayerBullet::IsKnowLine(PowerUpLine* line)
{
	//引数の線が既に知っているか確認
	for (auto itr = alreadyLines.begin(); itr != alreadyLines.end(); itr++)
	{
		//既に知っていたらtrueを返す
		if (line == (*itr))
		{
			return true;
		}
	}

	//全て確認しても知らなかったら新たに追加する
	alreadyLines.push_front(line);

	//知らなかった場合はfalse
	return false;
}

void PlayerBullet::Move()
{
	float moveSpeed = 3.0f;
	XMFLOAT3 pos = bulletObject->GetPosition();
	pos.x -= moveSpeed * sinf(angle);
	pos.y += moveSpeed * cosf(angle);
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
