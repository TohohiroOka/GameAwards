#include "DeadEnemyPoint.h"
#include "SafeDelete.h"

DeadEnemyPoint *DeadEnemyPoint::Create(Model *model, XMFLOAT3 position, float radius)
{
	//インスタンスを生成
	DeadEnemyPoint *instance = new DeadEnemyPoint();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model, position, radius)) {
		delete instance;
		assert(0);
	}

	return instance;
}

DeadEnemyPoint::~DeadEnemyPoint()
{
	safe_delete(deadPointObject);
}

bool DeadEnemyPoint::Initialize(Model *model, XMFLOAT3 position, float radius)
{
	//死んだ敵の位置オブジェクト生成
	deadPointObject = Object3d::Create();
	if (deadPointObject == nullptr) {
		return false;
	}

	//初期地点と円の半径をセット
	deadPointObject->SetPosition(position);
	//deadPointObject->SetRotation({ 0, 0, 180 });
	this->radius = radius;
	deadPointObject->SetScale({ radius, radius, 1 });

	if (model) {
		deadPointObject->SetModel(model);
	}

	return true;
}

void DeadEnemyPoint::Update()
{
	//オブジェクト更新
	deadPointObject->Update();
}

void DeadEnemyPoint::Draw()
{
	//オブジェクト描画
	deadPointObject->Draw();
}
