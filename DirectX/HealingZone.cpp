#include "HealingZone.h"
#include "SafeDelete.h"

HealingZone* HealingZone::Create(Model* model)
{
	//インスタンスを生成
	HealingZone* instance = new HealingZone();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

HealingZone::~HealingZone()
{
	safe_delete(healingZoneObject);
}

bool HealingZone::Initialize(Model* model)
{
	//衝撃波オブジェクト生成
	healingZoneObject = Object3d::Create();
	if (healingZoneObject == nullptr) {
		return false;
	}
	//モデルをセット
	if (model) {
		healingZoneObject->SetModel(model);
	}
	//座標をセット
	healingZoneObject->SetPosition({ 0, -10, 0 });
	//大きさをセット
	healingZoneObject->SetScale({ 30.0f, 30.0f, 1.0f });
	//色をセット
	healingZoneObject->SetColor({ 0.7f, 0.7f, 0.7f, 1 });

	return true;
}

void HealingZone::Update()
{
	//オブジェクト更新
	healingZoneObject->Update();
}

void HealingZone::Draw()
{
	//オブジェクト描画
	healingZoneObject->Draw();
}
