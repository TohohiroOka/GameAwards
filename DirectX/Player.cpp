#include "Player.h"
#include "Input.h"
#include "Easing.h"
#include "XInputManager.h"
#include "Audio.h"
#include "SafeDelete.h"
#include "StageEffect.h"

using namespace DirectX;

XMFLOAT2 Player::moveRangeMin = {};
XMFLOAT2 Player::moveRangeMax = {};

Player* Player::Create(Model* playerModel)
{
	//インスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(playerModel)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Player::~Player()
{
	safe_delete(playerObject);
}

bool Player::Initialize(Model* playerModel)
{
	//プレイヤーオブジェクト生成
	playerObject = Object3d::Create();
	if (playerObject == nullptr) {
		return false;
	}

	//初期地点と大きさをセット
	XMFLOAT3 startpos = { 0, -150, 0 };
	XMFLOAT3 scale = { 2, 2, 1 };
	playerObject->SetPosition(startpos);
	playerObject->SetScale(scale);

	//プレイヤーのモデルをセット
	if (playerModel) {
		playerObject->SetModel(playerModel);
	}

	playerObject->Update();

	//サウンドの読み込み
	Audio* audio = Audio::GetInstance();
	sound[0] = audio->SoundLoadWave("Resources/sound/shockWave.wav");//プレイヤー衝撃波

	return true;
}

void Player::Update()
{
	XInputManager* Xinput = XInputManager::GetInstance();

	//スポーン
	if (isSpawn)
	{
		Spawn();
	}
	//ゲーム開始時の座標に移動
	else if (isMoveStartPos)
	{
		MoveGameStartPos();
	}
	//ノックバック処理
	else if (isKnockback)
	{
		Knockback();
	}
	//ダメージを喰らったら硬直
	else if (isDamage)
	{
		DamageWaitingTime();
	}
	//自由に移動できるとき
	if (isFreeMove)
	{
		//移動処理
		if (Move())
		{
			//移動していたらエフェクトを出す
			StageEffect::SetPlayerMove(playerObject->GetPosition(), playerObject->GetRotation());
		}

		//ポイ捨て開始
		LitteringStart();
	}

	//画面外に出ないようにする
	CollisionFrame();


	//オブジェクト更新
	playerObject->Update();

	Xinput = nullptr;
}

void Player::Draw()
{
	//オブジェクト描画
	playerObject->Draw();
}

void Player::ResetTitle()
{
	//スポーン中か
	isSpawn = false;
	//スポーンする時間タイマー
	spawnTimer = 0;
	//ゲーム開始時の座標に移動中か
	isMoveStartPos = false;
	//ゲーム開始時の座標に移動終了したか
	isMoveStartPosEnd = false;
	//ゲーム開始時の座標に移動する時間タイマー
	moveStartPosTimer = 0;
	//移動速度
	moveSpeed = 0.5f;
	//移動角度
	moveDegree = 0;
	//自由に動けるか
	isFreeMove = false;
	//ダメージを喰らっているか
	isDamage = false;
	//ダメージを喰らってからの時間
	damageTimer = 0;
	//ノックバックするか
	isKnockback = false;
	//ノックバック時間
	knockBackTimer = 0;
	//ノックバックラジアン
	knockRadian = 0;
	//衝撃波を発射するか
	isShockWaveStart = false;

	//オブジェクト初期化
	playerObject->SetPosition({ 0, -150 ,0 });
	playerObject->SetRotation({ 0, 0 ,0 });
	playerObject->SetColor({ 1 ,1 ,1 ,1 });
	playerObject->Update();
}

void Player::ResetGame()
{
	//ゲーム開始時の座標に移動する状態にセット
	SetGameStartPos();
	//色を戻す
	playerObject->SetColor({ 1,1,1,1 });
	//ゲーム開始時の座標に移動終了したか
	isMoveStartPosEnd = false;
	//ゲーム開始時の座標に移動する時間タイマー
	moveStartPosTimer = 0;
	//移動速度
	moveSpeed = 0.5f;
	//移動角度
	moveDegree = 0;
	//ダメージを喰らっているか
	isDamage = false;
	//ダメージを喰らってからの時間
	damageTimer = 0;
	//ノックバックするか
	isKnockback = false;
	//ノックバック時間
	knockBackTimer = 0;
	//ノックバックラジアン
	knockRadian = 0;
	//衝撃波を発射するか
	isShockWaveStart = false;

	//オブジェクト更新
	playerObject->Update();
}

void Player::Damage()
{
	XInputManager* Xinput = XInputManager::GetInstance();

	//ダメージタイマーを初期化
	damageTimer = 0;

	//ダメージ状態にする
	isDamage = true;

	//色を変更する
	playerObject->SetColor({ 1,0,1,1 });
}

void Player::SetSpawn()
{
	//スポーン状態
	isSpawn = true;
	isSpawnEnd = false;

	//タイマーを初期化
	spawnTimer = 0;
}

void Player::SetGameStartPos()
{
	//ゲーム開始時の座標に移動前の座標をセット
	beforeMoveStartPos = playerObject->GetPosition();
	//ゲーム開始時の座標に移動前の角度をセット
	beforeMoveStartRota = playerObject->GetRotation();

	//タイマーを初期化
	moveStartPosTimer = 0;

	//ゲーム開始時の座標に移動する状態にセット
	isMoveStartPos = true;

	//自由に移動することはできない
	isFreeMove = false;
}

void Player::SetKnockback()
{
	//ノックバックの角度を設定する
	knockRadian = DirectX::XMConvertToRadians(playerObject->GetRotation().z);
	//ノックバックタイマーを初期化
	knockBackTimer = 0;
	//ノックバックを開始する
	isKnockback = true;
	//自由に動けないようにする
	isFreeMove = false;
}

bool Player::GetTriggerSpawnEnd()
{
	if (isSpawnEnd)
	{
		//トリガーなのでfalseに戻す
		isSpawnEnd = false;

		return true;
	}

	return false;
}

bool Player::GetTriggerMoveStartPosEnd()
{
	if (isMoveStartPosEnd)
	{
		//トリガーなのでfalseに戻す
		isMoveStartPosEnd = false;

		return true;
	}

	return false;
}

void Player::SetIsFreeMove(bool isFreeMove)
{
	this->isFreeMove = isFreeMove;

	//動けなくなった場合ダメージ状態も解除しておく
	if (isFreeMove == false)
	{
		isDamage = false;

		//色を元に戻す
		playerObject->SetColor({ 1,1,1,1 });
	}
}

void Player::Spawn()
{
	//スポーンを行う時間
	const int spawnTime = 60;

	//タイマーを更新
	spawnTimer++;

	//イージング計算用の時間
	float easeTimer = (float)spawnTimer / spawnTime;

	//イージングでタイトルロゴを動かす
	XMFLOAT3 pos = playerObject->GetPosition();
	pos.y = Easing::OutCubic(-100, -20, easeTimer);
	playerObject->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (spawnTimer >= spawnTime)
	{
		//スポーン終了
		isSpawn = false;

		//スポーン完了
		isSpawnEnd = true;
	}
}

void Player::MoveGameStartPos()
{
	//ゲーム開始時の座標に移動する時間
	const int moveTime = 80;

	//タイマーを更新
	moveStartPosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveStartPosTimer / moveTime;

	//座標をゲーム開始時の座標に移動
	XMFLOAT3 pos = playerObject->GetPosition();
	pos.x = Easing::OutCubic(beforeMoveStartPos.x, 0, easeTimer);
	pos.y = Easing::OutCubic(beforeMoveStartPos.y, -20, easeTimer);
	//更新した座標をセット
	playerObject->SetPosition(pos);

	//角度をゲーム開始時の角度に移動
	XMFLOAT3 rota = playerObject->GetRotation();
	rota.z = Easing::OutCubic(beforeMoveStartRota.z, 0, easeTimer);
	//更新した座標をセット
	playerObject->SetRotation(rota);

	//タイマーが指定した時間に到達したら
	if (moveStartPosTimer >= moveTime)
	{
		//移動終了
		isMoveStartPos = false;

		//プレイヤーが上向きなので移動角度を0にする
		moveDegree = 0;

		//移動完了
		isMoveStartPosEnd = true;
	}
}

bool Player::Move()
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//移動したらtrue
	bool isMove = false;

	//デバック用キー移動
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		//プレイヤーは移動している(キー入力)方向を向く
		float keyRota;
		if (input->PushKey(DIK_LEFT)) { keyRota = 90; }
		if (input->PushKey(DIK_RIGHT)) { keyRota = 270; }
		if (input->PushKey(DIK_UP)) { keyRota = 0; }
		if (input->PushKey(DIK_DOWN)) { keyRota = 180; }

		//プレイヤーの移動角度はキー入力の角度を追従する
		if (fabsf(moveDegree - keyRota) >= 180)
		{
			if (moveDegree > keyRota)
			{
				keyRota += 360;
			} else if (moveDegree < keyRota)
			{
				keyRota -= 360;
			}
		}
		float angleChangeSpeed = 1.0f;
		if (moveDegree > keyRota + angleChangeSpeed)
		{
			moveDegree -= angleChangeSpeed;
		} else if (moveDegree < keyRota - angleChangeSpeed)
		{
			moveDegree += angleChangeSpeed;
		} else
		{
			moveDegree = keyRota;
		}

		//プレイヤー移動角度を上向き0～360に直す
		if (moveDegree < 0)
		{
			moveDegree += 360;
		} else if (moveDegree > 360)
		{
			moveDegree -= 360;
		}

		//更新した角度をセット
		playerObject->SetRotation({ 0, 0, moveDegree });

		//移動し続けると加速する
		moveSpeed += 0.02f;
		//最高速度より速くはならない
		const float moveSpeedMax = 1.0f;
		if (moveSpeed > moveSpeedMax)
		{
			moveSpeed = moveSpeedMax;
		}

		//移動速度に移動角度を乗算してプレイヤー座標を更新
		float moveAngle = XMConvertToRadians(moveDegree + 90);
		XMFLOAT3 pos = playerObject->GetPosition();
		pos.x += moveSpeed * cosf(moveAngle);
		pos.y += moveSpeed * sinf(moveAngle);
		//更新した座標をセット
		playerObject->SetPosition(pos);


		isMove = true;
	}

	//ゲームパッドでの移動
	else if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false)
		|| Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
	{
		//プレイヤーは移動している(左スティックを傾けた)方向を向く
		float padRota = Xinput->GetPadLStickAngle();
		//パッドの角度は右向きなので上向きに直す
		float changeUpRota = padRota - 90;
		//パッド上向き角度を0～360に直す
		if (changeUpRota < 0)
		{
			changeUpRota += 360;
		}

		//プレイヤーの移動角度はパッドスティックの角度を追従する
		if (fabsf(moveDegree - changeUpRota) >= 180)
		{
			if (moveDegree > changeUpRota)
			{
				changeUpRota += 360;
			} else if (moveDegree < changeUpRota)
			{
				changeUpRota -= 360;
			}
		}
		float angleChangeSpeed = 10.0f;
		if (moveDegree > changeUpRota + angleChangeSpeed)
		{
			moveDegree -= angleChangeSpeed;
		} else if (moveDegree < changeUpRota - angleChangeSpeed)
		{
			moveDegree += angleChangeSpeed;
		} else
		{
			moveDegree = changeUpRota;
		}

		//プレイヤー移動角度を上向き0～360に直す
		if (moveDegree < 0)
		{
			moveDegree += 360;
		} else if (moveDegree > 360)
		{
			moveDegree -= 360;
		}

		//更新した角度をセット
		playerObject->SetRotation({ 0, 0, moveDegree });

		//移動し続けると加速する
		moveSpeed += 0.02f;
		//最高速度より速くはならない
		const float moveSpeedMax = 1.0f;
		if (moveSpeed > moveSpeedMax)
		{
			moveSpeed = moveSpeedMax;
		}

		//移動速度に移動角度を乗算してプレイヤー座標を更新
		float moveAngle = XMConvertToRadians(moveDegree + 90);
		XMFLOAT3 pos = playerObject->GetPosition();
		pos.x += moveSpeed * cosf(moveAngle);
		pos.y += moveSpeed * sinf(moveAngle);
		//更新した座標をセット
		playerObject->SetPosition(pos);


		isMove = true;
	}
	//移動パッドの入力がない場合速度を落としていく
	else if (!(Xinput->LeftStickX(true) || Xinput->LeftStickX(false) || Xinput->LeftStickY(true) || Xinput->LeftStickY(false)))
	{
		moveSpeed -= 0.05f;

		//最低速度より遅くはならない
		const float moveSpeedMin = 0.5f;
		if (moveSpeed < moveSpeedMin)
		{
			moveSpeed = moveSpeedMin;
		}
	}

	return isMove;
}

void Player::DamageWaitingTime()
{
	//タイマーを更新
	damageTimer++;

	//タイマーが40経過したら自由に動けるようにする
	const int moveTime = 40;
	if (damageTimer >= moveTime)
	{
		//自由に動けるようにする
		isFreeMove = true;
	}

	//タイマーが一定時間経過したら
	const int waitTime = 100;
	if (damageTimer >= waitTime)
	{
		//ダメージフラグをfalseにする
		isDamage = false;

		//色を元に戻す
		playerObject->SetColor({ 1,1,1,1 });
	}
}

void Player::LitteringStart()
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();
	Audio* audio = Audio::GetInstance();

	//毎フレーム発射しないのでfalseに戻しておく
	isShockWaveStart = false;

	//指定したボタンを押すと
	if (input->TriggerKey(DIK_SPACE) ||
		Xinput->TriggerButton(XInputManager::PUD_BUTTON::PAD_RB))
	{
		//衝撃波発射
		isShockWaveStart = true;
		//ダメージを受けたのでタイマーを増やす
	}
}

void Player::Knockback()
{
	XInputManager* Xinput = XInputManager::GetInstance();

	//ノックバックを行う時間
	const int knockBackTime = 20;

	//ノックバックタイマー更新
	knockBackTimer++;

	//イージング計算用の時間
	float easeTimer = (float)knockBackTimer / knockBackTime;
	//ノックバック基準の速度
	const float knockBackStartSpeed = 1.0f;
	float knockBackSpeed = Easing::OutCubic(knockBackStartSpeed, 0, easeTimer);

	//座標を更新
	XMFLOAT3 pos = playerObject->GetPosition();
	pos.x -= -knockBackSpeed * sinf(knockRadian);
	pos.y += -knockBackSpeed * cosf(knockRadian);
	//更新した座標をセット
	playerObject->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (knockBackTimer >= knockBackTime)
	{
		//ノックバックを止める
		isKnockback = false;
	}

	Xinput = nullptr;
}

void Player::CollisionFrame()
{
	//画面外に出ないようにする
	XMFLOAT3 pos = playerObject->GetPosition();
	XMFLOAT3 size = playerObject->GetScale();
	bool isCollision = false;
	if (pos.x <= moveRangeMin.x + size.x / 2)
	{
		pos.x = moveRangeMin.x + size.x / 2;
		isCollision = true;
	} else if (pos.x >= moveRangeMax.x - size.x / 2)
	{
		pos.x = moveRangeMax.x - size.x / 2;
		isCollision = true;
	}
	if (pos.y <= moveRangeMin.y + size.y / 2)
	{
		pos.y = moveRangeMin.y + size.y / 2;
		isCollision = true;
	} else if (pos.y >= moveRangeMax.y - size.y / 2)
	{
		pos.y = moveRangeMax.y - size.y / 2;
		isCollision = true;
	}

	//枠にライン当たっていたら
	if (isCollision)
	{
		//座標をセット
		playerObject->SetPosition(pos);
	}
}