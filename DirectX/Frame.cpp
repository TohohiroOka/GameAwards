#include "Frame.h"
#include "SafeDelete.h"
#include "Easing.h"

Frame* Frame::Create(Model* model)
{
	//インスタンスを生成
	Frame* instance = new Frame();
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

Frame::~Frame()
{
	safe_delete(frameObject);
}

bool Frame::Initialize(Model* model)
{
	//枠オブジェクト生成
	frameObject = Object3d::Create();
	if (frameObject == nullptr) {
		return false;
	}

	//初期地点と大きさと色をセット
	frameObject->SetPosition({ 0, 0, 0 });
	frameObject->SetScale({ 5.9f, 6.2f, 1 });
	frameObject->SetColor({ 0,0,0,1 });

	//モデルをセット
	if (model) {
		frameObject->SetModel(model);
	}

	return true;
}

void Frame::Update()
{
	//破壊されていたら更新しないで抜ける
	if (isBreak) { return; }

	//枠のライン変更状態の場合のみ
	if (isChangeFrameLine)
	{
		//枠のラインと枠オブジェクトの大きさ変更
		ChangeFrameLine();
	}

	//オブジェクト更新
	frameObject->Update();
}

void Frame::Draw()
{
	//破壊されていたら描画しないで抜ける
	if (isBreak) { return; }

	//オブジェクト描画
	frameObject->Draw();
}

void Frame::Reset()
{
	//オブジェクト初期化
	frameObject->SetPosition({ 0, 0, 0 });
	frameObject->SetScale({ 5.9f, 6.2f, 1 });
	frameObject->SetColor({ 0,0,0,1 });

	//画面上で見たときの枠のラインの位置初期化
	frameLine = { 100, 55 };
	//枠のラインの位置を変更中ではない
	isChangeFrameLine = false;
	//枠のラインの位置イージングタイマー初期化
	frameLineEaseTimer = 0;
	//破壊状態から戻す
	isBreak = false;
	//オブジェクト更新
	frameObject->Update();
}

void Frame::SetChangeFrameLine(char nextFrameNum)
{
	//引数の番号に合わせてラインとオブジェクトの大きさを変更する
	XMFLOAT2 line = {};
	XMFLOAT3 objectScale = {};
	if (nextFrameNum == 1)
	{
		line = { 100, 55 };
		objectScale = { 5.9f, 6.2f, 1 };
	} else if (nextFrameNum == 2)
	{
		line = { 151, 83 };
		objectScale = { 8.85f, 9.3f, 1 };
	} else if (nextFrameNum == 3)
	{
		line = { 202, 112 };
		objectScale = { 11.8f, 12.4f, 1 };
	} else
	{
		return;
	}

	//変更前の枠オブジェクトの大きさをセット
	frameScaleEaseStart = frameObject->GetScale();
	//変更後の枠オブジェクトの大きさをセット
	frameScaleEaseEnd = objectScale;
	//変更前の枠のラインをセット
	frameLineEaseStart = frameLine;
	//変更後の枠のラインをセット
	frameLineEaseEnd = line;
	//枠のライン変更タイマーを初期化
	frameLineEaseTimer = 0;
	//枠のラインを変更中にする
	isChangeFrameLine = true;
}

void Frame::ChangeFrameLine()
{
	//枠のライン変更
	if (isChangeFrameLine)
	{
		//枠のライン変更を行う時間
		const int changeTime = 100;

		//枠のライン変更タイマー更新
		frameLineEaseTimer++;

		//イージング計算用の時間
		float easeTimer = (float)frameLineEaseTimer / changeTime;

		//イージングで枠のライン変更
		XMFLOAT2 frameLine = {};
		frameLine.x = Easing::InQuint(frameLineEaseStart.x, frameLineEaseEnd.x, easeTimer);
		frameLine.y = Easing::InQuint(frameLineEaseStart.y, frameLineEaseEnd.y, easeTimer);
		//枠のラインを更新
		this->frameLine = frameLine;

		//イージングで枠オブジェクトの大きさ
		XMFLOAT3 scale = {};
		scale.x = Easing::InQuint(frameScaleEaseStart.x, frameScaleEaseEnd.x, easeTimer);
		scale.y = Easing::InQuint(frameScaleEaseStart.y, frameScaleEaseEnd.y, easeTimer);
		//枠オブジェクトの大きさを更新
		frameObject->SetScale(scale);

		//タイマーが指定した時間になったら
		if (frameLineEaseTimer >= changeTime)
		{
			//枠のライン変更状態終了
			isChangeFrameLine = false;
		}
	}
}

void Frame::Break()
{
	//破壊する
	isBreak = true;
}

void Frame::ChangeColor(float ratio)
{
	frameObject->SetColor({ resetColor.x * ratio,resetColor.y * ratio,resetColor.z * ratio,resetColor.w * ratio });
}