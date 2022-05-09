#include "UIFrame.h"
#include "SafeDelete.h"

UIFrame* UIFrame::Create(int UIFrameTexNum)
{
	//インスタンスを生成
	UIFrame* instance = new UIFrame();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(UIFrameTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

UIFrame::~UIFrame()
{
	safe_delete(frameSprite);
}

bool UIFrame::Initialize(int UIFrameTexNum)
{
	//枠スプライト生成
	frameSprite = Sprite::Create(UIFrameTexNum, { 0, 0 });
	if (frameSprite == nullptr) {
		return false;
	}
	//座標をセット
	frameSprite->SetPosition({ 0, 0 });
	//テクスチャサイズをセット
	frameSprite->SetTexSize({ 1280, 100 });
	//大きさをセット
	frameSprite->SetSize({ 1280, 120 });

	return true;
}

void UIFrame::Update()
{
	//スプライト更新
	frameSprite->Update();
}

void UIFrame::Draw()
{
	//スプライト描画
	frameSprite->Draw();
}
