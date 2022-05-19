#include "MainEngine.h"
#include "DrawLine.h"
#include "DrawLine3D.h"
#include "GameScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "Emitter.h"
#include "Fbx.h"
#include "NormalMap.h"
#include "SafeDelete.h"

MainEngine::~MainEngine()
{
	safe_delete(camera);
	safe_delete(scene);
	safe_delete(postEffect);
	DebugText::AllDelete();
	DrawLine3D::AllDelete();
	DrawLine::AllDelete();
	Object3d::AllDelete();
	Sprite::AllDelete();
	Fbx::AllDelete();
	ParticleManager::AllDelete();
	NormalMap::AllDelete();
	safe_delete(dXCommon);
	safe_delete(winApp);
}

void MainEngine::Initialize(const wchar_t* gameName, int window_width, int window_height)
{
	//ウィンドウ初期化
	winApp = new WindowApp();
	winApp->Initialize(window_width, window_height, gameName);

	//directX初期化
	dXCommon = new DirectXCommon();
	dXCommon->Initialize();

	//key
	input = Input::GetInstance();
	input->Initialize();

	//パッド
	Xinput = XInputManager::GetInstance();
	Xinput->Initialize();

	//Audio
	audio = Audio::GetInstance();
	audio->Initialize();

	//カメラの初期化
	camera = new Camera();

	//Object系の初期化
	Object3d::StaticInitialize(dXCommon->GetDevice(), camera);
	Sprite::StaticInitialize(dXCommon->GetDevice());
	DrawLine::StaticInitialize(dXCommon->GetDevice());
	DrawLine3D::StaticInitialize(dXCommon->GetDevice());
	ParticleManager::Initialize(dXCommon);
	LightGroup::StaticInitialize(dXCommon->GetDevice());
	Fbx::StaticInitialize(dXCommon->GetDevice());
	NormalMap::StaticInitialize(dXCommon->GetDevice());

	scene = new GameScene();
	scene->Initialize(camera);

	Sprite::LoadTexture(0, L"Resources/LetterResources/debugfont.png");

	postEffect = new PostEffect();
	postEffect->Initialize();

	//深度の初期化
	dXCommon->CreateDepth();

	fps = FrameRateKeep::Create();
}

bool MainEngine::Update()
{
	input->Update();
	Xinput->Update();

	//エスケープか×が押されたときゲーム終了
	if (input->PushKey(DIK_ESCAPE)|| gameFin(winApp) == true) { return true; }

	//更新
	scene->Update(camera);

	return false;
}

void MainEngine::Draw()
{
	//描画
	postEffect->PreDrawScene(dXCommon->GetCmdList());
	scene->Draw(dXCommon->GetCmdList());
	postEffect->PostDrawScene(dXCommon->GetCmdList());
	
	//描画前設定
	dXCommon->BeforeDraw();

	//ポストエフェクト描画
	postEffect->Draw(dXCommon->GetCmdList());

	//コマンド実行
	dXCommon->AfterDraw();
}

void MainEngine::debugNum(float x, float y, float z)
{
	//数字のデバッグ
	swprintf_s(str, L"%f,%f,%f\n", x, y, z);
	OutputDebugString(str);
}

bool MainEngine::gameFin(WindowApp* winApp) {
	//×が押されたとき
	if (winApp->Update() == true) {
		return true;
	} else { return false; }

}

void MainEngine::FrameControl()
{
	fps->FixedFps(winApp);
}