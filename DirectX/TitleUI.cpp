#include "TitleUI.h"
#include "SafeDelete.h"

TitleUI* TitleUI::Create(Model* RBModel, Model* arrowModel)
{
	//インスタンスを生成
	TitleUI* instance = new TitleUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(RBModel, arrowModel)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TitleUI::~TitleUI()
{
	safe_delete(RBObject);
	safe_delete(arrowObject);
}

bool TitleUI::Initialize(Model* RBModel, Model* arrowModel)
{
	//RBオブジェクト生成
	RBObject = Object3d::Create(RBModel);
	if (RBObject == nullptr) {
		return false;
	}
	//初期座標をセット
	RBObject->SetPosition({ 0, 0, 0 });
	RBObject->SetScale({ 2, 2, 1 });

	//矢印オブジェクト生成
	arrowObject = Object3d::Create(arrowModel);
	if (arrowObject == nullptr) {
		return false;
	}

	//初期座標をセット
	arrowObject->SetPosition({ -10, -10, 0 });
	arrowObject->SetScale({ 2, 2, 1 });

	return true;
}

void TitleUI::Update(XMFLOAT3 playerPos)
{
	//RBオブジェクトをプレイヤーに追従させる
	XMFLOAT3 RBpos = playerPos;
	RBpos.y -= 10;
	RBObject->SetPosition(RBpos);


	//オブジェクト更新
	RBObject->Update();
	arrowObject->Update();
}

void TitleUI::Draw()
{
	//オブジェクト描画
	RBObject->Draw();
	arrowObject->Draw();
}
