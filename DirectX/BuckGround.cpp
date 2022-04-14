#include "BuckGround.h"
#include "SafeDelete.h"

std::vector<Object3d*> BuckGround::buckGround = { nullptr };

BuckGround::~BuckGround()
{
	
}

void BuckGround::AllDelete()
{
	for (int i = 0; i < arreyNum; i++)
	{
		safe_delete(buckGround[i]);
		buckGround[i] = nullptr;
	}
	buckGround.clear();
}

BuckGround* BuckGround::Create(Model* model)
{
	// オブジェクトのインスタンスを生成
	BuckGround* instance = new BuckGround();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(model)){
		delete instance;
		assert(0);
	}

	return instance;
}

bool BuckGround::Initialize(Model* model)
{
	//サイズ変更
	buckGround.resize(arreyNum);
	const XMFLOAT3 initialPos = { -352.5,-185,150 };
	const XMFLOAT2 width = { 54.6f,48.0f };
	const float scale = 30.0f;
	int arrey = 0;

	for (int y = 0; y < widthY; y++)
	{
		for (int x = 0; x < widthX; x++)
		{
			float dist = 0;
			if (y % 2 == 1) { dist = width.x / 2.0f; }
			buckGround[arrey] = Object3d::Create();
			buckGround[arrey]->SetModel(model);
			if (buckGround[arrey] == nullptr) {
				return false;
			}
			buckGround[arrey]->SetPosition({ initialPos.x + x * width.x + dist ,initialPos.y + y * width.y ,initialPos.z });
			buckGround[arrey]->SetScale({ scale,scale,scale });
			buckGround[arrey]->SetColor({ 0.1f,0.1f,0.1f,1 });
			arrey++;
		}
	}

	return true;
}

void BuckGround::Update()
{
	for (int i = 0; i < arreyNum; i++)
	{
		buckGround[i]->Update();
	}
}

void BuckGround::Draw()
{
	for (int i = 0; i < arreyNum; i++)
	{
		buckGround[i]->Draw();
	}
}