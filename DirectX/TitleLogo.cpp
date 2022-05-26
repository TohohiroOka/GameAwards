#include "TitleLogo.h"
#include "Easing.h"

Model* TitleLogo::titleLogoModel = nullptr;
bool TitleLogo::isSpawnEnd = false;
const DirectX::XMFLOAT3 TitleLogo::spawnPos = { 0, 150, 0 };
const DirectX::XMFLOAT3 TitleLogo::stayPos = { 0, 40, 0 };

TitleLogo* TitleLogo::Create()
{
	//インスタンスを生成
	TitleLogo* instance = new TitleLogo();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize({}, 0)) {
		delete instance;
		assert(0);
	}

	return instance;
}

void TitleLogo::SetModel(Model* titleLogoModel)
{
	//引数のモデルを共通で使うためセットする
	TitleLogo::titleLogoModel = titleLogoModel;
}

bool TitleLogo::GetTriggerSpawnEnd()
{
	if (isSpawnEnd)
	{
		//トリガーなのでfalseに戻す
		isSpawnEnd = false;

		return true;
	}

	return false;
}

bool TitleLogo::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//所属グループを設定
	group = EnemyGroup::TitleLogo;

	//HPを3にセット
	HP = 3;

	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//スポーン座標セット
	enemyObject->SetPosition(spawnPos);

	//大きさをセット
	enemyObject->SetScale({ 20, 20, 1 });

	//モデルをセット
	if (titleLogoModel) {
		enemyObject->SetModel(titleLogoModel);
	}

	//基準のダメージ量をセット
	baseDamagePower = 1;

	return true;
}

void TitleLogo::SetKnockBack(float angle, int powerLevel, float powerMagnification)
{
	BaseEnemy::SetKnockBack(angle, powerLevel, powerMagnification);

	//一度壁に当たったかをリセットする
	isCollisionWallFirst = false;
}

bool TitleLogo::IsCollisionWall()
{
	//枠にぶつかっていたらtrueを返す
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	bool isCollision = false;

	if (pos.x <= wallLineMin.x + size.x / 2)
	{
		//枠から出ないようにする
		pos.x = wallLineMin.x + size.x / 2;
		enemyObject->SetPosition(pos);

		//反射させる
		ReflectionX();

		//壁に当たった
		isCollision = true;
	}
	else if (pos.x >= wallLineMax.x - size.x / 2)
	{
		//枠から出ないようにする
		pos.x = wallLineMax.x - size.x / 2;
		enemyObject->SetPosition(pos);

		//反射させる
		ReflectionX();

		//壁に当たった
		isCollision = true;
	}
	if (pos.y <= wallLineMin.y + size.y / 2)
	{
		//枠から出ないようにする
		pos.y = wallLineMin.y + size.y / 2;
		enemyObject->SetPosition(pos);

		//反射させる
		ReflectionY();

		//壁に当たった
		isCollision = true;
	}
	else if (pos.y >= wallLineMax.y - size.y / 2)
	{
		//枠から出ないようにする
		pos.y = wallLineMax.y - size.y / 2;
		enemyObject->SetPosition(pos);

		//反射させる
		ReflectionY();

		//壁に当たった
		isCollision = true;
	}

	//既に一度壁に当たっている場合はfalse
	if (isCollisionWallFirst) { return false; }

	//壁に当たっていなければ抜ける
	if (!isCollision) { return false; }

	//一度壁に当たった
	isCollisionWallFirst = true;

	//壁に当たったを返す
	return true;
}

void TitleLogo::Move()
{
	//スポーン時以外は抜ける
	if (!isSpawn) { return; }

	//スポーンを行う時間
	const int spawnTime = 90;

	//タイマーを更新
	spawnTimer++;

	//イージング計算用の時間
	float easeTimer = (float)spawnTimer / spawnTime;

	//イージングでタイトルロゴを動かす
	XMFLOAT3 logoPos = enemyObject->GetPosition();
	logoPos.y = Easing::OutBounce(spawnPos.y, stayPos.y, easeTimer);
	enemyObject->SetPosition(logoPos);

	//タイマーが指定した時間になったら
	if (spawnTimer >= spawnTime)
	{
		//スポーン終了
		isSpawn = false;
		//スポーン完了
		isSpawnEnd = true;
	}
}

void TitleLogo::ResultMove()
{
}

void TitleLogo::ReflectionX()
{
	//左右反射用に反射角をセットする
	float knockBackDegree = DirectX::XMConvertToDegrees(knockBackAngle);
	float reflectionDegree = 180 - knockBackDegree;
	knockBackAngle = DirectX::XMConvertToRadians(reflectionDegree);

	//ノックバックの時間を少し増やす
	knockBackTime += 10;
}

void TitleLogo::ReflectionY()
{
	//上下反射用に反射角をセットする
	float knockBackDegree = DirectX::XMConvertToDegrees(knockBackAngle);
	float reflectionDegree = 360 - knockBackDegree;
	knockBackAngle = DirectX::XMConvertToRadians(reflectionDegree);

	//ノックバックの時間を少し増やす
	knockBackTime += 10;
}
