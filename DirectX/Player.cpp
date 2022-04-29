#include "Player.h"
#include "Input.h"
#include "Easing.h"
#include "XInputManager.h"
#include "SafeDelete.h"
#include "StageEffect.h"

using namespace DirectX;

XMFLOAT2 Player::moveRange = {};

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
	XMFLOAT3 pos = { 0, 0, 0 };
	XMFLOAT3 scale = { 2, 2, 1 };
	playerObject->SetPosition(pos);
	playerObject->SetScale(scale);

	//プレイヤーのモデルをセット
	if (playerModel) {
		playerObject->SetModel(playerModel);
	}

	//ブルームをかける
	playerObject->SetBloom(true);


	return true;
}

void Player::Update()
{
	XInputManager* Xinput = XInputManager::GetInstance();
	//タイムを減らしていく
	if (vibrationTimer > 0) { vibrationTimer--; }
	//vibrationTimerが0なので振動を止める
	else if (vibrationTimer == 0) {
		vibrationTimer = -1;//初期化は-1
		Xinput->EndVibration();
	}

	//停止状態以外の場合動ける
	if (!isStop)
	{
		//ノックバック処理
		if (isKnockback)
		{
			Knockback();
		}
		//ノックバックしていない時
		else
		{
			//移動処理
			if (Move())
			{
				//移動していたらエフェクトを出す
				StageEffect::SetPlayerMove(playerObject->GetPosition(), playerObject->GetRotation());
			}

			//衝撃波発射
			ShockWaveStart();
		}

		//画面外に出ないようにする
		CollisionFrame();
	}
	//ダメージフラグがtrueなら
	if (isDamage)
	{
		//タイマーを更新
		damageTimer++;

		//タイマーが一定時間経過したら
		const int damageTime = 100;
		if (damageTimer >= damageTime)
		{
			//ダメージタイマーを初期化
			damageTimer = 0;

			//ダメージフラグをfalseにする
			isDamage = false;

			playerObject->SetColor({ 1,1,1,1 });
		}
	}

	//オブジェクト更新
	playerObject->Update();

	Xinput = nullptr;
}

void Player::Draw()
{
	//オブジェクト描画
	playerObject->Draw();
}

void Player::Reset()
{
	//座標をセット
	XMFLOAT3 pos = { 0, 500, 0 };
	playerObject->SetPosition(pos);

	//回転を戻す
	playerObject->SetRotation({});

	//大きさを戻す
	playerObject->SetScale({ 2, 2, 1 });

	//色を戻す
	playerObject->SetColor({ 1,1,1,1 });
	//ダメージを喰らっていない
	isDamage = false;
	//ダメージを喰らってからの時間初期化
	damageTimer = 0;
	//移動速度初期化
	moveSpeed = 0.5f;
	//ノックバックしない
	isKnockback = false;
	//ノックバック時間初期化
	knockBackTimer = 0;
	//ノックバックラジアン初期化
	knockRadian = 0;
	//停止状態にしておく
	isStop = true;


	//振動タイマー初期化-1
	vibrationTimer = -1;

	//オブジェクト更新
	playerObject->Update();
}

void Player::Damage()
{
	XInputManager* Xinput = XInputManager::GetInstance();

	//ダメージ状態にする
	isDamage = true;

	//色を変更する
	playerObject->SetColor({ 1,0,1,1 });

	//ダメージを受けたのでタイマーを増やす
	vibrationTimer = 10;
	//振動オン
	Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM);
}

void Player::SetKnockback()
{
	//ノックバックの角度を設定する
	knockRadian = DirectX::XMConvertToRadians(playerObject->GetRotation().z);
	//ノックバックタイマーを初期化
	knockBackTimer = 0;
	//ノックバックを開始する
	isKnockback = true;
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
		//通常時の移動速度
		float moveSpeed = 1.0f;

		//移動処理
		XMFLOAT3 pos = playerObject->GetPosition();
		if (input->PushKey(DIK_LEFT)) { pos.x -= moveSpeed, isMove = true; }
		if (input->PushKey(DIK_RIGHT)) { pos.x += moveSpeed, isMove = true; }
		if (input->PushKey(DIK_UP)) { pos.y += moveSpeed, isMove = true; }
		if (input->PushKey(DIK_DOWN)) { pos.y -= moveSpeed, isMove = true; }

		//更新した座標をセット
		playerObject->SetPosition(pos);
	}

	//ゲームパッドでの移動
	if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false)
		|| Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
	{
		//移動し続けると加速する
		moveSpeed += 0.02f;

		//最高速度より速くはならない
		const float moveSpeedMax = 1.0f;
		if (moveSpeed > moveSpeedMax)
		{
			moveSpeed = moveSpeedMax;
		}

		//移動速度に左スティックの角度を乗算して360度動けるようにする
		XMFLOAT3 pos = playerObject->GetPosition();
		pos.x += moveSpeed * Xinput->GetPadLStickIncline().x;
		pos.y += moveSpeed * Xinput->GetPadLStickIncline().y;
		isMove = true;

		//更新した座標をセット
		playerObject->SetPosition(pos);

		//プレイヤーは移動している(左スティックを傾けた)方向を向く
		XMFLOAT3 rota = { 0, 0, 0 };
		rota.z = -Xinput->GetPadLStickAngle();

		//更新した角度をセット
		playerObject->SetRotation(rota);
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

void Player::ShockWaveStart()
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//毎フレーム発射しないのでfalseに戻しておく
	isShockWaveStart = false;

	//衝撃波発射
	if (input->TriggerKey(DIK_SPACE) || Xinput->TriggerButton(XInputManager::PAD_RB))
	{
		isShockWaveStart = true;
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
	if (pos.x <= -moveRange.x + size.x / 2)
	{
		pos.x = -moveRange.x + size.x / 2;
		isCollision = true;
	}
	else if (pos.x >= moveRange.x - size.x / 2)
	{
		pos.x = moveRange.x - size.x / 2;
		isCollision = true;
	}
	if (pos.y <= -moveRange.y + size.y / 2)
	{
		pos.y = -moveRange.y + size.y / 2;
		isCollision = true;
	}
	else if (pos.y >= moveRange.y - size.y / 2)
	{
		pos.y = moveRange.y - size.y / 2;
		isCollision = true;
	}

	//枠にライン当たっていたら
	if (isCollision)
	{
		//座標をセット
		playerObject->SetPosition(pos);
	}
}

