#include "PowerUpLine.h"
#include "SafeDelete.h"

PowerUpLine *PowerUpLine::Create(XMFLOAT3 startPoint, XMFLOAT3 endPoint)
{
	//インスタンスを生成
	PowerUpLine *instance = new PowerUpLine();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(startPoint, endPoint)) {
		delete instance;
		assert(0);
	}

	return instance;
}

PowerUpLine::~PowerUpLine()
{
	safe_delete(line);
}

bool PowerUpLine::Initialize(XMFLOAT3 startPoint, XMFLOAT3 endPoint)
{
	//線生成
	line = DrawLine3D::Create();
	if (line == nullptr) {
		return false;
	}

	//始点と終点をセット
	this->startPoint = startPoint;
	this->endPoint = endPoint;

	//色と太さを指定
	XMFLOAT4 color = { 0.4f, 1, 0.2f, 1 };
	float weight = 0.5;

	//線を作る
	line->SetLine(startPoint, endPoint, color, weight);

	return true;
}

void PowerUpLine::Update(Camera *camera)
{
	//線を最新の状態に更新
	line->Update(camera);
}

void PowerUpLine::Draw()
{
	//線を描画
	line->Draw();
}

void PowerUpLine::SetColor(XMFLOAT4 color)
{
	float weight = 0.5f;

	line->SetLine(startPoint, endPoint, color, weight);
}
