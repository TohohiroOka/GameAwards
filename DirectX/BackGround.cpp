#include "BackGround.h"
#include "SafeDelete.h"

BackGround* BackGround::Create(int backGroundTexNum)
{
	//インスタンスを生成
	BackGround* instance = new BackGround();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(backGroundTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

BackGround::~BackGround()
{
	safe_delete(backGroundSprite);
}

bool BackGround::Initialize(int backGroundTexNum)
{
	//背景スプライト生成
	backGroundSprite = Sprite::Create(backGroundTexNum, { 0, 0 });
	if (backGroundSprite == nullptr) {
		return false;
	}
	//座標をセット
	backGroundSprite->SetPosition({ 0, 120 });
	//テクスチャサイズをセット
	backGroundSprite->SetTexSize({ 1280, 720 });
	//大きさをセット
	backGroundSprite->SetSize({ 1280, 600 });

	return true;
}

void BackGround::Update()
{
	//スプライト更新
	backGroundSprite->Update();
}

void BackGround::Draw()
{
	//スプライト描画
	backGroundSprite->Draw();
}
