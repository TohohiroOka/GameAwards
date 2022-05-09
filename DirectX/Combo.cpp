#include "Combo.h"
#include "SafeDelete.h"
#include "Easing.h"

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
		XMFLOAT2 texSize = { 48, 64 };
		numberSprite[i]->SetTexSize(texSize);

		//座標をセット
		XMFLOAT2 pos = { 777, 55 };
		pos.x -= size.x * i;
		numberSprite[i]->SetPosition(pos);
	}

	//COMBOスプライト生成
	comboSprite = Sprite::Create(comboTexNum);
	if (comboSprite == nullptr) {
		return false;
	}
	//大きさをセット
	XMFLOAT2 size = { 164, 38 };
	comboSprite->SetSize(size);
	//テクスチャサイズをセット
	XMFLOAT2 texSize = { 273, 63 };
	comboSprite->SetTexSize(texSize);
	//座標をセット
	XMFLOAT2 pos = { 880, 58 };
	comboSprite->SetPosition(pos);


	//コンボ数を0で生成
	combo = 0;
	//コンボ終了タイマー初期化
	lostComboTimer = 0;

	return true;
}

void Combo::Update()
{
	//ゲームシーンの座標に移動
	if (isMoveGamePos)
	{
		MoveGamePos();
	}
	//リザルトシーンの座標に移動
	else if (isMoveResultPos)
	{
		MoveResultPos();
	}

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
	//コンボ数
	combo = 0;
	//最大コンボ数
	maxCombo = 0;
	//コンボ終了タイマー
	lostComboTimer = 0;
	//ゲームシーンの座標に移動中か
	isMoveGamePos = false;
	//ゲームシーンの座標に移動終了したか
	isMoveGamePosEnd = false;
	//ゲームシーンの座標に移動する時間タイマー
	moveGamePosTimer = 0;
	//リザルトシーンの座標に移動中か
	isMoveResultPos = false;
	//リザルトシーンの座標に移動終了したか
	isMoveResultPosEnd = false;
	//リザルトシーンの座標に移動する時間タイマー
	moveResultPosTimer = 0;
	//表示用スプライトを0に戻す
	ChangeComboSprite();
	//スプライト更新
	for (int i = 0; i < comboDigits; i++)
	{
		numberSprite[i]->Update();
	}
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

	//更新したコンボ数が最大コンボ数を越えていたら最大コンボ数を更新する
	if (maxCombo < combo)
	{
		maxCombo = combo;
	}

	//コンボ数スプライトの数字更新
	ChangeComboSprite();

	//スプライトの透明度を1にする
	XMFLOAT4 color = comboSprite->GetColor();
	color.w = 1;
	for (int i = 0; i < comboDigits; i++)
	{
		numberSprite[i]->SetColor(color);
	}
	comboSprite->SetColor(color);

	//コンボ終了タイマー初期化
	lostComboTimer = 0;
}

void Combo::LostCombo()
{
	//コンボ数を0にする
	combo = 0;

	//スプライトの透明度をMAXにする
	XMFLOAT4 color = comboSprite->GetColor();
	color.w = 0;
	for (int i = 0; i < comboDigits; i++)
	{
		numberSprite[i]->SetColor(color);
	}
	comboSprite->SetColor(color);
}

void Combo::SetMoveGamePos()
{
	//ゲームシーンの座標に移動する時間タイマーを初期化
	moveGamePosTimer = 0;

	//移動状態にセット
	isMoveGamePos = true;
}

void Combo::SetMoveResultPos()
{
	//リザルトシーンの座標に移動する時間タイマーを初期化
	moveResultPosTimer = 0;

	//移動状態にセット
	isMoveResultPos = true;
}

void Combo::LostTimerUpdate()
{
	//コンボ終了時間
	const int lostComboTime = 300;

	//コンボ終了タイマー更新
	lostComboTimer++;

	//半分以上タイマーが進んだら色を薄くする
	if (lostComboTimer >= lostComboTime / 2)
	{
		//色を薄くする時間
		const int colorChangeTime = lostComboTime / 2;

		//色を薄くするタイマー
		const int colorChangeTimer = lostComboTimer - lostComboTime / 2;;

		//イージング計算用の時間
		float easeTimer = (float)colorChangeTimer / colorChangeTime;

		//スプライトの透明度をだんだん薄くしていく
		XMFLOAT4 color = comboSprite->GetColor();
		color.w = Easing::Lerp(1.0f, 0.1f, easeTimer);
		for (int i = 0; i < comboDigits; i++)
		{
			numberSprite[i]->SetColor(color);
		}
		comboSprite->SetColor(color);
	}

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

void Combo::MoveGamePos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveGamePosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveGamePosTimer / moveTime;

	//スプライトの座標を変更
	for (int i = 0; i < comboDigits; i++)
	{
		XMFLOAT2 numberPos = numberSprite[i]->GetPosition();
		numberPos.y = Easing::OutQuint(-100, 55, easeTimer);
		numberSprite[i]->SetPosition(numberPos);
	}
	XMFLOAT2 comboPos = comboSprite->GetPosition();
	comboPos.y = Easing::OutQuint(-100, 58, easeTimer);
	comboSprite->SetPosition(comboPos);

	//タイマーが指定した時間になったら
	if (moveGamePosTimer >= moveTime)
	{
		//移動状態終了
		isMoveGamePos = false;

		//移動完了
		isMoveGamePosEnd = true;
	}
}

void Combo::MoveResultPos()
{
	//移動を行う時間
	const int moveTime = 60;

	//タイマーを更新
	moveResultPosTimer++;

	//イージング計算用の時間
	float easeTimer = (float)moveResultPosTimer / moveTime;

	//スプライトの座標を変更
	for (int i = 0; i < comboDigits; i++)
	{
		XMFLOAT2 numberPos = numberSprite[i]->GetPosition();
		numberPos.y = Easing::OutQuint(55, -100, easeTimer);
		numberSprite[i]->SetPosition(numberPos);
	}
	XMFLOAT2 comboPos = comboSprite->GetPosition();
	comboPos.y = Easing::OutQuint(58, -100, easeTimer);
	comboSprite->SetPosition(comboPos);

	//タイマーが指定した時間になったら
	if (moveResultPosTimer >= moveTime)
	{
		//移動状態終了
		isMoveResultPos = false;

		//移動完了
		isMoveResultPosEnd = true;
	}
}
