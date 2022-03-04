#include "Zakorin.h"

Zakorin *Zakorin::Create(XMFLOAT2 position)
{
	//インスタンスを生成
	Zakorin *instance = new Zakorin();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Zakorin::Initialize(XMFLOAT2 position)
{
	//ザコリンスプライト生成
	enemySprite = Sprite::Create(1);
	if (enemySprite == nullptr) {
		return false;
	}

	//初期座標セット
	enemySprite->SetPosition(position);
	//大きさをセット
	enemySprite->SetSize({ 20.0f, 20.0f });

	return true;
}

void Zakorin::Update()
{
	//生存中の敵のみ更新
	if (isAlive) {

		XMFLOAT2 pos = enemySprite->GetPosition();
		pos.x++;
		pos.y++;
		enemySprite->SetPosition(pos);
	}
	//スプライト更新
	enemySprite->Update();
}
