#include "Player.h"
#include "Input.h"
#include "Easing.h"
#include "XInputManager.h"
#include "SafeDelete.h"
#include "StageEffect.h"

using namespace DirectX;

Model *Player::weaponHP1Model = nullptr;
Model *Player::weaponHP2Model = nullptr;
Model *Player::weaponHP3Model = nullptr;
XMFLOAT2 Player::moveRange = { 95, 50 };

Player *Player::Create(Model *playerModel)
{
	//インスタンスを生成
	Player *instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	XMFLOAT3 startPos = { 0 ,0 ,0 };
	XMFLOAT3 scale = { 2 ,2, 1 };
	if (!instance->Initialize(playerModel, startPos, scale)) {
		delete instance;
		assert(0);
	}

	return instance;
}

void Player::SetWeaponModel(Model *weaponHP1Model, Model *weaponHP2Model, Model *weaponHP3Model)
{
	Player::weaponHP1Model = weaponHP1Model;
	Player::weaponHP2Model = weaponHP2Model;
	Player::weaponHP3Model = weaponHP3Model;
}

Player::~Player()
{
	safe_delete(playerObject);
	safe_delete(weaponObject);
}

bool Player::Initialize(Model *playerModel, XMFLOAT3 position, XMFLOAT3 scale)
{
	//プレイヤーオブジェクト生成
	playerObject = Object3d::Create();
	if (playerObject == nullptr) {
		return false;
	}

	//初期地点と大きさをセット
	playerObject->SetPosition(position);
	playerObject->SetScale(scale);

	//プレイヤーのモデルをセット
	if (playerModel) {
		playerObject->SetModel(playerModel);
	}

	//ブルームをかける
	playerObject->SetBloom(true);

	//ウエポンオブジェクト生成
	weaponObject = Object3d::Create();
	if (weaponObject == nullptr) {
		return false;
	}
	//初期地点と大きさをセット
	weaponObject->SetPosition(position);
	weaponObject->SetScale(scale);

	//ウエポンのモデルをセット
	if (weaponHP3Model) {
		weaponObject->SetModel(weaponHP3Model);
	}

	//ブルームをかける
	weaponObject->SetBloom(true);

	return true;
}

void Player::Update()
{
	//停止状態以外の場合動ける
	if (!isStop)
	{
		//初期位置に戻る処理
		if (isResetPos)
		{
			ResetPosition();
		}
		//ノックバック処理
		else if (isKnockback)
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
			//パッドスティックによる角度変更
			PadStickRotation();
		}
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
	weaponObject->Update();
}

void Player::Draw()
{
	//オブジェクト描画
	playerObject->Draw();
	weaponObject->Draw();
}

void Player::Damage()
{
	//HPを減らす
	HP--;

	//HPが減ったため、モデルをHPに対応するモデルに変更
	if (HP == 2) { weaponObject->SetModel(weaponHP2Model); }
	else if (HP == 1) { weaponObject->SetModel(weaponHP1Model); }
	else if (HP == 0) { weaponObject->SetModel(nullptr); }

	//ダメージ状態にする
	isDamage = true;

	//色を変更する
	playerObject->SetColor({ 1,0,1,1 });
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

void Player::Dead()
{
	//死亡状態にする
	isAlive = false;
}

void Player::SetResetPosition()
{
	//ノックバック状態の場合終わらせる
	isKnockback = false;
	//初期位置に戻す前の位置(現在の位置)
	beforeResetPos = playerObject->GetPosition();
	//初期位置に戻す前の角度(現在の角度)
	beforeResetRota = playerObject->GetRotation();
	//初期位置に戻す前のウエポンの角度(現在のウエポンの角度)
	beforeResetWeaponRota = weaponObject->GetRotation();
	//初期位置に戻すタイマーを初期化
	resetPosTimer = 0;
	//初期位置に戻す処理を開始する
	isResetPos = true;
}

bool Player::Move()
{
	Input *input = Input::GetInstance();
	XInputManager *Xinput = XInputManager::GetInstance();

	//移動したらtrue
	bool isMove = false;

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
		if (input->PushKey(DIK_LEFT)) { pos.x -= moveSpeed, isMove = true; }
		if (input->PushKey(DIK_RIGHT)) { pos.x += moveSpeed, isMove = true; }
		if (input->PushKey(DIK_UP)) { pos.y += moveSpeed, isMove = true; }
		if (input->PushKey(DIK_DOWN)) { pos.y -= moveSpeed, isMove = true; }

		//画面外に出ないようにする
		XMFLOAT3 size = playerObject->GetScale();
		if (pos.x <= -moveRange.x - size.x / 2) { pos.x = -moveRange.x - size.x / 2; }
		else if (pos.x >= moveRange.x + size.x / 2) { pos.x = moveRange.x + size.x / 2; }
		if (pos.y <= -moveRange.y - size.y / 2) { pos.y = -moveRange.y - size.y / 2; }
		else if (pos.y >= moveRange.y + size.y / 2) { pos.y = moveRange.y + size.y / 2; }

		//更新した座標をセット
		playerObject->SetPosition(pos);
		weaponObject->SetPosition(pos);
	}

	//ゲームパッドでの移動
	if (Xinput->LeftStickX(true) || Xinput->LeftStickX(false)
		|| Xinput->LeftStickY(true) || Xinput->LeftStickY(false))
	{
		//移動速度に左スティックの角度を乗算して360度動けるようにする
		XMFLOAT3 pos = playerObject->GetPosition();
		pos.x += moveSpeed * Xinput->GetPadLStickIncline().x;
		pos.y += moveSpeed * Xinput->GetPadLStickIncline().y;
		isMove = true;

		//画面外に出ないようにする
		XMFLOAT3 size = playerObject->GetScale();
		if (pos.x <= -moveRange.x - size.x / 2) { pos.x = -moveRange.x - size.x / 2; }
		else if (pos.x >= moveRange.x + size.x / 2) { pos.x = moveRange.x + size.x / 2; }
		if (pos.y <= -moveRange.y - size.y / 2) { pos.y = -moveRange.y - size.y / 2; }
		else if (pos.y >= moveRange.y + size.y / 2) { pos.y = moveRange.y + size.y / 2; }

		//更新した座標をセット
		playerObject->SetPosition(pos);
		weaponObject->SetPosition(pos);

		//プレイヤーは移動している(左スティックを傾けた)方向を向く
		XMFLOAT3 rota = { 0, 0, 0 };
		rota.z = -Xinput->GetPadLStickAngle();

		//更新した角度をセット
		playerObject->SetRotation(rota);
	}

	return isMove;
}

void Player::PadStickRotation()
{
	XInputManager* Xinput = XInputManager::GetInstance();

	//パッドスティックを一定以上傾けると角度変更を開始する
	if (Xinput->RightStickX(true) || Xinput->RightStickX(false) ||
		Xinput->RightStickY(true) || Xinput->RightStickY(false))
	{
		//現在の角度
		rotaMin = weaponObject->GetRotation();

		//傾く角度
		rotaMax = -Xinput->GetPadRStickAngle();

		//差が-180度以下なら反対方向に回らないよう修正する
		if (rotaMax - rotaMin.z < -240) { rotaMin.z -= 360; }
		//差が180度以上なら反対方向に回らないよう修正する
		else if (rotaMax - rotaMin.z > 240) { rotaMin.z += 360; }

		weaponLarpTime = 1;
	}

	const int lerpMax = 4;
	if (lerpMax > weaponLarpTime)
	{
		//lerp処理
		float nowRota = Easing::Lerp(rotaMin.z, rotaMax, (float)weaponLarpTime / lerpMax);

		if (nowRota > -90) { rotaMin.z -= 360; } else if (nowRota <= -450) { rotaMin.z += 360; }

		//時間を進める
		weaponLarpTime++;

		//更新した角度をセット
		weaponObject->SetRotation({ rotaMin.x,rotaMin.y,nowRota });
	}
}

void Player::Knockback()
{
	XInputManager *Xinput = XInputManager::GetInstance();

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
	weaponObject->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (knockBackTimer >= knockBackTime)
	{
		//ノックバックを止める
		isKnockback = false;
	}
}

void Player::ResetPosition()
{
	//初期位置に戻す時間
	const int resetPosTime = 40;

	//初期位置に戻すタイマー更新
	resetPosTimer++;

	//初期位置
	XMFLOAT3 initPos = { 0, 0, 0 };
	XMFLOAT3 initRota = { 0, 0, 0 };

	//イージング計算用の時間
	float easeTimer = (float)resetPosTimer / resetPosTime;

	//イージングで動かす座標
	XMFLOAT3 pos = {};
	//イージングで動かす角度
	XMFLOAT3 rota = {};
	XMFLOAT3 weaponRota = {};
	//イージングで動かす
	pos.x = Easing::OutCubic(beforeResetPos.x, initPos.x, easeTimer);
	pos.y = Easing::OutCubic(beforeResetPos.y, initPos.y, easeTimer);
	rota.z = Easing::OutCubic(beforeResetRota.z, initRota.z, easeTimer);
	weaponRota.z = Easing::OutCubic(beforeResetWeaponRota.z, initRota.z, easeTimer);

	//更新した座標をセット
	playerObject->SetPosition(pos);
	weaponObject->SetPosition(pos);
	//更新した角度をセット
	playerObject->SetRotation(rota);
	weaponObject->SetRotation(weaponRota);

	//タイマーが指定した時間になったら
	if (resetPosTimer >= resetPosTime)
	{
		//初期位置に戻す処理を止める
		isResetPos = false;
	}
}
