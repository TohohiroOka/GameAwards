#include "Player.h"
#include "Input.h"
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
	XMFLOAT3 scale = { 10 ,10, 1 };
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

	//オブジェクト更新
	playerObject->Update();
}

void Player::Draw()
{
	//オブジェクト描画
	playerObject->Draw();
}

void Player::Move()
{
	Input *input = Input::GetInstance();

	//デバック用キー移動
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		float moveSpeed = 2.0f;

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
	float lStickJudgeNum = 250;
	if (input->TiltGamePadLStickX(lStickJudgeNum) || input->TiltGamePadLStickX(-lStickJudgeNum)
		|| input->TiltGamePadLStickY(lStickJudgeNum) || input->TiltGamePadLStickY(-lStickJudgeNum))
	{
		float moveSpeed = 2.0f;
		XMFLOAT3 pos = playerObject->GetPosition();

		//移動速度に左スティックの角度を乗算して360度動けるようにする
		pos.x += moveSpeed * input->GetPadLStickIncline().x;
		pos.y -= moveSpeed * input->GetPadLStickIncline().y;

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
	Input *input = Input::GetInstance();

	//パッドスティックを一定以上傾けると角度変更を開始する
	float rStickJudgeNum = 500;
	if (input->TiltGamePadRStickX(rStickJudgeNum) || input->TiltGamePadRStickX(-rStickJudgeNum)
		|| input->TiltGamePadRStickY(rStickJudgeNum) || input->TiltGamePadRStickY(-rStickJudgeNum))
	{
		//右スティックを傾けた角度に傾く
		XMFLOAT3 rota = { 0, 0, 0 };
		rota.z = -input->GetPadRStickAngle();

		//更新した角度をセット
		playerObject->SetRotation(rota);
	}
}
