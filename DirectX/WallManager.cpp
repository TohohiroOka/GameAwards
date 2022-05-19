#include "WallManager.h"
#include "SafeDelete.h"
#include "Easing.h"
#include <random>
#include <cstdlib>
#include "StageEffect.h"
#include "XInputManager.h"
#include "Audio.h"

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
	Audio* audio = Audio::GetInstance();

	//壁が破壊され、壁を生成していない場合
	if (!status.isCreate && !status.isAlive)
	{
		//消滅状態ならまだ生成しない
		if (!((*endItr)->GetState() == WallObject::STATE::TRANSPARENCY))
		{
			//サウンドの再生
			audio->SoundPlayWava(sound[0], false);

			//壁生成状態にする
			status.isCreate = true;

			isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_PLAY;
		}
	}
	//壁が破壊され、壁を生成している場合
	else if (status.isCreate)
	{
		CreateWall();
	}

	if (isSetEffect == EFFECT_NUM::OUT_SCREEN)
	{
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetState(WallObject::STATE::OUT_SCREEN);
		}

		isSetEffect = EFFECT_NUM::WAIT;
	}

	//オブジェクト更新
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Update();
	}
}

void WallManager::Draw()
{
	//オブジェクト描画
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr)->Draw();
	}
}

void WallManager::Reset(bool allReset)
{
	//オブジェクト個数
	status.wallNum = WALL_STEP::step1;
	//壁生成フラグ
	isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_START;
	if (!allReset)
	{
		isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_PLAY;
	}
	//壁の最大HP
	status.maxHP = baseMaxHP;
	//壁のHP
	status.hp = status.maxHP;
	//壁生成回数
	breakCount = 0;
	//壁生成中か
	status.isCreate = true;
	//壊されたか
	status.isBreak = false;
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
	Audio* audio = Audio::GetInstance();

	//引数で指定した強さの分HPを減らす
	status.hp -= damagePower;

	//残りHPに応じて色変更
	PercentageDestruction();

	//HPが0以下になったら破壊
	if (status.hp <= 0)
	{
		//サウンドの再生
		audio->SoundPlayWava(sound[1], false);

		//破壊する
		status.isBreak = true;

		//生きていない
		status.isAlive = false;
	}
}

bool WallManager::GetTriggerBreak()
{
	//破壊されていたら
	if (status.isBreak)
	{
		//トリガーなのでfalseに戻す
		status.isBreak = false;

		return true;
	}

	return false;
}

void WallManager::SetHitEffect(XMFLOAT3 enemyPos)
{
	XMFLOAT2 maxPosition = WallObject::GetWallMaxPosition();
	XMFLOAT2 minPosition = WallObject::GetWallMinPosition();

	XMFLOAT2 range = { maxPosition.x + enemyPos.x,maxPosition.y + enemyPos.y };
	if (range.x > minPosition.x + enemyPos.x)
	{
		range.x = minPosition.x + enemyPos.x;
	}
	if (range.y > minPosition.y + enemyPos.y)
	{
		range.y = minPosition.y + enemyPos.y;
	}

	//ラジアン
	float radius = atan2f(range.y - enemyPos.y, range.x - enemyPos.x);
	//角度
	float angle = DirectX::XMConvertToDegrees(radius);


	StageEffect::SetHitWall(enemyPos, angle);
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
	Audio* audio = Audio::GetInstance();

	//モデル読み込み
	LoadModel();

	//壁オブジェクトの生成
	const XMFLOAT2 min = WallObject::GetWallMinPosition();
	const XMFLOAT2 max = WallObject::GetWallMaxPosition();

	//モデルの番号
	int modelNum = 0;

	//オブジェクト配列のサイズ変更
	object.resize((int)status.wallNum);
	for (auto itr = object.begin(); itr != object.end(); itr++)
	{
		(*itr) = WallObject::Create(model[modelNum % 10]);
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

	//初回の壁生成
	status.isCreate = true;
	isSetEffect = EFFECT_NUM::SET_FIXED_POSITION_START;

	//サウンドの読み込み
	sound[0] = audio->SoundLoadWave("Resources/sound/wallSet.wav");//壁生成音
	sound[1] = audio->SoundLoadWave("Resources/sound/wallBreak.wav");//壁破壊音

	//サウンドの再生
	audio->SoundPlayWava(sound[0], false);

	return true;
}

void WallManager::SetUpEffect()
{
	effectTime++;

	//0以外ならオブジェクトをセットしない
	if (effectTime % 4 != 0) { return; }

	XMFLOAT2 maxPosition = WallObject::GetWallMaxPosition();
	XMFLOAT2 minPosition = WallObject::GetWallMinPosition();

	for (int i = 0; i < 8; i++)
	{
		//上下で68
		//32
		WallObject::STATE state = WallObject::STATE::NONE;
		DirectX::XMFLOAT3 startPos = {};

		//上下と左右で個数を2 : 1にする
		if (createCount % 2 == 1) { continue; }

		//左上
		if (i == 0 || i == 1)
		{
			startPos = { minPosition.x,maxPosition.y,0 };
			if (i % 2 == 0)
			{
				state = WallObject::STATE::MOVE_UP_RIGHT;
				if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
				{
					startPos.y += 8.0f;
				}
			} else
			{
				state = WallObject::STATE::MOVE_LEFT_DOWN;
			}
		}
		//左下
		else if (i == 2 || i == 3)
		{
			startPos = { minPosition.x,minPosition.y,0 };
			if (i % 2 == 0)
			{
				state = WallObject::STATE::MOVE_DOWN_RIGHT;
				if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
				{
					startPos.y += 8.0f;
				}
			} else
			{
				state = WallObject::STATE::MOVE_LEFT_UP;
			}
		}
		//右上
		else if (i == 4 || i == 5)
		{
			startPos = { maxPosition.x,maxPosition.y,0 };
			if (i % 2 == 0)
			{
				state = WallObject::STATE::MOVE_UP_LEFT;
				if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
				{
					startPos.y += 8.0f;
				}
			} else
			{
				state = WallObject::STATE::MOVE_RIGHT_DOWN;
			}
		}
		//右下
		else if (i == 6 || i == 7)
		{
			startPos = { maxPosition.x,minPosition.y,0 };
			if (i % 2 == 0)
			{
				state = WallObject::STATE::MOVE_DOWN_LEFT;
				if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START)
				{
					startPos.y += 8.0f;
				}
			} else
			{
				state = WallObject::STATE::MOVE_RIGHT_UP;
			}
		}

		//情報のセット
		(*nowItr)->Reset();
		(*nowItr)->SetState(state);
		(*nowItr)->SetPosition(startPos);

		//イテレータとカウントを進める
		nowItr++;

		//全てセットし終えたらセット終了
		if (nowItr == object.end())
		{
			//使用した値の初期化
			nowItr = object.begin();
			effectTime = 0;
			isSetEffect = EFFECT_NUM::WAIT;
			createCount = 0;
			return;
		}
	}

	createCount++;
}

void WallManager::CreateWall()
{
	//定位置への移動セット
	if (isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_START ||
		isSetEffect == EFFECT_NUM::SET_FIXED_POSITION_PLAY)
	{
		SetUpEffect();
	}

	//最初のオブジェクトが定位置に着いたら壁になる
	WallObject::STATE objState = (*endItr)->GetState();
	if (objState >= WallObject::STATE::MOVE_UP_LEFT && objState <= WallObject::STATE::MOVE_LEFT_UP)
	{
		WallObject::SetSlow(true);
	}

	//全てセットし終えたら
	if ((*endItr)->GetState() == WallObject::STATE::WAIT)
	{
		//最大HPを設定
		breakCount++;
		status.maxHP = baseMaxHP + (10 * breakCount);
		status.hp = status.maxHP;

		//復活
		status.wallNum = WALL_STEP::step1;

		//生きている
		status.isAlive = true;

		//壁生成終了
		status.isCreate = false;
	}
}

void WallManager::PercentageDestruction()
{
	//残りHPが最大HPの20%以下の場合
	if (status.hp <= status.maxHP / 5)
	{
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetColor({ 0.2f,1.0f,1.0f,1.0f });
		}
	}
	//残りHPが最大HPの50%以下の場合
	if (status.hp <= status.maxHP / 2)
	{
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetColor({ 1.0f,0.1f,0.1f,1.0f });
		}
	}
	//残りHPが0の場合
	if (status.hp <= 0)
	{
		for (auto itr = object.begin(); itr != object.end(); itr++)
		{
			(*itr)->SetState(WallObject::STATE::TRANSPARENCY);
		}

		//振動
		XInputManager* Xinput = XInputManager::GetInstance();
		XInputManager::STRENGTH strength = XInputManager::STRENGTH::SMALL;
		Xinput->StartVibration(XInputManager::STRENGTH::LARGE, 20);
		Xinput = nullptr;

	}
}