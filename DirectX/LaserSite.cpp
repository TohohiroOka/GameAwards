#include "LaserSite.h"
#include "SafeDelete.h"

LaserSite *LaserSite::Create()
{
	//インスタンスを生成
	LaserSite *instance = new LaserSite();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	return instance;
}

LaserSite::~LaserSite()
{
	safe_delete(line);
}

bool LaserSite::Initialize()
{
	//線生成
	line = DrawLine3D::Create();
	if (line == nullptr) {
		return false;
	}

	//色を指定
	color = { 1, 0.1f, 0.1f, 1 };
	//線の太さ
	weight = 0.5f;

	//線を作る
	line->SetLine(startPoint, endPoint, color, weight);

	return true;
}

void LaserSite::Update(Camera *camera)
{
	//レーザーを更新
	line->Update(camera);
}

void LaserSite::Draw()
{
	//レーザーを描画
	line->Draw();
}

void LaserSite::SetPosition(XMFLOAT3 weaponPosition, XMFLOAT3 weaponRotation)
{
	//レーザーサイトはウエポンオブジェクトの角度を追従する
	XMFLOAT3 rota = weaponRotation;
	float angle = DirectX::XMConvertToRadians(rota.z + 90);

	//レーザーサイトの始点はウエポンオブジェクトの中心から少し離れたところ(画面ではちょうどオブジェクトの始点)
	XMFLOAT3 startPoint = weaponPosition;
	startPoint.x += 6.0f * cosf(angle);
	startPoint.y += 6.0f * sinf(angle);
	this->startPoint = startPoint;

	//レーザーサイトの終点は始点からもっと遠いところ(画面ではちょうど強化されていない弾の描画が終了する地点)
	XMFLOAT3 endPoint = weaponPosition;
	endPoint.x += 35.0f * cosf(angle);
	endPoint.y += 35.0f * sinf(angle);
	this->endPoint = endPoint;

	//更新した座標で線を作り直す
	line->SetLine(startPoint, endPoint, color, weight);
}
