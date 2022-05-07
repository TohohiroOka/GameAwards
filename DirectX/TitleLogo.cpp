#include "TitleLogo.h"

Model* TitleLogo::titleLogoModel = { nullptr };


TitleLogo* TitleLogo::Create(XMFLOAT3 spawnPosition)
{
	//インスタンスを生成
	TitleLogo* instance = new TitleLogo();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(spawnPosition, 0)) {
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

bool TitleLogo::Initialize(XMFLOAT3 spawnPosition, float moveDegree)
{
	//所属グループを設定
	group = EnemyGroup::TitleLogo;

	//オブジェクト生成
	enemyObject = Object3d::Create();
	if (enemyObject == nullptr) {
		return false;
	}

	//初期座標セット
	enemyObject->SetPosition(spawnPosition);

	//大きさをセット
	enemyObject->SetScale({ 20, 20, 1 });

	//モデルをセット
	if (titleLogoModel) {
		enemyObject->SetModel(titleLogoModel);
	}

	//攻撃力をセット
	power = 1000;


	return true;
}

void TitleLogo::Move()
{
}

void TitleLogo::ResultMove()
{
}
