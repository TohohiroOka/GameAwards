#include "Player.h"
#include "Input.h"
#include "XInputManager.h"
#include "SafeDelete.h"

using namespace DirectX;

Player *Player::Create(Model *model)
{
	//インスタンスを生成
	Player *instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	XMFLOAT3 startPos = { 0 ,0 ,0 };
	XMFLOAT3 scale = { 3 ,3, 1 };
	if (!instance->Initialize(model, startPos, scale)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Player::~Player()
{
	safe_delete(playerObject);
}

bool Player::Initialize(Model *model, XMFLOAT3 position, XMFLOAT3 scale)
{
	//プレイヤーオブジェクト生成
	playerObject = Object3d::Create();
	if (playerObject == nullptr) {
		return false;
	}

	//初期地点と大きさをセット
	playerObject->SetPosition(position);
	playerObject->SetScale(scale);

	if (model) {
		playerObject->SetModel(model);
	}

	return true;
}

void Player::Update()
{
	//移動処理
	Move();

	//パッドスティックによる角度変更
	PadStickRotation();

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
}

void Player::Draw()
{
	//オブジェクト描画
	playerObject->Draw();
}

void Player::Damage()
{
	//HPを減らす
	HP--;

	//ダメージ状態にする
	isDamage = true;

	playerObject->SetColor({ 1,0,1,1 });
}

void Player::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void Player::Move()
{
	Input *input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//通常時の移動速度
	float moveSpeed = 1.0f;

	//特定のキーorボタンを押すと移動速度を遅くする
	if (input->PushKey(DIK_LSHIFT) || Xinput->PushButton(XInputManager::PAD_LB))
	{
		moveSpeed = 0.5f;
	}

	//デバック用キー移動
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		//移動処理
		XMFLOAT3 pos = playerObject->GetPosition();
		if (input->PushKey(DIK_LEFT)) { pos.x -= moveSpeed; }
		if (input->PushKey(DIK_RIGHT)) { pos.x += moveSpeed; }
		if (input->PushKey(DIK_UP)) { pos.y += moveSpeed; }
		if (input->PushKey(DIK_DOWN)) { pos.y -= moveSpeed; }

		//画面外に出ないようにする
		/*XMFLOAT2 windowSize = { 1280, 720 };
		XMFLOAT3 size = playerObject->GetScale();
		if (pos.x - size.x / 2 < 0) { pos.x = size.x / 2; }
		else if (pos.x + size.x / 2 > windowSize.x) { pos.x = windowSize.x - size.x / 2; }
		if (pos.y - size.y / 2 < 0) { pos.y = size.y / 2; }
		else if (pos.y + size.y / 2 > windowSize.y) { pos.y = windowSize.y - size.y / 2; }*/

		//更新した座標をセット
		playerObject->SetPosition(pos);
	}

	//ゲームパッドでの移動
	if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false)
		|| Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
	{
		//移動速度に左スティックの角度を乗算して360度動けるようにする
		XMFLOAT3 pos = playerObject->GetPosition();		
		pos.x += moveSpeed * Xinput->GetPadLStickIncline().x;
		pos.y += moveSpeed * Xinput->GetPadLStickIncline().y;

		//画面外に出ないようにする
		/*XMFLOAT2 windowSize = { 1280, 720 };
		XMFLOAT3 size = playerObject->GetScale();
		if (pos.x - size.x / 2 < 0) { pos.x = size.x / 2; }
		else if (pos.x + size.x / 2 > windowSize.x) { pos.x = windowSize.x - size.x / 2; }
		if (pos.y - size.y / 2 < 0) { pos.y = size.y / 2; }
		else if (pos.y + size.y / 2 > windowSize.y) { pos.y = windowSize.y - size.y / 2; }*/

		//更新した座標をセット
		playerObject->SetPosition(pos);
	}
}

void Player::PadStickRotation()
{
	XInputManager* Xinput = XInputManager::GetInstance();

	//パッドスティックを一定以上傾けると角度変更を開始する
	if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false)
		|| Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
	{
		//右スティックを傾けた角度に傾く
		XMFLOAT3 rota = { 0, 0, 0 };
		rota.z = -Xinput->GetPadLStickAngle();

		//更新した角度をセット
		playerObject->SetRotation(rota);
	}
}
