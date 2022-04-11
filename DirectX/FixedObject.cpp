#include "FixedObject.h"
#include "SafeDelete.h"

FixedObject *FixedObject::Create(Model *model, XMFLOAT3 position)
{
	//インスタンスを生成
	FixedObject *instance = new FixedObject();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

FixedObject::~FixedObject()
{
	safe_delete(fixedObject);
}

bool FixedObject::Initialize(Model *model, XMFLOAT3 position)
{
	//固定オブジェクト生成
	fixedObject = Object3d::Create();
	if (fixedObject == nullptr) {
		return false;
	}

	//座標をセット 敵の中心座標と同じ
	fixedObject->SetPosition(position);

	//大きさをセット
	fixedObject->SetScale({ 2, 2, 1 });

	if (model) {
		fixedObject->SetModel(model);
	}

	return true;
}

void FixedObject::Update()
{
	//固定オブジェクトを更新
	fixedObject->Update();
}

void FixedObject::Draw()
{
	//固定オブジェクトを描画
	fixedObject->Draw();
}

void FixedObject::SetDelete()
{
	//削除する
	isDelete = true;
}
