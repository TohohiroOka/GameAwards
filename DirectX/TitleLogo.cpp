#include "TitleLogo.h"
#include "SafeDelete.h"
#include "Easing.h"

TitleLogo *TitleLogo::Create(int textureNum)
{
	//インスタンスを生成
	TitleLogo *instance = new TitleLogo();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(textureNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TitleLogo::~TitleLogo()
{
	safe_delete(titleSprite);
}

bool TitleLogo::Initialize(int textureNum)
{
	//タイトルロゴスプライト生成
	titleSprite = Sprite::Create(textureNum);
	if (titleSprite == nullptr) {
		return false;
	}

	//初期座標をセット
	titleSprite->SetPosition({ -1000, -1000 });
	titleSprite->SetSize({ 600, 120 });
	titleSprite->SetTexSize({ 64, 64 });

	return true;
}

void TitleLogo::Update()
{
	//落下中
	if (isFall)
	{
		Fall();
	}

	//スプライト更新
	titleSprite->Update();
}

void TitleLogo::Draw()
{
	//スプライト描画
	titleSprite->Draw();
}

void TitleLogo::FallStart(XMFLOAT2 fallStartPosition, XMFLOAT2 fallEndPosition)
{
	//落下開始座標をセット
	this->fallStartPosition = fallStartPosition;
	//落下地点をセット
	this->fallEndPosition = fallEndPosition;
	//落下中状態にする
	isFall = true;
}

void TitleLogo::Fall()
{
	//落下を行う時間
	const int fallTime = 200;

	//落下タイマー更新
	fallTimer++;

	//イージング計算用の時間
	float easeTimer = (float)fallTimer / fallTime;
	//落下時の画面外からの座標移動
	XMFLOAT2 pos = {};
	pos.x = Easing::InQuint(fallStartPosition.x, fallEndPosition.x, easeTimer);
	pos.y = Easing::InQuint(fallStartPosition.y, fallEndPosition.y, easeTimer);
	//更新したアルファ値をセット
	titleSprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (fallTimer >= fallTime)
	{
		//落下終了
		isFall = false;
	}
}
