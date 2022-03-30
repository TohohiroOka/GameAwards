#include "EnemyCircle.h"

EnemyCircle *EnemyCircle::Create(Model *model, GaruEnemy *enemy, float radius)
{
	//インスタンスを生成
	EnemyCircle *instance = new EnemyCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, enemy, radius)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool EnemyCircle::Initialize(Model *model, GaruEnemy *enemy, float radius)
{
	//円オブジェクト生成
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//敵の情報を保持
	this->enemy = enemy;

	//座標をセット 敵の中心座標と同じ
	XMFLOAT3 pos = enemy->GetPosition();
	circleObject->SetPosition(pos);

	//基準のサイズをセット
	baseRadius = radius;

	//変更前の円のサイズをセット
	changeRadiusStart = 0;
	//変更後の円のサイズをセット
	changeRadiusEnd = baseRadius;
	circleObject->SetScale({ changeRadiusStart, changeRadiusStart, 1 });

	//円のサイズを変更状態にする
	isChangeRadius = true;

	if (model) {
		circleObject->SetModel(model);
	}

	//色を緑にする
	circleObject->SetColor({ 0.2f, 1, 0.2f, 0.4f });

	return true;
}
