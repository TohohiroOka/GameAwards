#include "WallManager.h"
#include "SafeDelete.h"
#include "Easing.h"
#include <random>

WallManager* WallManager::Create()
{
	//インスタンスを生成
	WallManager* instance = new WallManager();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	return instance;
}

WallManager::~WallManager()
{
	for (int i = 0; i < 10; i++)
	{
		safe_delete(model[i]);
	}

	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		safe_delete((*itr));
	}
	object.clear();
}

void WallManager::Update()
{
	if (isSetEffect != 0)
	{
		SetUpEffect();
	}

	//リザルトシーン用に動かす
	if (isChangeResult)
	{
		ChangeResult();
	}

	//休憩中
	if (isBreakTime)
	{
		BreakTime();
	}
	//壁生成中
	else if (isCreate)
	{
		CreateWall();
	}

	//オブジェクト更新
	int num = 0;
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		if ((int)status.wallNum < num) { continue; }
		(*itr)->Update();
		num++;
	}
}

void WallManager::Draw()
{
	if (isBreakTime) { return; }

	//オブジェクト描画
	int num = 0;
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		if ((int)status.wallNum < num) { continue; }
		(*itr)->Draw();
		num++;
	}
}

void WallManager::Reset()
{
	//オブジェクト個数
	status.wallNum = WALL_STEP::step1;
	//壁の最大HP
	maxHP = 20;
	//壁のHP
	status.hp = maxHP;
	//休憩時間タイマー
	breakTimer = 0;
	//休憩中か
	isBreakTime = true;
	//壁生成タイマー
	createTimer = 0;
	//壁生成回数
	createCount = 0;
	//壁生成中か
	isCreate = false;
	//リザルトシーン用に動かしす時間タイマー
	changeResultTimer = 0;
	//壁をリザルトシーン用に動かしているか
	isChangeResult = false;
	//壊されたか
	isBreak = false;
	//生きているか
	status.isAlive = false;
}

void WallManager::Damage(int damagePower)
{
	//引数で指定した強さの分HPを減らす
	status.hp -= damagePower;

	//残りHPに応じて色変更
	ChangeColor();

	//HPが0以下になったら破壊
	if (status.hp <= 0)
	{
		Break();

		//生きていない
		status.isAlive = false;
	}
}

void WallManager::Break()
{
	//破壊する
	isBreak = true;

	//休憩状態にする
	SetBreakTime();
}

void WallManager::SetBreakTime()
{
	//休憩時間タイマーを初期化
	breakTimer = 0;

	//休憩状態にする
	isBreakTime = true;
}

void WallManager::SetCreateWall()
{
	//最大HPを設定
	createCount++;
	maxHP = baseMaxHP + (10 * createCount);
	status.hp = maxHP;

	//壁生成初期サイズをセット
	createStartScale = { 12.1f, 12.7f, 1 };
	//壁生成最終サイズをセット
	createEndScale = { 11.8f, 12.4f, 1 };

	//壁生成タイマーを初期化
	createTimer = 0;

	//壁生成状態にする
	isCreate = true;
}

bool WallManager::GetTriggerBreak()
{
	//破壊されていたら
	if (isBreak) 
	{
		//トリガーなのでfalseに戻す
		isBreak = false;

		return true;
	}

	return false;
}

void WallManager::SetChangeResult()
{
	//リザルトシーン用に動かすタイマーを初期化
	changeResultTimer = 0;

	//リザルトシーン用に動かす状態にセット
	isChangeResult = true;
}

void WallManager::LoadModel()
{
	model[0] = Model::CreateFromOBJ("garakuta1");
	model[1] = Model::CreateFromOBJ("garakuta2");
	model[2] = Model::CreateFromOBJ("garakuta3");
	model[3] = Model::CreateFromOBJ("garakuta4");
	model[4] = Model::CreateFromOBJ("garakuta5");
	model[5] = Model::CreateFromOBJ("garakuta6");
	model[6] = Model::CreateFromOBJ("garakuta7");
	model[7] = Model::CreateFromOBJ("garakuta8");
	model[8] = Model::CreateFromOBJ("garakuta9");
	model[9] = Model::CreateFromOBJ("garakuta10");
}

bool WallManager::Initialize()
{
	//モデル読み込み
	LoadModel();

	//乱数生成
	std::random_device rand;
	std::mt19937 mt(rand());

	//モデルの番号
	int modelNum = 0;

	//壁オブジェクトの生成
	object.resize((int)status.wallNum);
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr) = WallObject::Create(model[modelNum % 10]);
		(*itr)->SetPosition({ 0.0f,0.0f,-500.0f });
		(*itr)->SetScale({ 5.0f,5.0f,5.0f });
		modelNum++;
	}

	//演出のため始めのイテレータを保存しておく
	nowItr = object.begin();

	//最初のHPを設定
	maxHP = baseMaxHP;
	status.hp = maxHP;

	return true;
}

void WallManager::SetUpEffect()
{
	//定位置への移動セット
	if (isSetEffect == 1)
	{
		effectTime++;

		//0以外ならオブジェクトをセットしない
		if (effectTime % 8 != 0) { return; }

		XMFLOAT2 maxPosition = WallObject::GetWallMaxPosition();
		XMFLOAT2 minPosition = WallObject::GetWallMinPosition();

		for (int i = 0; i < 4; i++)
		{
			WallObject::STATE state;
			DirectX::XMFLOAT3 lerpEnd = {};
			if (i == 0)
			{
				state = WallObject::STATE::DIRECTING_LEFT_UP;
				lerpEnd = { minPosition.x,maxPosition.y,0 };
			} else if (i == 1)
			{
				state = WallObject::STATE::DIRECTING_RIGHT_UP;
				lerpEnd = { maxPosition.x,maxPosition.y,0 };
			} else if (i == 2)
			{
				state = WallObject::STATE::DIRECTING_RIGHT_DOWN;
				lerpEnd = { maxPosition.x,minPosition.y,0 };
			} else if (i == 3)
			{
				state = WallObject::STATE::DIRECTING_LEFT_DOWN;
				lerpEnd = { minPosition.x,minPosition.y,0 };
			}

			//情報のセット
			(*nowItr)->SetState(state);
			(*nowItr)->SetLerpEndPosition(lerpEnd);

			//イテレータとカウントを進める
			nowItr++;

			//全てセットし終えたらセット終了
			if (nowItr == object.end())
			{
				isSetEffect = 2;
				nowItr--;
				effectTime = 0;
				continue;
			}
		}
	}

	//ちりばめ
	else if (isSetEffect == 2)
	{
		//リストの最後のオブジェクトが移動し終えたら次に行く
		if ((*nowItr)->GetTime() < 150) { return; }

		//乱数準備
		std::random_device rnd;
		std::mt19937 mt(rnd());

		//変化量のセット
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetState(WallObject::STATE::DISPERSE);
			(*itr)->SetDisperseMovePos({ (float)(mt() % 10) - 5,(float)(mt() % 10) - 5 ,(float)(mt() % 10) - 5 });
			(*itr)->SetDisperseMoveRota({ (float)(mt() % 360),(float)(mt() % 10) ,(float)(mt() % 10) });
		}

		//演出フラグとイテレータの初期化
		isSetEffect = 0;
		nowItr = object.begin();
		status.isAlive = true;
	}
}

void WallManager::BreakTime()
{
	//休憩時間
	const int breakTime = 80;

	//タイマーを更新
	breakTimer++;

	//タイマーが指定した時間になったら
	if (breakTimer >= breakTime)
	{
		//壁生成状態にする
		SetCreateWall();

		//休憩終了
		isBreakTime = false;
	}
}

void WallManager::CreateWall()
{
	////壁生成時間
	//const int createTime = 100;

	////壁生成タイマーを更新
	//createTimer++;

	////イージング計算用の時間
	//float easeTimer = (float)createTimer / createTime;

	////イージングで壁オブジェクトの大きさ変更
	//XMFLOAT3 scale = wallObject->GetScale();
	//scale.x = Easing::OutQuint(createStartScale.x, createEndScale.x, easeTimer);
	//scale.y = Easing::OutQuint(createStartScale.y, createEndScale.y, easeTimer);
	////壁オブジェクトの大きさを更新
	//wallObject->SetScale(scale);

	////タイマーが指定した時間になったら
	//if (createTimer >= createTime)
	//{
	//	//HP最大時の色にセット
	//	wallObject->SetColor({ 0.8f, 0.3f, 0.1f, 1.0f });

	//	//生きている
	//	status.isAlive = true;

	//	//壁生成終了
	//	isCreate = false;
	//}
}

void WallManager::ChangeColor()
{
	////残りHPが最大HPの20%以下の場合
	//if (HP <= maxHP / 5) { wallObject->SetColor({ 0.3f, 0.1f, 0.1f, 1.0f }); }
	////残りHPが最大HPの50%以下の場合
	//else if (HP <= maxHP / 2) { wallObject->SetColor({ 0.5f, 0.2f, 0.2f, 1.0f }); }
}

void WallManager::ChangeResult()
{
	////壁を動かす時間
	//const int changeTime = 100;

	////タイマーを更新
	//changeResultTimer++;

	////イージング計算用の時間
	//float easeTimer = (float)changeResultTimer / changeTime;

	////イージングで壁オブジェクトの大きさ変更
	//XMFLOAT3 scale = wallObject->GetScale();
	//scale.x = Easing::OutQuint(createEndScale.x, 12.8f, easeTimer);
	//scale.y = Easing::OutQuint(createEndScale.y, 13.4f, easeTimer);
	////壁オブジェクトの大きさを更新
	//wallObject->SetScale(scale);

	////タイマーが指定した時間になったら
	//if (changeResultTimer >= changeTime)
	//{
	//	//壁を動かす終了
	//	isChangeResult = false;
	//}
}
