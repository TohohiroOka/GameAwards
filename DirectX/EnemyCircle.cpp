#include "EnemyCircle.h"

EnemyCircle* EnemyCircle::Create(Model* model, GaruEnemy* garuEnemy)
{
	//インスタンスを生成
	EnemyCircle* instance = new EnemyCircle();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, garuEnemy)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool EnemyCircle::Initialize(Model* model, GaruEnemy* garuEnemy)
{
	//円オブジェクト生成
	circleObject = Object3d::Create();
	if (circleObject == nullptr) {
		return false;
	}

	//敵の情報を保持
	this->garuEnemy = garuEnemy;

	//座標をセット 敵の中心座標と同じ
	XMFLOAT3 pos = garuEnemy->GetPosition();
	circleObject->SetPosition(pos);

	//基準の半径をセット( ガル族の大きさ ×（ 倒された時の弾の強さ / 4 ））
	float baseRadius = garuEnemy->GetScale().x * ((float)garuEnemy->GetKillBulletPower() / 4);
	this->baseRadius = baseRadius;

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
