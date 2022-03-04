#include "Player.h"
#include "Input.h"
#include "SafeDelete.h"

using namespace DirectX;

Player *Player::Create(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size)
{
	//インスタンスを生成
	Player *instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(texNumber, position, size)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Player::~Player()
{
	safe_delete(playerSprite);
}

bool Player::Initialize(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size)
{
	//プレイヤースプライト生成
	playerSprite = Sprite::Create(texNumber);
	if (playerSprite == nullptr) {
		return false;
	}

	//初期地点と大きさをセット
	playerSprite->SetPosition(position);
	playerSprite->SetSize(size);

	return true;
}

void Player::Update()
{
	Input *input = Input::GetInstance();

	//デバック用キー移動
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		float moveSpeed = 5.0f;

		XMFLOAT2 pos = playerSprite->GetPosition();
		if (input->PushKey(DIK_LEFT)) pos.x -= moveSpeed;
		if (input->PushKey(DIK_RIGHT)) pos.x += moveSpeed;
		if (input->PushKey(DIK_UP)) pos.y -= moveSpeed;
		if (input->PushKey(DIK_DOWN)) pos.y += moveSpeed;

		//画面外に出ないようにする
		XMFLOAT2 windowSize = { 1280, 720 };
		XMFLOAT2 size = playerSprite->GetSize();
		if (pos.x - size.x / 2 < 0) { pos.x = size.x / 2; }
		else if (pos.x + size.x / 2 > windowSize.x) { pos.x = windowSize.x - size.x / 2; }
		if (pos.y - size.y / 2 < 0) { pos.y = size.y / 2; }
		else if (pos.y + size.y / 2 > windowSize.y) { pos.y = windowSize.y - size.y / 2; }

		//更新した座標をセット
		playerSprite->SetPosition(pos);
	}

	//ゲームパッドでの移動
	float lStickJudgeNum = 250;
	if (input->TiltGamePadLStickX(lStickJudgeNum) || input->TiltGamePadLStickX(-lStickJudgeNum)
		|| input->TiltGamePadLStickY(lStickJudgeNum) || input->TiltGamePadLStickY(-lStickJudgeNum))
	{
		float moveSpeed = 5.0f;
		XMFLOAT2 pos = playerSprite->GetPosition();

		//移動速度に左スティックの角度を乗算して360度動けるようにする
		pos.x += moveSpeed * input->GetPadLStickIncline().x;
		pos.y += moveSpeed * input->GetPadLStickIncline().y;


		//画面外に出ないようにする
		XMFLOAT2 windowSize = { 1280, 720 };
		XMFLOAT2 size = playerSprite->GetSize();
		if (pos.x - size.x / 2 < 0) { pos.x = size.x / 2; }
		else if (pos.x + size.x / 2 > windowSize.x) { pos.x = windowSize.x - size.x / 2; }
		if (pos.y - size.y / 2 < 0) { pos.y = size.y / 2; }
		else if (pos.y + size.y / 2 > windowSize.y) { pos.y = windowSize.y - size.y / 2; }

		//更新した座標をセット
		playerSprite->SetPosition(pos);
	}



	//弾発射角度変更
	float rStickJudgeNum = 500;
	if (input->TiltGamePadRStickX(rStickJudgeNum) || input->TiltGamePadRStickX(-rStickJudgeNum)
		|| input->TiltGamePadRStickY(rStickJudgeNum) || input->TiltGamePadRStickY(-rStickJudgeNum))
	{
		//右スティックを傾けた角度に弾発射角度も傾く
		float rota = input->GetPadRStickAngle();

		//更新した角度をセット
		playerSprite->SetRotation(rota);
	}

	//スプライト更新
	playerSprite->Update();
}

void Player::Draw()
{
	//スプライト描画
	playerSprite->Draw();
}
