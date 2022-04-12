#include "FixedEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

FixedEnemy* FixedEnemy::Create(Model* coreModel, Model* squareModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//インスタンスを生成
	FixedEnemy* instance = new FixedEnemy();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(coreModel, squareModel, spawnPosition, stayPosition)) {
		delete instance;
		assert(0);
	}

	return instance;
}

FixedEnemy::~FixedEnemy()
{
	safe_delete(coreObject);
	safe_delete(squareObject);
}

bool FixedEnemy::Initialize(Model* coreModel, Model* squareModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition)
{
	//スポーン時の座標と移動後の座標をセット
	this->spawnPosition = spawnPosition;
	this->stayPosition = stayPosition;

	//コアオブジェクト生成
	coreObject = Object3d::Create();
	if (coreObject == nullptr) {
		return false;
	}

	//初期座標セット
	coreObject->SetPosition(spawnPosition);
	//大きさをセット
	coreObject->SetScale({ 2.0f, 2.0f, 1 });

	//モデルをセット
	if (coreObject) {
		coreObject->SetModel(coreModel);
	}
	//ブルームをかける
	coreObject->SetBloom(true);


	//外枠オブジェクト生成
	squareObject = Object3d::Create();
	if (squareObject == nullptr) {
		return false;
	}

	//初期座標セット
	squareObject->SetPosition(spawnPosition);
	//大きさをセット
	squareObject->SetScale({ 0 ,0, 1 });

	//モデルをセット
	if (squareObject) {
		squareObject->SetModel(squareModel);
	}
	//ブルームをかける
	squareObject->SetBloom(true);

	return true;
}

void FixedEnemy::Update()
{
	//存在がある場合
	if (isExistence)
	{
		//スポーン中の処理
		if (isDuringSpawn)
		{
			//敵をスポーン
			Spawn();
		}
		//スポーン中以外の処理
		else
		{
			//死亡した場合
			if (!isAlive)
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
						//外枠の大きさを変更状態にセット
						isChangeSquareSize = true;
						//ノックバックが終わった瞬間
						triggerEndKnockBack = true;
					}
				}
				//ノックバック処理
				else
				{
					KnockBack();
				}
			}
		}
	}
	//存在がなくなったら
	else
	{
		//大きさ変更状態なら
		if (isChangeSquareSize)
		{
			ChangeSquareSize();
		}
	}

	//オブジェクト更新
	coreObject->Update();
	squareObject->Update();
}

void FixedEnemy::Draw()
{
	//オブジェクト描画
	coreObject->Draw();
	squareObject->Draw();
}

void FixedEnemy::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void FixedEnemy::SetDelete()
{
	//削除する
	isDelete = true;
}

void FixedEnemy::SetKnockBack(float angle, int power)
{
	//ノックバックに使用する角度と強さをセット
	this->knockBackAngle = angle;
	this->killBulletPower = power;

	//死亡状態にする
	Dead();
}

bool FixedEnemy::TriggerEndKnockBack()
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

void FixedEnemy::Spawn()
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
	//更新した座標をセット
	coreObject->SetPosition(pos);
	squareObject->SetPosition(pos);

	//スポーン中の色アルファ値
	float colorAlpha = Easing::OutCubic(0.0f, 1.0f, easeTimer);
	//更新したアルファ値をセット
	coreObject->SetColor({ 1, 1, 1, colorAlpha });

	//タイマーが指定した時間になったら
	if (spawnTimer >= spawnTime)
	{
		//スポーン終了
		isDuringSpawn = false;
	}
}

void FixedEnemy::KnockBack()
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
	XMFLOAT3 pos = coreObject->GetPosition();
	pos.x -= knockBackSpeed * sinf(knockBackAngle) * power;
	pos.y += knockBackSpeed * cosf(knockBackAngle) * power;
	//更新した座標をセット
	coreObject->SetPosition(pos);
	squareObject->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (knockBackTimer >= knockBackTime)
	{
		//敵が倒されたときのエフェクト
		effectCount = StageEffect::SetEnemeyDead(coreObject->GetPosition());
		isEffect = true;
	}
}

void FixedEnemy::ChangeSquareSize()
{
	//大きさ変更を行う時間
	const int changeTime = 20;

	//大きさ変更タイマー更新
	changeSquareSizeTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeSquareSizeTimer / changeTime;

	//大きさを更新
	float size = 0;
	size = Easing::OutBack(0, 2, easeTimer);
	//更新した座標をセット
	squareObject->SetScale({ size, size, 1 });

	//タイマーが指定した時間になったら
	if (changeSquareSizeTimer >= changeTime)
	{
		//大きさ変更終了
		isChangeSquareSize = false;
	}
}
