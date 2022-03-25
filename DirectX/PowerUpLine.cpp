#include "PowerUpLine.h"
#include "SafeDelete.h"

PowerUpLine *PowerUpLine::Create(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint)
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

bool PowerUpLine::Initialize(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint)
{
	//線生成
	line = DrawLine3D::Create();
	if (line == nullptr) {
		return false;
	}

	//始点と終点をセット
	this->startPoint = startPoint;
	this->endPoint = endPoint;

	//色を指定
	XMFLOAT4 color = { 0.4f, 1, 0.2f, 1 };

	//線を作る
	line->SetLine(startPoint->GetPosition(), endPoint->GetPosition(), color, weight);

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

bool PowerUpLine::IsAlreadyCreateLine(DeadEnemyPoint *startPoint, DeadEnemyPoint *endPoint)
{
	//指定した始点と終点で既に線が作られていたらtrueを返す
	if (this->startPoint == startPoint && this->endPoint == endPoint)
	{
		return true;
	}
	//始点と終点が逆の可能性も判定する
	if (this->startPoint == endPoint && this->endPoint == startPoint)
	{
		return true;
	}

	return false;
}

void PowerUpLine::CheckUsePoints(DeadEnemyPoint *point)
{
	//始点か終点で引数の円を使用していたら
	if (point == startPoint || point == endPoint)
	{
		//削除フラグをtrueに
		isDelete = true;
	}
}

void PowerUpLine::SetColor(XMFLOAT4 color)
{
	//色を変えて線を作り直す
	line->SetLine(startPoint->GetPosition(), endPoint->GetPosition(), color, weight);
}
