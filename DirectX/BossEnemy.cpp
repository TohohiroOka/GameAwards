#include "BossEnemy.h"
#include "SafeDelete.h"

DirectX::XMFLOAT2 BossEnemy::frameLine = { 97, 53 };

BossEnemy::~BossEnemy()
{
    safe_delete(bossObject);
}

bool BossEnemy::Initialize(Model* model)
{
	//オブジェクト生成
	bossObject = Object3d::Create();
	if (bossObject == nullptr) {
		return false;
	}

	//モデルをセット
	if (model) {
		bossObject->SetModel(model);
	}

	return true;
}

void BossEnemy::Draw()
{
	//ボスオブジェクト描画
	bossObject->Draw();
}

void BossEnemy::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	HP -= damagePower;
}

void BossEnemy::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void BossEnemy::SetDelete()
{
	//削除する
	isDelete = true;
}

void BossEnemy::Reset()
{
	//体力をリセット
	HP = 20;
	//行動を最初に戻す
	action = 0;
	//行動シーンを最初に戻す
	actionScene = 0;
	//次の行動シーンにしない
	isChangeActionScene = false;
	//行動した時間を計測するタイマーを初期化
	actionTimer = 0;
	//死んでたら生き返る
	isAlive = true;
	//削除状態を解除
	isDelete = false;
	//スポーン中にする
	isDuringSpawn = true;
	//スポーンタイマーを初期化
	spawnTimer = 0;
	//弾を発射しない
	isBulletShot = false;
	//弾発射カウントを初期化
	shotCount = 0;
	//弾発射からの時間を初期化
	bulletShotTimer = 0;
	//強い衝撃をかけない
	isImpact = false;
}

bool BossEnemy::TriggerImpact()
{
	//強い衝撃を与えるなら
	if (isImpact)
	{
		//トリガーなのでfalseに戻しておく
		isImpact = false;

		//trueを返す
		return true;
	}

	return false;
}
