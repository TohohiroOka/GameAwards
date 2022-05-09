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
	else if (status.isCreate)
	{
		CreateWall();
	}

	//オブジェクト更新
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Update();
	}
}

void WallManager::Draw()
{
	if (isBreakTime) { return; }

	int i = 0;

	//オブジェクト描画
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Draw();
		i++;
	}
}

void WallManager::Reset()
{
	//オブジェクト個数
	status.wallNum = WALL_STEP::step1;
	//壁の最大HP
	status.maxHP = baseMaxHP;
	//壁のHP
	status.hp = status.maxHP;
	//休憩中か
	isBreakTime = true;
	//壁生成回数
	createCount = 0;
	//壁生成中か
	status.isCreate = false;
	//リザルトシーン用に動かしす時間タイマー
	changeResultTimer = 0;
	//壁をリザルトシーン用に動かしているか
	isChangeResult = false;
	//壊されたか
	isBreak = false;
	//生きているか
	status.isAlive = false;
	//オブジェクト単体の初期化
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Reset();
	}
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
		//破壊する
		isBreak = true;

		//休憩状態にする
		isBreakTime = true;

		//生きていない
		status.isAlive = false;
	}
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
	endItr = object.end();
	endItr--;

	//最初のHPを設定
	status.maxHP = baseMaxHP;
	status.hp = status.maxHP;

	return true;
}

void WallManager::SetUpEffect()
{
	//定位置への移動セット
	if (!(isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START) &&
		!(isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_PLAY)) {
		return;
	}

	float time = 50.0f;
	if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
	{
		time = 150;
	}
	WallObject::SetEffectTime(time);

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
		(*nowItr)->Reset();
		(*nowItr)->SetState(state);
		(*nowItr)->SetLerpEndPosition(lerpEnd);

		//イテレータとカウントを進める
		nowItr++;

		//全てセットし終えたらセット終了
		if (nowItr == object.end())
		{
			//使用した値の初期化
			nowItr = object.begin();
			effectTime = 0;
			isSetEffect = EFFECT_NUM::WAIT;
			status.isAlive = true;

			continue;
		}
	}
}

void WallManager::BreakTime()
{
	//消滅状態ならまだ生成しない
	if ((*endItr)->SetState() == WallObject::STATE::TRANSPARENCY) { return; }

	//最大HPを設定
	createCount++;
	status.maxHP = baseMaxHP + (10 * createCount);
	status.hp = status.maxHP;

	//壁生成状態にする
	status.isCreate = true;

	//プレイ前状態
	if (isSetEffect == EFFECT_NUM::NONE)
	{
		isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_START;
	} else {
		isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_PLAY;
	}

	//休憩終了
	isBreakTime = false;
}

void WallManager::CreateWall()
{
	SetUpEffect();

	//タイマーが指定した時間になったら
	if ((*endItr)->GetTime() > WallObject::GetEffectTime())
	{
		//復活
		status.wallNum = WALL_STEP::step1;

		//生きている
		status.isAlive = true;

		//壁生成終了
		status.isCreate = false;
	}
}

void WallManager::ChangeColor()
{
	//残りHPが最大HPの20%以下の場合
	if (status.hp <= status.maxHP / 5)
	{
		status.wallNum = WALL_STEP::step2;
		int num = 0;
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			if (num > (int)WALL_STEP::step2)
			{
				(*itr)->SetState(WallObject::STATE::TRANSPARENCY);
			}
			num++;
		}
	}
	//残りHPが最大HPの50%以下の場合
	if (status.hp <= status.maxHP / 2)
	{
		status.wallNum = WALL_STEP::step3;
		int num = 0;
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			if (num > (int)WALL_STEP::step3 && num < (int)WALL_STEP::step2)
			{
				(*itr)->SetState(WallObject::STATE::TRANSPARENCY);
			}
			num++;
		}
	}
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
