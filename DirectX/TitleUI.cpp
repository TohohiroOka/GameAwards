#include "TitleUI.h"
#include "SafeDelete.h"

TitleUI* TitleUI::Create(Model* RBModel)
{
	//インスタンスを生成
	TitleUI* instance = new TitleUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(RBModel)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TitleUI::~TitleUI()
{
	safe_delete(RBObject);
}

bool TitleUI::Initialize(Model* RBModel)
{
	//RBオブジェクト生成
	RBObject = Object3d::Create(RBModel);
	if (RBObject == nullptr) {
		return false;
	}
	//初期座標をセット
	RBObject->SetPosition({ 0, 0, 0 });
	RBObject->SetScale({ 5, 5, 1 });

	return true;
}

void TitleUI::Update(XMFLOAT3 playerPos)
{
	//描画しない場合は抜ける
	if (!isDraw) { return; }

	//RBオブジェクトをプレイヤーに追従させる
	XMFLOAT3 RBpos = playerPos;
	RBpos.y -= 15;
	RBObject->SetPosition(RBpos);

	//オブジェクト更新
	RBObject->Update();
}

void TitleUI::Draw()
{
	//描画しない場合は抜ける
	if (!isDraw) { return; }

	//オブジェクト描画
	RBObject->Draw();
}
