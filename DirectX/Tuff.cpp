#include "Tuff.h"
#include "SafeDelete.h"
#include "Easing.h"

Tuff* Tuff::Create(Model* model)
{
	//インスタンスを生成
	Tuff* instance = new Tuff();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Tuff::~Tuff()
{
	safe_delete(bossObject);
}

bool Tuff::Initialize(Model* model)
{
	//オブジェクト生成など
	if (!BossEnemy::Initialize(model))
	{
		return false;
	}

	//ボスがタッフだということを設定
	name = BossName::Tuff;

	//座標をセット
	bossObject->SetPosition({ 0, 0, 0 });

	//大きさをセット
	bossObject->SetScale({ 4, 4, 1 });

	//色をなしにする
	bossObject->SetColor({ 1, 1, 1, 0 });

	return true;
}

void Tuff::Update(XMFLOAT3 targetPosition)
{
	//スポーン中の処理
	if (isDuringSpawn)
	{
		//敵をスポーン
		Spawn();
	}
	else
	{
		//生きていたら
		if (isAlive)
		{
			//行動シーンを次に移す
			if (isChangeActionScene)
			{
				ChangeAction();
			}

			//行動パターンにあった動きをする
			if (action == MovementPattern::ThreeWayBullet)
			{
				ShotBulletThreeWay();
			}
			else if (action == MovementPattern::MoveRandomAngle)
			{
				MoveAngle();
			}
			else if (action == MovementPattern::FiveWayBullet)
			{
				ShotBulletFiveWay();
			}
			else if (action == MovementPattern::MoveTopScreen)
			{
				MoveTop();
			}
			else if (action == MovementPattern::FallLockOn)
			{
				LockOnTargetHead(targetPosition);
			}
			else if (action == MovementPattern::Shake)
			{
				ShakeObject();
			}
			else if (action == MovementPattern::Fall)
			{
				FallObject();
			}
			else if (action == MovementPattern::MoveRandomPos)
			{
				MovePos();
			}

			//枠のラインを越えたら座標を戻す
			CollisitonFrame();
		}
	}

	//ボスオブジェクト更新
	bossObject->Update();
}

void Tuff::Spawn()
{
	//スポーンを行う時間
	const int spawnTime = 200;

	//スポーンタイマー更新
	spawnTimer++;

	//イージング計算用の時間
	float easeTimer = (float)spawnTimer / spawnTime;

	//スポーン座標
	XMFLOAT3 spawnPosition = { 0, 400, 0 };
	//停止地点
	XMFLOAT3 stayPosition = { 0, 20, 0 };

	//スポーン時の画面外からの座標移動
	XMFLOAT3 pos = {};
	pos.x = Easing::OutCubic(spawnPosition.x, stayPosition.x, easeTimer);
	pos.y = Easing::OutCubic(spawnPosition.y, stayPosition.y, easeTimer);
	//更新した座標値をセット
	bossObject->SetPosition(pos);

	//スポーン中の色アルファ値
	float colorAlpha = Easing::InQuint(0.0f, 1.0f, easeTimer);
	//更新したアルファ値をセット
	bossObject->SetColor({ 1, 1, 1, colorAlpha });

	//タイマーが指定した時間になったら
	if (spawnTimer >= spawnTime)
	{
		//スポーン終了
		isDuringSpawn = false;
	}
}

void Tuff::ChangeAction()
{
	//次の行動シーンへ
	actionScene++;

	//ループする
	if (actionScene > 11)
	{
		actionScene = 0;
	}

	//それぞれのシーン番号にあった行動を設定
	if (actionScene == 0) { action = MovementPattern::ThreeWayBullet; }
	else if (actionScene == 1) { action = MovementPattern::MoveRandomAngle; }
	else if (actionScene == 2) { action = MovementPattern::FiveWayBullet; }
	else if (actionScene == 3) { action = MovementPattern::MoveRandomAngle; }
	else if (actionScene == 4) { action = MovementPattern::ThreeWayBullet; }
	else if (actionScene == 5) { action = MovementPattern::MoveRandomAngle; }
	else if (actionScene == 6) { action = MovementPattern::FiveWayBullet; }
	else if (actionScene == 7) { action = MovementPattern::MoveTopScreen; }
	else if (actionScene == 8) { action = MovementPattern::FallLockOn; }
	else if (actionScene == 9) { action = MovementPattern::Shake; }
	else if (actionScene == 10) { action = MovementPattern::Fall; }
	else if (actionScene == 11) { action = MovementPattern::MoveRandomPos; }

	if (action == MovementPattern::MoveRandomAngle)
	{
		//ランダムで角度をセット
		SetRandomAngle();
	}
	else if (action == MovementPattern::MoveTopScreen)
	{
		//画面上部への移動をセット
		SetMoveTop();
	}
	else if (action == MovementPattern::Shake)
	{
		//シェイク情報をセット
		SetShake();
	}
	else if (action == MovementPattern::Fall)
	{
		//落下情報をセット
		SetFall();
	}
	else if (action == MovementPattern::MoveRandomPos)
	{
		//ランダムで座標をセット
		SetRandomPos();
	}

	//falseに戻しておく
	isChangeActionScene = false;
}

void Tuff::ShotBulletThreeWay()
{
	//弾は毎フレーム発射しないのでfalseに戻しておく
	isBulletShot = false;
	//弾発射タイマーを更新する
	bulletShotTimer++;
	//弾発射タイマーが一定時間までカウントされたら
	const int bulletInterval = 50;
	if (bulletShotTimer >= bulletInterval)
	{
		//弾発射タイマー初期化
		bulletShotTimer = 0;
		//弾を撃った回数を更新
		shotCount++;
		//弾発射
		isBulletShot = true;
	}

	//指定した回数弾を発射したら次の行動に移す
	const int shotNum = 3;
	if (shotCount >= shotNum)
	{
		//弾を撃った回数を初期化
		shotCount = 0;
		//次の行動へ移行
		isChangeActionScene = true;
	}
}

void Tuff::ShotBulletFiveWay()
{
	//弾は毎フレーム発射しないのでfalseに戻しておく
	isBulletShot = false;
	//弾発射タイマーを更新する
	bulletShotTimer++;
	//弾発射タイマーが一定時間までカウントされたら
	const int bulletInterval = 30;
	if (bulletShotTimer >= bulletInterval)
	{
		//弾発射タイマー初期化
		bulletShotTimer = 0;
		//弾を撃った回数を更新
		shotCount++;
		//弾発射
		isBulletShot = true;
	}

	//指定した回数弾を発射したら次の行動に移す
	const int shotNum = 2;
	if (shotCount >= shotNum)
	{
		//弾を撃った回数を初期化
		shotCount = 0;
		//次の行動へ移行
		isChangeActionScene = true;
	}
}

void Tuff::SetRandomAngle()
{
	//ランダムで8方向を指定する
	int random = rand() % 8;
	float angle = 0;
	if (random == 0) { angle = 0; }
	else if (random == 1) { angle = 45; }
	else if (random == 2) { angle = 90; }
	else if (random == 3) { angle = 135; }
	else if (random == 4) { angle = 180; }
	else if (random == 5) { angle = 225; }
	else if (random == 6) { angle = 270; }
	else if (random == 7) { angle = 315; }

	//発射角度を設定するために角度をラジアンに直す(右方向が0なので上方向にずらす)
	moveAngle = DirectX::XMConvertToRadians(angle + 90);
}

void Tuff::MoveAngle()
{
	//この行動をした時間を更新
	actionTimer++;

	moveSpeed += 0.02f;
	XMFLOAT3 pos = bossObject->GetPosition();
	pos.x += moveSpeed * cosf(moveAngle);
	pos.y += moveSpeed * sinf(moveAngle);
	//更新した座標をセット
	bossObject->SetPosition(pos);

	//行動が一定時間経過したら次の行動に移す
	const int actionTime = 40;
	if (actionTimer >= actionTime)
	{
		//移動速度を初期化
		moveSpeed = 0;
		//行動タイマーを初期化
		actionTimer = 0;
		//次の行動へ移行
		isChangeActionScene = true;
	}
}

void Tuff::SetMoveTop()
{
	//イージングのスタート座標をセット
	easeStartPos = bossObject->GetPosition();
}

void Tuff::MoveTop()
{
	//移動を行う時間
	const int moveTime = 80;

	//この行動をした時間を更新
	actionTimer++;

	//イージング計算用の時間
	float easeTimer = (float)actionTimer / moveTime;

	//スポーン時の画面外からの座標移動
	XMFLOAT3 pos = bossObject->GetPosition();
	//画面上部に移動するようにセット
	float topY = frameLine.y - 10;
	pos.y = Easing::OutCubic(easeStartPos.y, topY, easeTimer);
	//更新した座標をセット
	bossObject->SetPosition(pos);

	if (actionTimer >= moveTime)
	{
		//行動タイマーを初期化
		actionTimer = 0;
		//次の行動へ移行
		isChangeActionScene = true;
	}
}

void Tuff::LockOnTargetHead(XMFLOAT3 targetPosition)
{
	//この行動をした時間を更新
	actionTimer++;

	//X軸の座標を標的に追従させる
	XMFLOAT3 pos = bossObject->GetPosition();

	//ターゲットがボスより左側にいれば左に動かす
	float speedAccle = 0.12f;
	if (pos.x > targetPosition.x - 2.0f)
	{
		//ターゲットを追い越してしまったときのみ旋回
		if (moveSpeed > 0)
		{
			speedAccle = 0.36f;
		}

		//速度を変えることによって座標を動かす
		moveSpeed -= speedAccle;
	}
	//ターゲットがボスより右側にいれば右に動かす
	if (pos.x < targetPosition.x + 2.0f)
	{
		//ターゲットを追い越してしまったときのみ旋回
		if (moveSpeed < 0)
		{
			speedAccle = 0.36f;
		}

		//速度を変えることによって座標を動かす
		moveSpeed += speedAccle;
	}
	//最大速度を設定
	const float maxSpeed = 3.0f;
	if (moveSpeed > maxSpeed)
	{
		moveSpeed = maxSpeed;
	}
	else if (moveSpeed < -maxSpeed)
	{
		moveSpeed = -maxSpeed;
	}
	//速度を座標に加算して動かす
	pos.x += moveSpeed;
	bossObject->SetPosition(pos);

	//行動が一定時間経過したら次の行動に移す
	const int lockOnTime = 400;
	if (actionTimer >= lockOnTime)
	{
		//移動速度を初期化
		moveSpeed = 0;
		//行動タイマーを初期化
		actionTimer = 0;
		//次の行動へ移行
		isChangeActionScene = true;
	}
}

void Tuff::SetRandomPos()
{
	//ランダムで座標を指定する
	int random = rand() % 1;
	XMFLOAT3 pos = {};
	if (random == 0) { pos.x = 0; pos.y = 0; }

	//イージングのスタート座標をセット
	easeStartPos = bossObject->GetPosition();
	//イージングのエンド座標をセット
	easeEndPos = pos;
}

void Tuff::SetShake()
{
	//シェイクの基準の座標をセット
	shakeBasePos = bossObject->GetPosition();
}

void Tuff::ShakeObject()
{
	//シェイクを行う時間
	const int shakeTime = 60;

	//この行動をした時間を更新
	actionTimer++;

	//イージング計算用の時間
	float easeTimer = (float)actionTimer / shakeTime;

	//シェイクの強さにイージングをかける
	float shakePower = 0;
	shakePower = Easing::InQuad(0, 1, easeTimer);

	//シェイクで動かす
	int randAngle = rand() % 360;
	//シェイク角度を設定するために角度をラジアンに直す(右方向が0なので上方向にずらす)
	float shakeAngle = DirectX::XMConvertToRadians((float)randAngle + 90);

	//角度にシェイクの強さを乗算してシェイクさせる
	XMFLOAT3 pos = shakeBasePos;
	pos.x += shakePower * cosf(shakeAngle);
	pos.y += shakePower * sinf(shakeAngle);
	//更新した座標をセット
	bossObject->SetPosition(pos);

	//行動が一定時間経過したら次の行動に移す
	if (actionTimer >= shakeTime)
	{
		//座標を基準の位置に戻す
		bossObject->SetPosition(shakeBasePos);
		//行動タイマーを初期化
		actionTimer = 0;
		//次の行動へ移行
		isChangeActionScene = true;
	}
}

void Tuff::SetFall()
{
	//イージングのスタート座標をセット
	XMFLOAT3 startPos = bossObject->GetPosition();
	easeStartPos = startPos;
}

void Tuff::FallObject()
{
	//この行動を行う時間
	const int actionTime = 100;
	//この行動をした時間を更新
	actionTimer++;

	//落下を行う時間
	const int fallTime = 40;
	if (actionTimer <= fallTime)
	{
		//イージング計算用の時間
		float easeTimer = (float)actionTimer / fallTime;

		//イージングで下枠まで落下させる
		XMFLOAT3 pos = bossObject->GetPosition();
		pos.y = Easing::InCubic(easeStartPos.y, -frameLine.y, easeTimer);
		//更新した座標値をセット
		bossObject->SetPosition(pos);

		//落下地点に着いた瞬間
		if (actionTimer == fallTime)
		{
			//強い衝撃を与える
			isImpact = true;
		}
	}
	//行動が一定時間経過したら次の行動に移す
	if (actionTimer >= actionTime)
	{
		//行動タイマーを初期化
		actionTimer = 0;
		//次の行動へ移行
		isChangeActionScene = true;
	}
}

void Tuff::MovePos()
{
	//移動を行う時間
	const int moveTime = 150;

	//この行動をした時間を更新
	actionTimer++;

	//イージング計算用の時間
	float easeTimer = (float)actionTimer / moveTime;

	//スポーン時の画面外からの座標移動
	XMFLOAT3 pos = {};
	pos.x = Easing::OutCubic(easeStartPos.x, easeEndPos.x, easeTimer);
	pos.y = Easing::OutCubic(easeStartPos.y, easeEndPos.y, easeTimer);
	//更新した座標値をセット
	bossObject->SetPosition(pos);

	//行動が一定時間経過したら次の行動に移す
	if (actionTimer >= moveTime)
	{
		//行動タイマーを初期化
		actionTimer = 0;
		//次の行動へ移行
		isChangeActionScene = true;
	}
}

void Tuff::CollisitonFrame()
{
	//画面外に出ないようにする
	XMFLOAT3 pos = bossObject->GetPosition();
	XMFLOAT3 size = bossObject->GetScale();
	bool isCollision = false;
	if (pos.x <= -frameLine.x + size.x)
	{
		pos.x = -frameLine.x + size.x;
		isCollision = true;
	}
	else if (pos.x >= frameLine.x - size.x)
	{
		pos.x = frameLine.x - size.x;
		isCollision = true;
	}
	if (pos.y <= -frameLine.y + size.y / 2)
	{
		pos.y = -frameLine.y + size.y / 2;
		isCollision = true;
	}
	else if (pos.y >= frameLine.y - size.y * 1.4f)
	{
		pos.y = frameLine.y - size.y * 1.4f;
		isCollision = true;
	}

	//枠にライン当たっていたら
	if (isCollision)
	{
		//座標をセット
		bossObject->SetPosition(pos);

		//移動速度を初期化
		moveSpeed = 0;
	}
}
