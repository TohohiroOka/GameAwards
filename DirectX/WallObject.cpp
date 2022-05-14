#include "WallObject.h"
#include "Easing.h"
#include "StageEffect.h"

using namespace DirectX;

const XMFLOAT2 WallObject::minPosition = { -204, -110 };
const XMFLOAT2 WallObject::maxPosition = { 204, 78 };
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

void WallObject::WallMove()
{
	const float speed = 2.0f * (1.0f - slow);

	//速度が0以下なら次に行く
	if (speed <= 0.2f)
	{
		time = 0;
		isSlow = false;
		slow = 0.0f;
		state = STATE::WAIT;
	}

	//左へ移動
	if (state == STATE::MOVE_UP_LEFT || state == STATE::MOVE_DOWN_LEFT)
	{
		position.x -= speed;
	}
	//下に移動
	else if (state == STATE::MOVE_RIGHT_DOWN || state == STATE::MOVE_LEFT_DOWN)
	{
		position.y -= speed / 1.1f;
	}
	//右に移動
	else if (state == STATE::MOVE_UP_RIGHT || state == STATE::MOVE_DOWN_RIGHT)
	{
		position.x += speed;
	}
	//上に移動
	else if (state == STATE::MOVE_RIGHT_UP || state == STATE::MOVE_LEFT_UP)
	{
		position.y += speed / 1.1f;
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
		slow += 0.1f;
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

	SetPosition({ 0,0,0 });
	SetScale({ 2,2,2 });

	return true;
}

void WallObject::Update()
{
	//壁移動
	if (state >= STATE::MOVE_UP_LEFT && state <= STATE::MOVE_LEFT_UP)
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
	//Object3Dの初期化
	position = { 0,0,0 };
	rotation = { 0,0,0 };
	color = { 1,1,1,1 };
}