#include "TitleLogo.h"
#include "Easing.h"

Model* TitleLogo::titleLogoModel = nullptr;
bool TitleLogo::isSpawnEnd = false;
const DirectX::XMFLOAT3 TitleLogo::spawnPos = { 0, 100, 0 };
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
