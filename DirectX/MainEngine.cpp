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
	DebugText::AllDelete();
	safe_delete(dXCommon);
	safe_delete(winApp);
}

//メインに書く（初期化処理）
void MainEngine::Initialize(const wchar_t* gameName, int window_width, int window_height)
{
	
	//ウィンドウ初期化
	winApp = new WindowApp();
	winApp->Initialize(window_width, window_height, gameName);

	//1フレームの時間設定
	QueryPerformanceFrequency(&timeFreq);

	// 1度取得しておく(初回計算用)
	QueryPerformanceCounter(&timeStart);

	//directX初期化
	dXCommon = new DirectXCommon();
	dXCommon->Initialize(winApp);

	//key
	input = Input::GetInstance();
	input->Initialize(winApp);

	//カメラの初期化
	camera = new Camera(window_width, window_height);

	//Object系の初期化
	Object3d::StaticInitialize(dXCommon->GetDevice(), camera);
	Sprite::StaticInitialize(dXCommon->GetDevice(), window_width, window_height);
	DrawLine::StaticInitialize(dXCommon->GetDevice(), window_width, window_height);
	DrawLine3D::StaticInitialize(dXCommon->GetDevice());
	ParticleManager::Initialize(dXCommon);
	LightGroup::StaticInitialize(dXCommon->GetDevice());
	Fbx::StaticInitialize(dXCommon->GetDevice());
	NormalMap::StaticInitialize(dXCommon->GetDevice());

	scene = new GameScene();
	scene->Initialize(input, camera);

	Sprite::LoadTexture(0, L"Resources/LetterResources/debugfont.png");

	//深度の初期化
	dXCommon->CreateDepth(winApp);

}

//メインに書く（更新処理）
bool MainEngine::Update()
{
	input->Update();

	//エスケープか×が押されたときゲーム終了
	if (input->PushKey(DIK_ESCAPE)|| gameFin(winApp) == true) { return true; }

	//更新
	scene->Update(camera);

	//描画前設定
	dXCommon->BeforeDraw(winApp);

	//描画
	scene->Draw(dXCommon->getcmdList());

	return false;
}

//デバッグ用数字
void MainEngine::debugNum(float x, float y, float z)
{
	//数字のデバッグ
	swprintf_s(str, L"%f,%f,%f\n", x, y, z);
	OutputDebugString(str);
}

//メインに書く（描画処理）
void MainEngine::draw() {
	//コマンド実行
	dXCommon->AfterDraw();
}

void MainEngine::frameRateKeep() {
	// 今の時間を取得
	QueryPerformanceCounter(&timeEnd);
	// (今の時間 - 前フレームの時間) / 周波数 = 経過時間(秒単位)
	frameTime = static_cast<float>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<float>(timeFreq.QuadPart);

	if (frameTime < MIN_FREAM_TIME) { // 時間に余裕がある
		// ミリ秒に変換
		DWORD sleepTime = static_cast<DWORD>((MIN_FREAM_TIME - frameTime) * 1000);

		timeBeginPeriod(1); // 分解能を上げる(こうしないとSleepの精度はガタガタ)
		Sleep(sleepTime);   // 寝る
		timeEndPeriod(1);   // 戻す

		// 次週に持ち越し(こうしないとfpsが変になる?)
		return;
	}

	if (frameTime > 0.0) { // 経過時間が0より大きい(こうしないと下の計算でゼロ除算になると思われ)
		fps = (fps * 0.99f) + (0.01f / frameTime); // 平均fpsを計算
#ifdef _DEBUG
// デバッグ用(デバッガにFSP出す)
#ifdef UNICODE
		std::wstringstream stream;
#else
		std::stringstream stream;
#endif
		stream << fps << " FPS" << std::endl;
		// カウンタ付けて10回に1回出力、とかにしないと見づらいかもね
		OutputDebugString(stream.str().c_str());
#endif // _DEBUG
	}

	timeStart = timeEnd; // 入れ替え
}

//メインに書く（エスケープが入力されたら終了する処理）
bool MainEngine::gameFin(WindowApp* winApp) {
	//×が押されたとき
	if (winApp->Update() == true) {
		return true;
	} else { return false; }

}