#include "ShockWave.h"
#include "SafeDelete.h"
#include "Easing.h"

ShockWave* ShockWave::Create(Model* waveModel, XMFLOAT3 position, int power)
{
	//インスタンスを生成
	ShockWave* instance = new ShockWave();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(waveModel, position, power)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ShockWave::~ShockWave()
{
	//オブジェクト解放
	safe_delete(shockWaveObject);

	//衝撃波が知っているガル族のリスト解放
	alreadyGaruEnemys.clear();
	//衝撃波が知っているチャロのリスト解放
	alreadyCharoEnemys.clear();
	//衝撃波が知っているポルタのリスト解放
	alreadyPortaEnemys.clear();
}

bool ShockWave::Initialize(Model* waveModel, XMFLOAT3 position, int power)
{
	//衝撃波オブジェクト生成
	shockWaveObject = Object3d::Create();
	if (shockWaveObject == nullptr) {
		return false;
	}

	//モデルをセット
	if (waveModel) {
		shockWaveObject->SetModel(waveModel);
	}

	//座標をセット
	this->position = position;
	shockWaveObject->SetPosition(position);

	//色を水色にしておく
	shockWaveObject->SetColor({ 0, 1, 1, 1 });

	//ブルームをかける
	shockWaveObject->SetBloom(true);

	//攻撃力を設定
	this->power = power;

	return true;
}

void ShockWave::Update()
{
	//衝撃波を広げる
	WaveSpread();

	//オブジェクト更新
	shockWaveObject->Update();
}

void ShockWave::Draw()
{
	//オブジェクト更新
	shockWaveObject->Draw();
}

void ShockWave::SetDelete()
{
	//削除する
	isDelete = true;
}

bool ShockWave::IsKnowGaruEnemy(GaruEnemy* garuEnemy)
{
	//引数のガル族を既に知っているか確認
	for (auto itr = alreadyGaruEnemys.begin(); itr != alreadyGaruEnemys.end(); itr++)
	{
		//既に知っていたらtrueを返す
		if (garuEnemy == (*itr))
		{
			return true;
		}
	}

	//全て確認しても知らなかったら新たに追加する
	alreadyGaruEnemys.push_front(garuEnemy);

	//知らなかった場合はfalse
	return false;
}

bool ShockWave::IsKnowCharo(Charo* charo)
{
	//引数のチャロを既に知っているか確認
	for (auto itr = alreadyCharoEnemys.begin(); itr != alreadyCharoEnemys.end(); itr++)
	{
		//既に知っていたらtrueを返す
		if (charo == (*itr))
		{
			return true;
		}
	}

	//全て確認しても知らなかったら新たに追加する
	alreadyCharoEnemys.push_front(charo);

	//知らなかった場合はfalse
	return false;
}

bool ShockWave::IsKnowPorta(Porta* porta)
{
	//引数のポルタを既に知っているか確認
	for (auto itr = alreadyPortaEnemys.begin(); itr != alreadyPortaEnemys.end(); itr++)
	{
		//既に知っていたらtrueを返す
		if (porta == (*itr))
		{
			return true;
		}
	}

	//全て確認しても知らなかったら新たに追加する
	alreadyPortaEnemys.push_front(porta);

	//知らなかった場合はfalse
	return false;
}

void ShockWave::WaveSpread()
{
	//生存できる時間
	const int aliveTime = 20 + power;

	//生成からの時間を更新
	aliveTimer++;

	//広がる速度の速度をだんだん速くしていく
	spreadSpeedAccle += 0.001f;
	//広がる速度をだんだん速くしていく
	spreadSpeed += spreadSpeedAccle;
	//どんどん半径を大きくしていく
	radius += spreadSpeed;
	shockWaveObject->SetScale({ radius + 0.1f, radius + 0.1f, 1 });


	//回転させる
	XMFLOAT3 rota = shockWaveObject->GetRotation();
	rota.z++;
	shockWaveObject->SetRotation(rota);


	//イージング計算用の時間
	float easeTimer = (float)aliveTimer / aliveTime;
	//色を薄くしていく
	float alpha = Easing::InCubic(1.0f, 0.1f, easeTimer);
	//更新した色の薄さをセット
	shockWaveObject->SetColor({ 1, 1, 1, alpha });


	//生存時間が指定の時間になったら削除する
	if (aliveTimer >= aliveTime)
	{
		SetDelete();
	}
}