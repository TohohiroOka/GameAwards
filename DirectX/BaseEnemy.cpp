#include "BaseEnemy.h"
#include "SafeDelete.h"
#include "Easing.h"
#include "StageEffect.h"

DirectX::XMFLOAT2 BaseEnemy::wallLineMin = { 196, 110 };
DirectX::XMFLOAT2 BaseEnemy::wallLineMax = { 196, 110 };
DirectX::XMFLOAT3 BaseEnemy::targetPos = {};
bool BaseEnemy::isResultMove = false;

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
		//リザルトシーン用の動き
		else if (isResultMove)
		{
			ResultMove();
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

void BaseEnemy::Damage()
{
	//HPを1減らす
	HP--;

	//HPが0になったら死亡
	if (HP <= 0)
	{
		isAlive = false;
	}
}

void BaseEnemy::SetDelete()
{
	//削除する
	isDelete = true;
}

void BaseEnemy::SetKnockBack(float angle, int powerLevel, float powerMagnification, int shockWaveGroup)
{
	//ノックバックに使用する角度をセット
	knockBackAngle = angle;


	//ノックバックの強さと時間を決める
	//プレイヤーから出る通常衝撃波
	if (powerLevel == 1) { knockBackPower = 5.0f * powerMagnification; knockBackTime = (int)(40 * powerMagnification); }
	//巨大衝撃波1～3段階
	else if (powerLevel == 2) { knockBackPower = 6.0f * powerMagnification; knockBackTime = (int)(50 * powerMagnification); }
	else if (powerLevel == 3) { knockBackPower = 8.0f * powerMagnification; knockBackTime = (int)(60 * powerMagnification); }
	else if (powerLevel == 4) { knockBackPower = 12.0f * powerMagnification; knockBackTime = (int)(70 * powerMagnification); }

	//ノックバックタイマーを初期化
	knockBackTimer = 0;

	//移動角度変更開始速度をセット
	changeAngleSpeed = 53 * powerMagnification;

	//最後に当たった衝撃波の種類を更新
	lastCollisionShockWave = shockWaveGroup;

	//ノックバック状態にする
	isKnockBack = true;

	//演出をセット
	StageEffect::SetPushEnemy(enemyObject->GetPosition(), angle, enemyObject->GetColor());
}

bool BaseEnemy::IsCollisionWall()
{
	//枠にぶつかっていたらtrueを返す
	XMFLOAT3 pos = enemyObject->GetPosition();
	XMFLOAT3 size = enemyObject->GetScale();
	if (pos.x <= wallLineMin.x + size.x / 2)
	{
		//枠から出ないようにする
		pos.x = wallLineMin.x + size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.x >= wallLineMax.x - size.x / 2)
	{
		//枠から出ないようにする
		pos.x = wallLineMax.x - size.x / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	if (pos.y <= wallLineMin.y + size.y / 2)
	{
		//枠から出ないようにする
		pos.y = wallLineMin.y + size.y / 2;
		enemyObject->SetPosition(pos);
		return true;
	}
	else if (pos.y >= wallLineMax.y - size.y / 2)
	{
		//枠から出ないようにする
		pos.y = wallLineMax.y - size.y / 2;
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
	vel.x = cosf(moveAngle);
	vel.y = sinf(moveAngle);
}

void BaseEnemy::KnockBack()
{
	//ノックバックタイマー更新
	knockBackTimer++;

	//イージング計算用の時間
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//ノックバック基準の速度
	const float knockBackStartSpeed = 4.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);

	//座標を更新
	XMFLOAT3 pos = enemyObject->GetPosition();
	pos.x += knockBackSpeed * cosf(knockBackAngle) * knockBackPower;
	pos.y += knockBackSpeed * sinf(knockBackAngle) * knockBackPower;
	//更新した座標をセット
	enemyObject->SetPosition(pos);

	//タイトルロゴは
	if (group == EnemyGroup::TitleLogo)
	{
		//回転させる
		float degree = moveDegree;
		degree += changeAngleSpeed;
		if (degree <= 360)
		{
			degree -= 360;
		}
		SetMoveAngle(degree);

		changeAngleSpeed -= 2.0f;
		if (changeAngleSpeed < 0) {
			changeAngleSpeed = 0;
		}
	}
	//タイトルロゴ以外は
	else
	{
		//ノックバック中の前半は
		if (knockBackTimer <= knockBackTime / 2)
		{
			//回転させる
			float degree = moveDegree;
			degree += changeAngleSpeed;
			if (degree <= 360)
			{
				degree -= 360;
			}
			SetMoveAngle(degree);

			changeAngleSpeed -= 2.0f;
			if (changeAngleSpeed < 0) {
				changeAngleSpeed = 0;
			}
		}
		//ノックバック中の後半は
		else
		{
			//ターゲットの方向を向くようにする
			float radian = atan2f(targetPos.y - pos.y, targetPos.x - pos.x);
			moveAngle = radian;
			//オブジェクトの向きを進行方向にセット ラジアンを角度に直し上向きを0に調整する
			float degree = DirectX::XMConvertToDegrees(radian);
			SetMoveAngle(degree - 90);
		}
	}

	//タイマーが指定した時間になったら
	if (knockBackTimer >= knockBackTime)
	{
		//ノックバック終了
		isKnockBack = false;
	}
}

void BaseEnemy::ReflectionX()
{
	//左右反射用に反射角をセットする
	float reflectionAngle = 360 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//速度を変更する
	moveSpeed = 1.5f;
}

void BaseEnemy::ReflectionY()
{
	//上下反射用に反射角をセットする
	float reflectionAngle = 180 - moveDegree;
	SetMoveAngle(reflectionAngle);

	//速度を変更する
	moveSpeed = 1.5f;
}

