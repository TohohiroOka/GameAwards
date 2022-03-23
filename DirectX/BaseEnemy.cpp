#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
}

void BaseEnemy::Update(StageEffect* effects)
{
	//スポーン中の処理
	if (isDuringSpawn)
	{
		Spawn();
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
				EffectCount--;
				//エフェクト時間が0以下になったら
				if (EffectCount <= 0)
				{
					isExistence = false;
					isEffect = false;
				}
			}
			//ノックバック処理
			else
			{
				KnockBack(effects);
			}
		}
	}

	//オブジェクト更新
	enemyObject->Update();
}

void BaseEnemy::Draw()
{
	//オブジェクト描画
	enemyObject->Draw();
}

void BaseEnemy::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	HP -= damagePower;
}

void BaseEnemy::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void BaseEnemy::SetKnockBack(float angle, int power)
{
	//ノックバックに使用する角度と強さをセット
	this->knockBackAngle = angle;
	this->killBulletPower = power;

	//死亡状態にする
	Dead();
}

void BaseEnemy::Spawn()
{
	//スポーンを行う時間
	const int spawnTime = 50;
	//イージング計算用の時間
	float easeTimer = (float)spawnTimer / spawnTime;
	//スポーン中の色アルファ値
	float colorAlpha = Easing::InCubic(0.0f, 1.0f, easeTimer);

	//更新したアルファ値をセット
	enemyObject->SetColor({ 1, 1, 1, colorAlpha });

	//スポーンタイマー更新
	spawnTimer++;
	//タイマーが指定した時間になったら
	if (spawnTimer >= spawnTime)
	{
		//スポーン終了
		isDuringSpawn = false;
	}
}

void BaseEnemy::ShotBullet()
{
	//弾は毎フレーム発射しないのでfalseに戻しておく
	isBulletShot = false;
	//弾発射タイマーを更新する
	bulletShotTimer++;
	//弾発射タイマーが一定時間までカウントされたら
	const int bulletInterval = 300;
	if (bulletShotTimer >= bulletInterval)
	{
		//弾発射タイマー初期化
		bulletShotTimer = 0;

		//弾発射
		isBulletShot = true;
	}
}

void BaseEnemy::KnockBack(StageEffect* effect)
{
	//ノックバックを行う時間
	const int knockBackTime = 20;
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

	//ノックバックタイマー更新
	knockBackTimer++;
	//タイマーが指定した時間になったら
	if (knockBackTimer >= knockBackTime)
	{
		//敵が倒されたときのエフェクト
		EffectCount = effect->SetEnemeyDead1(enemyObject->GetPosition());
		isEffect = true;
	}
}

void BaseEnemy::Escape()
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
		//イージング計算用の時間
		float easeTimer = (float)escapeTimer / escapeTime;
		//逃走中の色アルファ値
		float colorAlpha = Easing::OutCubic(1.0f, 0.0f, easeTimer);

		//更新したアルファ値をセット
		enemyObject->SetColor({ 1, 1, 1, colorAlpha });

		//逃走用タイマー更新
		escapeTimer++;
		//タイマーが指定した時間になったら
		if (escapeTimer >= escapeTime)
		{
			//逃走完了
			isEscapeCompleted = true;
		}
	}
}

