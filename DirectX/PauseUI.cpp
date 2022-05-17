#include "PauseUI.h"
#include "SafeDelete.h"
#include "Input.h"
#include "XInputManager.h"

PauseUI* PauseUI::Create(int plainTexNum, int pauseTexNum, int backGameTexNum, int backTitleTexNum)
{
	//インスタンスを生成
	PauseUI* instance = new PauseUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(plainTexNum, pauseTexNum, backGameTexNum, backTitleTexNum)) {
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
}

bool PauseUI::Initialize(int plainTexNum, int pauseTexNum, int backGameTexNum, int backTitleTexNum)
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
	backSprite->SetColor({ 1, 1, 1, 0.4f });
	//スプライト更新
	backSprite->Update();


	//ポーズスプライト生成
	pauseSprite = Sprite::Create(pauseTexNum);
	if (pauseSprite == nullptr) {
		return false;
	}
	//座標をセット
	pauseSprite->SetPosition({ 640, 200 });
	//テクスチャサイズをセット
	pauseSprite->SetTexSize({ 233, 63 });
	//大きさをセット
	pauseSprite->SetSize({ 233, 63 });
	//スプライト更新
	pauseSprite->Update();


	//ゲームに戻るシーン選択スプライト生成
	backGameSprite = Sprite::Create(backGameTexNum);
	if (backGameSprite == nullptr) {
		return false;
	}
	//座標をセット
	backGameSprite->SetPosition({ 400, 600 });
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
	backTitleSprite->SetPosition({ 880, 600 });
	//テクスチャサイズをセット
	backTitleSprite->SetTexSize({ 207, 63 });
	//大きさをセット
	backTitleSprite->SetSize({ 207, 63 });
	//スプライト更新
	backTitleSprite->Update();

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
}

void PauseUI::Draw()
{
	//スプライト描画
	backSprite->Draw();
	pauseSprite->Draw();
	backGameSprite->Draw();
	backTitleSprite->Draw();
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
}

void PauseUI::SelectBack()
{
	//確定していたら抜ける
	if (isSelect) { return; }

	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//ゲームに戻るを選択しているとき
	if (isBackGame)
	{
		//右方向にスティックを倒すと
		if (input->TriggerKey(DIK_RIGHT) || Xinput->TriggerLeftStickX(false))
		{
			//タイトルシーンに戻る状態に変更
			isBackGame = false;

			backGameSprite->SetColor({ 1, 1, 1, 1 });
			backTitleSprite->SetColor({ 1, 0, 0, 1 });
		}
	}
	//タイトルシーンに戻るを選択しているとき
	else
	{
		//左方向にスティックを倒すと
		if (input->TriggerKey(DIK_LEFT) || Xinput->TriggerLeftStickX(true))
		{
			//ゲームに戻る状態に変更
			isBackGame = true;

			backGameSprite->SetColor({ 1, 0, 0, 1 });
			backTitleSprite->SetColor({ 1, 1, 1, 1 });
		}
	}
}
