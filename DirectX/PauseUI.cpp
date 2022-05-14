#include "PauseUI.h"
#include "SafeDelete.h"
#include "Input.h"
#include "XInputManager.h"

PauseUI* PauseUI::Create(int plainTexNum, int pauseTexNum, int selectBackTexNum)
{
	//インスタンスを生成
	PauseUI* instance = new PauseUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(plainTexNum, pauseTexNum, selectBackTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PauseUI::~PauseUI()
{
	safe_delete(backSprite);
	safe_delete(pauseSprite);
	safe_delete(selectBackSprite);
}

bool PauseUI::Initialize(int plainTexNum, int pauseTexNum, int selectBackTexNum)
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
	pauseSprite->SetTexSize({ 700, 200 });
	//大きさをセット
	pauseSprite->SetSize({ 350, 100 });
	//スプライト更新
	pauseSprite->Update();


	//戻るシーン選択スプライト生成
	selectBackSprite = Sprite::Create(selectBackTexNum);
	if (selectBackSprite == nullptr) {
		return false;
	}
	//座標をセット
	selectBackSprite->SetPosition({ 640, 600 });
	//テクスチャサイズをセット
	selectBackSprite->SetTexSize({ 700, 200 });
	//大きさをセット
	selectBackSprite->SetSize({ 350, 100 });
	//スプライト更新
	selectBackSprite->Update();

	return true;
}

void PauseUI::Update()
{
	//ゲームに戻るかタイトルシーンに戻るか選択
	SelectBack();

	//スプライト更新
	backSprite->Update();
	pauseSprite->Update();
	selectBackSprite->Update();
}

void PauseUI::Draw()
{
	//スプライト描画
	backSprite->Draw();
	pauseSprite->Draw();
	selectBackSprite->Draw();
}

void PauseUI::Reset()
{
	//ゲームに戻るか
	isBackGame = true;
	//選択したか
	isSelect = false;

	//スプライト初期化
	selectBackSprite->SetTexLeftTop({ 0, 0 });
	selectBackSprite->Update();
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

			float texsizeX = selectBackSprite->GetTexSize().x;
			selectBackSprite->SetTexLeftTop({ texsizeX, 0 });
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

			
			selectBackSprite->SetTexLeftTop({ 0, 0 });
		}
	}
}
