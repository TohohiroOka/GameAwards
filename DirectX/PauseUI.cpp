#include "PauseUI.h"
#include "SafeDelete.h"
#include "Input.h"
#include "XInputManager.h"
#include "Audio.h"

PauseUI* PauseUI::Create(int plainTexNum, int pauseTexNum, int backGameTexNum, int backTitleTexNum, int AButtonTexNum)
{
	//インスタンスを生成
	PauseUI* instance = new PauseUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(plainTexNum, pauseTexNum, backGameTexNum, backTitleTexNum, AButtonTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PauseUI::~PauseUI()
{
	safe_delete(backSprite);
	safe_delete(pauseSprite);
	safe_delete(backGameSprite);
	safe_delete(backTitleSprite);
	safe_delete(AButtonSprite);
}

bool PauseUI::Initialize(int plainTexNum, int pauseTexNum, int backGameTexNum, int backTitleTexNum, int AButtonTexNum)
{
	//背景スプライト生成
	backSprite = Sprite::Create(plainTexNum, { 0, 0 });
	if (backSprite == nullptr) {
		return false;
	}
	//座標をセット
	backSprite->SetPosition({ 0, 0 });
	//テクスチャサイズをセット
	backSprite->SetTexSize({ 1, 1 });
	//大きさをセット
	backSprite->SetSize({ 1280, 720 });
	//色をセット
	backSprite->SetColor({ 0, 0, 0, 0.6f });
	//スプライト更新
	backSprite->Update();


	//ポーズスプライト生成
	pauseSprite = Sprite::Create(pauseTexNum);
	if (pauseSprite == nullptr) {
		return false;
	}
	//座標をセット
	pauseSprite->SetPosition({ 640, 180 });
	//テクスチャサイズをセット
	pauseSprite->SetTexSize({ 233, 63 });
	//大きさをセット
	pauseSprite->SetSize({ 350, 95 });
	//スプライト更新
	pauseSprite->Update();


	//ゲームに戻るシーン選択スプライト生成
	backGameSprite = Sprite::Create(backGameTexNum);
	if (backGameSprite == nullptr) {
		return false;
	}
	//座標をセット
	backGameSprite->SetPosition({ 470, 500 });
	//テクスチャサイズをセット
	backGameSprite->SetTexSize({ 194, 63 });
	//大きさをセット
	backGameSprite->SetSize({ 194, 63 });
	//スプライト更新
	backGameSprite->Update();

	//タイトルに戻るシーン選択スプライト生成
	backTitleSprite = Sprite::Create(backTitleTexNum);
	if (backTitleSprite == nullptr) {
		return false;
	}
	//座標をセット
	backTitleSprite->SetPosition({ 810, 500 });
	//テクスチャサイズをセット
	backTitleSprite->SetTexSize({ 207, 63 });
	//大きさをセット
	backTitleSprite->SetSize({ 207, 63 });
	//スプライト更新
	backTitleSprite->Update();


	//Aボタンスプライト生成
	AButtonSprite = Sprite::Create(AButtonTexNum);
	if (AButtonSprite == nullptr) {
		return false;
	}
	//座標をセット
	XMFLOAT2 AButtonSpritePos = backGameSprite->GetPosition();
	AButtonSpritePos.y += 70;
	AButtonSprite->SetPosition(AButtonSpritePos);
	//テクスチャサイズをセット
	AButtonSprite->SetTexSize({ 32, 32 });
	//大きさをセット
	AButtonSprite->SetSize({ 32, 32 });
	//スプライト更新
	AButtonSprite->Update();

	//サウンドの読み込み
	Audio* audio = Audio::GetInstance();
	sound[0] = audio->SoundLoadWave("Resources/sound/select.wav");//カーソル移動音

	return true;
}

void PauseUI::Update()
{
	//ゲームに戻るかタイトルシーンに戻るか選択
	SelectBack();

	//スプライト更新
	backSprite->Update();
	pauseSprite->Update();
	backGameSprite->Update();
	backTitleSprite->Update();
	AButtonSprite->Update();
}

void PauseUI::Draw()
{
	//スプライト描画
	backSprite->Draw();
	pauseSprite->Draw();
	backGameSprite->Draw();
	backTitleSprite->Draw();
	AButtonSprite->Draw();
}

void PauseUI::Reset()
{
	//ゲームに戻るか
	isBackGame = true;
	//選択したか
	isSelect = false;

	//スプライト初期化
	backGameSprite->SetColor({ 1, 0, 0, 1 });
	backGameSprite->Update();
	backTitleSprite->SetColor({ 1, 1, 1, 1 });
	backTitleSprite->Update();
	XMFLOAT2 AButtonSpritePos = backGameSprite->GetPosition();
	AButtonSpritePos.y += 70;
	AButtonSprite->SetPosition(AButtonSpritePos);
	AButtonSprite->Update();
}

void PauseUI::SelectBack()
{
	//確定していたら抜ける
	if (isSelect) { return; }

	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();
	Audio* audio = Audio::GetInstance();

	//ゲームに戻るを選択しているとき
	if (isBackGame)
	{
		//右方向にスティックを倒すと
		if (input->TriggerKey(DIK_RIGHT) || Xinput->TriggerLeftStickX(false))
		{
			//サウンドの再生
			audio->SoundPlayWava(sound[0], false);
			//タイトルシーンに戻る状態に変更
			isBackGame = false;

			//選択に合わせて色を変更
			backGameSprite->SetColor({ 1, 1, 1, 1 });
			backTitleSprite->SetColor({ 1, 0, 0, 1 });

			//Aボタンを選択中の下にセット
			XMFLOAT2 AButtonSpritePos = backTitleSprite->GetPosition();
			AButtonSpritePos.y += 70;
			AButtonSprite->SetPosition(AButtonSpritePos);
			AButtonSprite->Update();
		}
	}
	//タイトルシーンに戻るを選択しているとき
	else
	{
		//左方向にスティックを倒すと
		if (input->TriggerKey(DIK_LEFT) || Xinput->TriggerLeftStickX(true))
		{
			//サウンドの再生
			audio->SoundPlayWava(sound[0], false);
			//ゲームに戻る状態に変更
			isBackGame = true;

			//選択に合わせて色を変更
			backGameSprite->SetColor({ 1, 0, 0, 1 });
			backTitleSprite->SetColor({ 1, 1, 1, 1 });

			//Aボタンを選択中の下にセット
			XMFLOAT2 AButtonSpritePos = backGameSprite->GetPosition();
			AButtonSpritePos.y += 70;
			AButtonSprite->SetPosition(AButtonSpritePos);
			AButtonSprite->Update();
		}
	}
}
