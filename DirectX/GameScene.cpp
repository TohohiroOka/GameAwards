#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>

#include "Player.h"
#include "Ground.h"
#include "TouchableObject.h"

const float radian = XM_PI / 180.0f;//ラジアン

GameScene::~GameScene()
{
	//newオブジェクトの破棄
	safe_delete(audio);
	safe_delete(sprite);
	safe_delete(emit);
	safe_delete(uma);
	safe_delete(ground);
	safe_delete(PLAYER);
	safe_delete(GROUND);
	safe_delete(water);
	safe_delete(light);
	safe_delete(anm);
}

void GameScene::Initialize(Input* input, Camera* camera)
{
	// nullptrチェック
	assert(input);
	this->input = input;

	//ライト
	light = LightGroup::Create();
	light->DefaultLightSetting();
	// 3Dオブエクトにライトをセット
	Object3d::SetLightGroup(light);

	//スプライト
	Sprite::LoadTexture(1, L"Resources/amm.jpg");

	text = DebugText::GetInstance();
	text->Initialize(0);

	sprite = Sprite::Create();

	//object3d
	uma = Model::CreateFromOBJ("uma");
	ground = Model::CreateFromOBJ("wall");
	block = Model::CreateFromOBJ("Square");

	PLAYER = Player::Create(uma);
	
	GROUND = Ground::Create(ground);
	GROUND->SetScale(100);
	GROUND->SetPosition({ 1, -5, 0 });
	
	TouchableObject* Tobject3d = TouchableObject::Create(block);
	Tobject3d->SetScale(10.0f);
	Tobject3d->SetPosition({ -100,0,-100 });
	BLOCK = Tobject3d;

	//NormalMap
	tex[0] = NormalMap::LoadTexture(L"Resources/white1x1.png");
	tex[1] = NormalMap::LoadTexture(L"Resources/wN1.jpg");
	tex[2] = NormalMap::LoadTexture(L"Resources/pN2.png");

	water = new NormalMap();
	water->Create();

	//Fbx
	FbxUma = Fbx::LoadFbx("uma");

	anm = new Fbx();
	anm->CreateModel(FbxUma);

	//パーティクル用
	ParticleManager::LoadTexture(0, L"Resources/particle/effect1.png");

	emit = new Emitter();
	emit->Create(0);

	//サウンド用
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	camera->SetPosition(PLAYER->GetPosition());

	//Obj
	PLAYER->Update();
	GROUND->Update();
	BLOCK->Update();

	//NormalMap
	XMFLOAT4 normal = { 1.0f,1.0f,1.0f,1.0f };
	//法線マップの移動処理
	if (lightF) { lightPos -= 0.5f; } else if (lightF == false) { lightPos += 0.5f; }

	if (lightPos > 30) { lightF = true; }
	if (lightPos < -30) { lightF = false; }

	uvPos += 0.2f;
	water->SetInformation({ 100,5,100 }, { 0,0,0 }, { 2,2,2 },
		{ 1.0f,1.0f,1.0f,1.0f }, { 0.1f, 0.7f, 1.0f, 0.5f }, { 1.0f, 1.0f, 1.0f, 0.5f }, lightPos, uvPos);
	water->Update(camera);

	//Fbx
	anm->SetInformation({ 0,0,0 }, { 0,0,0 }, { 10,10,10 }, true);
	anm->Update(camera);

	//パーティクル
	emit->InEmitter(50, 30, { (float)(rand() % 5)-20,0,(float)(rand() % 5) }, { 0,2,0 },
		{ 0,0,0 }, 50, 1, { 1,1,1,1 }, { 0,0,0,1 });

	//ライト
	light->Update();

	//スプライト
	sprite->Update(1, { 0,0 }, { 100,100 });

	emit->Update(camera);

	camera->UpdateTps({ 0,50,-50 });
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	NormalMap::PreDraw(cmdList);
	water->Draw(tex[0], tex[1], tex[2]);
	NormalMap::PostDraw();

	Object3d::PreDraw(cmdList);
	PLAYER->Draw();
	GROUND->Draw();
	BLOCK->Draw();
	Object3d::PostDraw();

	Fbx::PreDraw(cmdList);
	anm->Draw(FbxUma);
	Fbx::PostDraw();

	//スプライト描画
	Sprite::PreDraw(cmdList);
	sprite->Draw();
	Sprite::PostDraw();

	//パーティクル
	emit->Draw();

}
