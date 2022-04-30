#include "Combo.h"
#include "SafeDelete.h"

Combo* Combo::Create(int numberTexNum, int comboTexNum)
{
	//インスタンスを生成
	Combo* instance = new Combo();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(numberTexNum, comboTexNum)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Combo::~Combo()
{
	for (int i = 0; i < comboDigits; i++)
	{
		safe_delete(numberSprite[i]);
	}
	safe_delete(comboSprite);
}

bool Combo::Initialize(int numberTexNum, int comboTexNum)
{
	//コンボの桁数分回す
	for (int i = 0; i < comboDigits; i++)
	{
		//コンボ数表示用スプライト生成
		numberSprite[i] = Sprite::Create(numberTexNum);
		if (numberSprite[i] == nullptr) {
			return false;
		}

		//大きさをセット
		XMFLOAT2 size = { 32, 64 };
		numberSprite[i]->SetSize(size);

		//テクスチャサイズをセット
		XMFLOAT2 texSize = { 32, 64 };
		numberSprite[i]->SetTexSize(texSize);

		//座標をセット
		XMFLOAT2 pos = { 952, 55 };
		pos.x -= size.x * i;
		numberSprite[i]->SetPosition(pos);
	}

	//COMBOスプライト生成
	comboSprite = Sprite::Create(comboTexNum);
	if (comboSprite == nullptr) {
		return false;
	}
	//大きさをセット
	XMFLOAT2 size = { 250, 50 };
	comboSprite->SetSize(size);
	//テクスチャサイズをセット
	XMFLOAT2 texSize = { 300, 64 };
	comboSprite->SetTexSize(texSize);
	//座標をセット
	XMFLOAT2 pos = { 1100, 58 };
	comboSprite->SetPosition(pos);


	//コンボ数を0で生成
	combo = 0;
	//コンボ終了タイマー初期化
	lostComboTimer = 0;

	return true;
}

void Combo::Update()
{
	//コンボ数が1以上のときにコンボ終了タイマーを更新
	if (combo >= 1)
	{
		LostTimerUpdate();
	}

	//スプライト更新
	for (int i = 0; i < comboDigits; i++)
	{
		numberSprite[i]->Update();
	}
	comboSprite->Update();
}

void Combo::Draw()
{
	//スプライト描画
	if (combo >= 100)
	{
		numberSprite[2]->Draw();
	}
	if (combo >= 10)
	{
		numberSprite[1]->Draw();
	}
	if (combo >= 1)
	{
		numberSprite[0]->Draw();
		comboSprite->Draw();
	}
}

void Combo::Reset()
{
}

void Combo::AddCombo()
{
	//コンボ数を増やす
	combo++;
	const int comboMax = 999;

	//コンボは最大値を越えない
	if (combo >= comboMax) 
	{
		combo = comboMax;
	}

	//コンボ数スプライトの数字更新
	ChangeComboSprite();

	//コンボ終了タイマー初期化
	lostComboTimer = 0;
}

void Combo::LostCombo()
{
	//コンボ数を0にする
	combo = 0;
}

void Combo::LostTimerUpdate()
{
	//コンボ終了時間
	const int lostComboTime = 300;

	//コンボ終了タイマー更新
	lostComboTimer++;

	//コンボ終了タイマーが指定時間にいったら
	if (lostComboTimer >= lostComboTime)
	{
		LostCombo();
	}
}

void Combo::ChangeComboSprite()
{
	//3桁の数字をそれぞれ出力する
	int digit[comboDigits];
	digit[0] = combo % 10;			//001
	digit[1] = (combo / 10) % 10;	//010
	digit[2] = (combo / 100) % 10;	//100

	//それぞれの桁の数字分スプライトのテクスチャ切り出しをずらす
	for (int i = 0; i < comboDigits; i++)
	{
		XMFLOAT2 leftTop = {};
		leftTop.x = numberSprite[i]->GetTexSize().x * digit[i];
		numberSprite[i]->SetTexLeftTop(leftTop);
	}
}
