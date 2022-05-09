#include "WallObject.h"
#include "Easing.h"

using namespace DirectX;

const XMFLOAT3 WallObject::startPosition = { 0,0,500 };
const float WallObject::directingMaxTime = 150;
const XMFLOAT2 WallObject::minPosition = { -196, -102 };
const XMFLOAT2 WallObject::maxPosition = { 196, 70 };
const float WallObject::initDistance = 50.0f;
const float WallObject::disperseMaxTime = 30.0f;

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

	distance= Easing::Lerp(initDistance, 0, progress);

	position.x = cos(radius) * distance + Easing::Lerp(startPosition.x, easingPos.end.x, progress);
	position.y = sin(radius) * distance + Easing::Lerp(-55, easingPos.end.y, progress);
	position.z = Easing::Lerp(startPosition.z, 0, progress);
}

void WallObject::WallMove()
{
	const float speed = 2.0f;
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
	//壁移動
	else if (state >= STATE::MOVE_UP && state <= STATE::MOVE_LEFT)
	{
		WallMove();
	}
	//散らばる
	else if (state == STATE::DISPERSE)
	{
		if (oldState >= STATE::MOVE_UP && oldState <= STATE::MOVE_LEFT)
		{
			//イージングの終点登録
			easingPos.start = position;
			easingPos.end.x = position.x + disperseMovePos.x;
			easingPos.end.y = position.y + disperseMovePos.y;
			easingPos.end.z = position.z + disperseMovePos.z;
			easingRota.start = rotation;
			easingRota.end = disperseMoveRota;
			time = 0;
		}

		//時間を進める
		time++;
		//lerp%
		float progress = time / directingMaxTime;

		//疑似Lerpで座標移動
		position.x = Easing::InSine(easingPos.start.x, easingPos.end.x, progress);
		position.y = Easing::InSine(easingPos.start.y, easingPos.end.y, progress);
		position.z = Easing::InSine(easingPos.start.z, easingPos.end.z, progress);
		//Lerpで回転
		rotation.x = Easing::InSine(easingRota.start.x, easingRota.end.x, progress);
		rotation.y = Easing::InSine(easingRota.start.y, easingRota.end.y, progress);
		rotation.z = Easing::InSine(easingRota.start.z, easingRota.end.z, progress);

		//ちりばめたので待機状態にする
		if (time > disperseMaxTime)
		{
			state = STATE::WAIT;
		}
	}

	Object3d::Update();

	//一フレーム前の状態保持
	oldState = state;
}
