#include "LaserSite.h"
#include "SafeDelete.h"

LaserSite* LaserSite::Create(Model* model)
{
	//インスタンスを生成
	LaserSite* instance = new LaserSite();
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

LaserSite::~LaserSite()
{
	safe_delete(razorObject);
}

bool LaserSite::Initialize(Model* model)
{
	//オブジェクト生成
	razorObject = Object3d::Create(model);
	if (razorObject == nullptr) {
		return false;
	}

	//大きさをセット
	razorObject->SetScale({ 2, 2, 1 });

	return true;
}

void LaserSite::Update()
{
	//オブジェクトを更新
	razorObject->Update();
}

void LaserSite::Draw()
{
	//オブジェクトを描画
	razorObject->Draw();
}

void LaserSite::SetPosition(XMFLOAT3 weaponPosition, XMFLOAT3 weaponRotation)
{
	//レーザーサイトはウエポンオブジェクトの角度を追従する
	XMFLOAT3 rota = weaponRotation;
	razorObject->SetRotation(rota);

	float angle = DirectX::XMConvertToRadians(rota.z + 90);

	//レーザーサイトの始点はウエポンオブジェクトの中心から少し離れたところ(画面ではちょうどオブジェクトの始点)
	XMFLOAT3 startPoint = weaponPosition;
	//startPoint.x -= 6.0f * cosf(angle);
	//startPoint.y -= 6.0f * sinf(angle);
	razorObject->SetPosition(startPoint);
}
