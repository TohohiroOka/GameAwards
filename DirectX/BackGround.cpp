#include "BackGround.h"
#include "SafeDelete.h"
#include "StageEffect.h"

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
	backGroundSprite->SetPosition({ 0, 0 });
	//テクスチャサイズをセット
	backGroundSprite->SetTexSize({ 1280, 720 });
	//大きさをセット
	backGroundSprite->SetSize({ 1280, 720 });

	return true;
}

void BackGround::Update()
{
	//スプライト更新
	backGroundSprite->Update();

	//回復フィールドエフェクト更新
	StageEffect::SetHealField({ 1.0f,-11.0f,0.0f });
}

void BackGround::Draw()
{
	//スプライト描画
	backGroundSprite->Draw();
}
