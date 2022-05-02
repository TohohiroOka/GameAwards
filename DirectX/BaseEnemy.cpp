#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 BaseEnemy::wallLine = { 196, 110 };

BaseEnemy::~BaseEnemy()
{
	safe_delete(enemyObject);
}

void BaseEnemy::Update()
{
	//生きている場合
	if (isAlive)
	{
		//ノックバック
		if (isKnockBack)
		{
			KnockBack();
		}
		//ノックバック時以外は移動
		else 
		{
			Move();
		}
	}
	//死亡した場合
	else
	{
		//ノックバック後のエフェクト時間
		effectCount--;
		//エフェクト時間が0以下になったら
		if (effectCount <= 0)
		{
			//存在がなくなる
			SetDelete();
		}
	}

	//敵オブジェクト更新
	enemyObject->Update();
}

void BaseEnemy::Draw()
{
	//敵オブジェクト描画
	enemyObject->Draw();
}

void BaseEnemy::Dead()
{
	//敵が倒されたときのエフェクト
	effectCount = StageEffect::SetEnemeyDead(enemyObject->GetPosition());

	//死亡状態にする
	isAlive = false;
}

void BaseEnemy::SetDelete()
{
	//削除する
	isDelete = true;
}

void BaseEnemy::SetKnockBack(float angle, int powerLevel)
{
	//ノックバックに使用する角度をセット
	knockBackAngle = angle;

	//ノックバックに使用する強さをセット
	if (powerLevel <= 3)
	{
		//衝撃波に当たるたびに吹っ飛ぶ威力を上げる
		knockBackPowerLevel += powerLevel;
		//ノックバックの強さは上限を越えない
		const int powerLevelMax = 3;
		if (knockBackPowerLevel >= powerLevelMax)
		{
			knockBackPowerLevel = powerLevelMax;
		}
	}
	//最高威力の衝撃波に当たったときのみ上限を越える
	else if (powerLevel == 4)
	{
		knockBackPowerLevel = powerLevel;
	}

	//敵の色を変更
	if (knockBackPowerLevel == 1) { enemyObject->SetColor({ 0, 1, 0, 1 }); }
	else if (knockBackPowerLevel == 2) { enemyObject->SetColor({ 1, 1, 0, 1 }); }
	else if (knockBackPowerLevel >= 3) { enemyObject->SetColor({ 1, 0, 0, 1 }); }

	//ノックバックタイマーを初期化
	knockBackTimer = 0;

	//ノックバック状態にする
	isKnockBack = true;

	//エフェクトのセット
	StageEffect::SetPushEnemy(enemyObject->GetPosition(), angle, enemyObject->GetColor());
}

bool BaseEnemy::IsCollisionWall(XMFLOAT2 wallLine)
{
	//枠にぶつかっていたらtrueを返す
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x <= -wallLine.x + size.x / 2)
	{
		//枠から出ないようにする
		pos.x = -wallLine.x + size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.x >= wallLine.x - size.x / 2)
	{
		//枠から出ないようにする
		pos.x = wallLine.x - size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	if (pos.y <= -wallLine.y + size.y / 2)
	{
		//枠から出ないようにする
		pos.y = -wallLine.y + size.y / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.y >= wallLine.y - size.y / 2)
	{
		//枠から出ないようにする
		pos.y = wallLine.y - size.y / 2;
		enemyObject->SetPosition(pos);
		return true;
	}

	//当たっていない場合はfalseを返す
	return false;
}

void BaseEnemy::SetMoveAngle(float moveDegree)
{
	//上向きを0にするため90度傾ける
	this->moveDegree = moveDegree;

	//度数をラジアンに直す
	moveAngle = DirectX::XMConvertToRadians(this->moveDegree + 90);

	//オブジェクトの向きを進行方向にセット
	XMFLOAT3 rota = { 0, 0, this->moveDegree };
	enemyObject->SetRotation(rota);

	//移動量をセット
	vel.x = moveSpeed * cosf(moveAngle);
	vel.y = moveSpeed * sinf(moveAngle);
}

void BaseEnemy::KnockBack()
{
	//ノックバックを行う時間
	const int knockBackTime = 25 + knockBackPowerLevel * 5;

	//ノックバックタイマー更新
	knockBackTimer++;

	//イージング計算用の時間
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//ノックバック基準の速度
	const float knockBackStartSpeed = 5.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);

	//座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += knockBackSpeed * cosf(knockBackAngle) * knockBackPowerLevel;
	pos.y += knockBackSpeed * sinf(knockBackAngle) * knockBackPowerLevel;
	//更新した座標をセット
	enemyObject->SetPosition(pos);


	//タイマーが指定した時間になったら
	if (knockBackTimer >= knockBackTime)
	{
		//ノックバック終了
		isKnockBack = false;
	}
}

