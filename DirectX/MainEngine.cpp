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
	//�E�B���h�E������
	winApp = new WindowApp();
	winApp->Initialize(window_width, window_height, gameName);

	//directX������
	dXCommon = new DirectXCommon();
	dXCommon->Initialize();

	//key
	input = Input::GetInstance();
	input->Initialize();

	//�p�b�h
	Xinput = XInputManager::GetInstance();
	Xinput->Initialize();

	//�J�����̏�����
	camera = new Camera();

	//Object�n�̏�����
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

	//�[�x�̏�����
	dXCommon->CreateDepth();

	fps = FrameRateKeep::Create();
}

bool MainEngine::Update()
{
	input->Update();
	Xinput->Update();

	//�G�X�P�[�v���~�������ꂽ�Ƃ��Q�[���I��
	if (input->PushKey(DIK_ESCAPE)|| gameFin(winApp) == true) { return true; }

	//�X�V
	scene->Update(camera);

	return false;
}

void MainEngine::Draw()
{
	//�`��
	postEffect->PreDrawScene(dXCommon->GetCmdList());
	scene->Draw(dXCommon->GetCmdList());
	postEffect->PostDrawScene(dXCommon->GetCmdList());
	
	//�`��O�ݒ�
	dXCommon->BeforeDraw();

	//�|�X�g�G�t�F�N�g�`��
	postEffect->Draw(dXCommon->GetCmdList());

	//�R�}���h���s
	dXCommon->AfterDraw();
}

void MainEngine::debugNum(float x, float y, float z)
{
	//�����̃f�o�b�O
	swprintf_s(str, L"%f,%f,%f\n", x, y, z);
	OutputDebugString(str);
}

bool MainEngine::gameFin(WindowApp* winApp) {
	//�~�������ꂽ�Ƃ�
	if (winApp->Update() == true) {
		return true;
	} else { return false; }

}

void MainEngine::FrameControl()
{
	fps->FixedFps(winApp);
}