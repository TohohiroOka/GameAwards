#include "WallObject.h"
#include "Easing.h"
#include "StageEffect.h"

using namespace DirectX;

const XMFLOAT3 WallObject::startPosition = { 0,0,500 };
float WallObject::directingMaxTime = 0;
const XMFLOAT2 WallObject::minPosition = { -204, -110 };
const XMFLOAT2 WallObject::maxPosition = { 204, 78 };
const float WallObject::initDistance = 50.0f;
const float WallObject::disperseMaxTime = 30.0f;
const float WallObject::transparentMaxTime = 30.0f;
bool WallObject::isSlow = false;

WallObject* WallObject::Create(Model* model)
{
	// 3Dオブジェクトのインスタンスを生成
	WallObject* instance = new WallObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

void WallObject::Directing()
{
	//ラジアン
	float radius = XMConvertToRadians((float)angle);
	//lerp%
	float progress = time / directingMaxTime;

	distance = Easing::Lerp(initDistance, 0, progress);

	position.x = cos(radius) * distance + Easing::Lerp(0, easingPos.end.x, progress);
	position.y = sin(radius) * distance + Easing::Lerp(-55, easingPos.end.y, progress);
	position.z = Easing::Lerp(startPosition.z, 0, progress);

	//定位置への移動が終わったら壁を作る
	if (time >= directingMaxTime)
	{
		//壁移動に状態を変更
		//右に移動
		if (state == STATE::DIRECTING_LEFT_UP)
		{
			state = STATE::MOVE_UP;
		}
		//下に移動
		else if (state == STATE::DIRECTING_RIGHT_UP)
		{
			state = STATE::MOVE_RIGHT;
		}
		//左に移動
		else if (state == STATE::DIRECTING_RIGHT_DOWN)
		{
			state = STATE::MOVE_DOWN;
		}
		//上に移動
		else if (state == STATE::DIRECTING_LEFT_DOWN)
		{
			state = STATE::MOVE_LEFT;
		}
	}
}

void WallObject::WallMove()
{
	const float speed = 4.0f * (1.0f - slow);

	//速度が0以下なら次に行く
	if (speed <= 0.2f)
	{
		time = 0;
		isSlow = false;
		slow = 0.0f;
		state = STATE::WAIT;
	}

	//右に移動
	if (state == STATE::MOVE_UP)
	{
		position.x += speed;
		if (position.x >= maxPosition.x)
		{
			state = STATE::MOVE_RIGHT;
		}
	}
	//下に移動
	else if (state == STATE::MOVE_RIGHT)
	{
		position.y -= speed;
		if (position.y <= minPosition.y)
		{
			state = STATE::MOVE_DOWN;
		}
	}
	//左に移動
	else if (state == STATE::MOVE_DOWN)
	{
		position.x -= speed;
		if (position.x <= minPosition.x)
		{
			state = STATE::MOVE_LEFT;
		}
	}
	//上に移動
	else if (state == STATE::MOVE_LEFT)
	{
		position.y += speed;
		if (position.y >= maxPosition.y)
		{
			state = STATE::MOVE_UP;
		}
	}

	//角度変更
	rotation.x += 3;
	rotation.y += 5;
	rotation.z += 1;
	//大きさ変更
	if (scale.x < 8.0f)
	{
		scale.x += 0.04f;
		scale.y += 0.04f;
		scale.z += 0.04f;
	}

	//減速させる
	if (isSlow)
	{
		slow += 0.05f;
	}
}

void WallObject::Transparency()
{
	//lerp%
	float progress = time / transparentMaxTime;

	color.w = Easing::Lerp(1, 0, progress);
	if (time > transparentMaxTime)
	{
		time = 0;
		scale.x = 5.0f;
		scale.y = 5.0f;
		scale.z = 5.0f;
		state = STATE::NONE;
	}
}

bool WallObject::Initialize()
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	SetPosition({ 0,0,-100 });
	SetScale({ 2,2,2 });
	distance = initDistance;

	return true;
}

void WallObject::Update()
{
	//定位置への移動
	if (state >= STATE::DIRECTING_LEFT_UP && state <= STATE::DIRECTING_LEFT_DOWN)
	{
		angle += 6;
		time++;
		Directing();
	}
	//壁移動
	else if (state >= STATE::MOVE_UP && state <= STATE::MOVE_LEFT)
	{
		WallMove();
	}
	//割合破壊に透明化する
	else if (state == STATE::TRANSPARENCY)
	{
		time++;
		Transparency();

		//particle
		StageEffect::SetWallBreak(position);
	}

	Object3d::Update();

	//一フレーム前の状態保持
	oldState = state;
}

void WallObject::Draw()
{
	if (state != STATE::NONE)
	{
		Object3d::Draw();
	}
}

void WallObject::Reset()
{
	//オブジェクトの現在の状態
	state = STATE::NONE;
	//オブジェクトの前の状態
	oldState = STATE::NONE;
	//スタート時の演出時間
	time = 0;
	//回転の演出時に使用する角度
	angle = 0;
	//回転時の幅
	distance = 0;
	//イージングで使う座標
	easingPos = {};
	//イージングで使う回転
	easingRota = {};
	//ちりばめ時の移動距離
	disperseMovePos = {};
	//ちりばめ時の回転角
	disperseMoveRota = {};
	//Object3Dの初期化
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	color = { 1,1,1,1 };
}