#include "GaruEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

GaruEnemy::~GaruEnemy()
{
	safe_delete(enemyObject);
	safe_delete(stayPointObject);
}

void GaruEnemy::Update(StageEffect *effects)
{
	//存在がある場合
	if (isExistence)
	{
		//スポーン中の処理
		if (isDuringSpawn)
		{
			//敵をスポーン
			Spawn();

			//停止座標オブジェクトを更新
			stayPointObject->Update();
		}
		//スポーン中以外の処理
		else
		{
			//生きている場合
			if (isAlive)
			{
				//弾発射処理
				ShotBullet();

				//逃走
				Escape();
			}
			//死亡した場合
			else
			{
				//ノックバック後のエフェクト時間
				if (isEffect)
				{
					effectCount--;
					//エフェクト時間が0以下になったら
					if (effectCount <= 0)
					{
						//存在がなくなる
						isExistence = false;
						//エフェクトを終了
						isEffect = false;
						//ノックバックが終わった瞬間
						triggerEndKnockBack = true;
					}
				}
				//ノックバック処理
				else
				{
					KnockBack(effects);
				}
			}
		}
	}

	//敵オブジェクト更新
	enemyObject->Update();
}

void GaruEnemy::Draw()
{
	//敵オブジェクト描画
	enemyObject->Draw();

	//スポーン中以外なら飛ばす
	if (!isDuringSpawn) { return; }
	//停止座標オブジェクトを描画
	stayPointObject->Draw();
}

void GaruEnemy::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	HP -= damagePower;
}

void GaruEnemy::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void GaruEnemy::SetDelete()
{
	//削除する
	isDelete = true;
}

void GaruEnemy::SetKnockBack(float angle, int power)
{
	//ノックバックに使用する角度と強さをセット
	this->knockBackAngle = angle;
	this->killBulletPower = power;

	//死亡状態にする
	Dead();
}

bool GaruEnemy::TriggerEndKnockBack()
{
	//ノックバックが終わった瞬間なら
	if (triggerEndKnockBack)
	{
		//次のフレームはトリガーではないのでfalseに戻す
		triggerEndKnockBack = false;

		return true;
	}

	//ノックバックが終わった瞬間ではない
	return false;
}

void GaruEnemy::Spawn()
{
	//スポーンを行う時間
	const int spawnTime = 150;

	//スポーンタイマー更新
	spawnTimer++;

	//イージング計算用の時間
	float easeTimer = (float)spawnTimer / spawnTime;
	//スポーン時の画面外からの座標移動
	XMFLOAT3 pos = {};
	pos.x = Easing::OutCubic(spawnPosition.x, stayPosition.x, easeTimer);
	pos.y = Easing::OutCubic(spawnPosition.y, stayPosition.y, easeTimer);
	//更新したアルファ値をセット
	enemyObject->SetPosition(pos);

	//スポーン中の色アルファ値
	float colorAlpha = Easing::OutCubic(0.0f, 1.0f, easeTimer);
	//更新したアルファ値をセット
	enemyObject->SetColor({ 1, 1, 1, colorAlpha });

	//タイマーが指定した時間になったら
	if (spawnTimer >= spawnTime)
	{
		//スポーン終了
		isDuringSpawn = false;
	}
}

void GaruEnemy::ShotBullet()
{
	//弾は毎フレーム発射しないのでfalseに戻しておく
	isBulletShot = false;
	//弾発射タイマーを更新する
	bulletShotTimer++;
	//弾発射タイマーが一定時間までカウントされたら
	const int bulletInterval = 200;
	if (bulletShotTimer >= bulletInterval)
	{
		//弾発射タイマー初期化
		bulletShotTimer = 0;

		//弾発射
		isBulletShot = true;
	}
}

void GaruEnemy::KnockBack(StageEffect *effect)
{
	//ノックバックを行う時間
	const int knockBackTime = 20;

	//ノックバックタイマー更新
	knockBackTimer++;

	//イージング計算用の時間
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//ノックバック基準の速度
	const float knockBackStartSpeed = 1.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);
	int power = killBulletPower / 10;

	//座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x -= knockBackSpeed * sinf(knockBackAngle) * power;
	pos.y += knockBackSpeed * cosf(knockBackAngle) * power;
	//更新した座標をセット
	enemyObject->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (knockBackTimer >= knockBackTime)
	{
		//敵が倒されたときのエフェクト
		effectCount = effect->SetEnemeyDead1(enemyObject->GetPosition());
		isEffect = true;
	}
}

void GaruEnemy::Escape()
{
	//逃走していない場合
	if (!isEscape)
	{
		//生きた時間タイマーを更新
		aliveTimer++;
		const int aliveTime = 300;

		//一定時間生きたら
		if (aliveTimer >= aliveTime)
		{
			//逃走を開始する
			isEscape = true;
		}
	}

	//逃走を行っている場合
	else
	{
		//逃走を行う時間
		const int escapeTime = 200;

		//逃走用タイマー更新
		escapeTimer++;

		//イージング計算用の時間
		float easeTimer = (float)escapeTimer / escapeTime;
		//逃走中の色アルファ値
		float colorAlpha = Easing::OutCubic(1.0f, 0.0f, easeTimer);

		//更新したアルファ値をセット
		enemyObject->SetColor({ 1, 1, 1, colorAlpha });

		//タイマーが指定した時間になったら
		if (escapeTimer >= escapeTime)
		{
			//敵を削除する
			SetDelete();
		}
	}
}

