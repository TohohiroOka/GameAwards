#include "BossEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"

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
	//死亡してサイズを変更状態ではない
	isDeadChangeScale = false;
	//膨張前のサイズ初期化
	changeStartScale = 0;
	//膨張後のサイズ初期化
	changeEndScale = 0;
	//サイズ変更シーン初期化
	changeScaleScene = 0;
	//サイズ変更タイマー初期化
	changeScaleTimer = 0;
	//存在している
	isExistence = true;
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

void BossEnemy::SetDeadChangeScale()
{
	//膨張前のサイズをセット
	changeStartScale = bossObject->GetScale().x;
	//膨張後のサイズをセット
	changeEndScale = bossObject->GetScale().x * 3;

	//サイズ変更状態にセット
	isDeadChangeScale = true;
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

void BossEnemy::DeadChangeScale()
{
	//サイズ変更タイマー更新
	changeScaleTimer++;

	//膨張シーン
	if (changeScaleScene == 0)
	{
		//サイズ変更する時間
		const int changeTime = 18;

		//イージング計算用の時間
		float easeTimer = (float)changeScaleTimer / changeTime;

		//イージングで膨張させる
		float size = Easing::OutCubic(changeStartScale, changeEndScale, easeTimer);
		//更新したサイズをセット
		bossObject->SetScale({ size, size, 1 });

		//タイマーが指定した時間になったら
		if (changeScaleTimer >= changeTime)
		{
			//サイズ変更タイマー初期化
			changeScaleTimer = 0;

			//次のシーンへ
			changeScaleScene++;
		}
	}
	//縮小シーン
	else if (changeScaleScene == 1)
	{
		//サイズ変更する時間
		const int changeTime = 42;

		//イージング計算用の時間
		float easeTimer = (float)changeScaleTimer / changeTime;

		//イージングで縮小させる
		float size = Easing::OutCubic(changeEndScale, 0, easeTimer);
		//更新したサイズをセット
		bossObject->SetScale({ size, size, 1 });

		//タイマーが指定した時間になったら
		if (changeScaleTimer >= changeTime)
		{
			//サイズ変更状態終了
			isDeadChangeScale = false;

			//存在を消す
			isExistence = false;
		}
	}
}
