#include "ShockWave.h"
#include "SafeDelete.h"
#include "Easing.h"

ShockWave* ShockWave::Create(Model* waveModel)
{
	//インスタンスを生成
	ShockWave* instance = new ShockWave();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(waveModel)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ShockWave::~ShockWave()
{
	//衝撃波が知っているガル族のリスト解放
	alreadyGaruEnemys.clear();

	//オブジェクト解放
	safe_delete(shockWaveObject);
}

bool ShockWave::Initialize(Model* waveModel)
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

	//色を水色にしておく
	shockWaveObject->SetColor({ 0, 1, 1, 1 });

	//ブルームをかける
	shockWaveObject->SetBloom(true);


	return true;
}

void ShockWave::Update()
{
	//発射中のみ更新
	if (!isAlive) return;

	//衝撃波を広げる
	WaveSpread();

	//オブジェクト更新
	shockWaveObject->Update();
}

void ShockWave::Draw()
{
	//発射中のみ描画
	if (!isAlive) return;

	//オブジェクト更新
	shockWaveObject->Draw();
}

void ShockWave::PlayerWaveStart(XMFLOAT3 position)
{
	//色のセット
	shockWaveObject->SetColor({ 0, 1, 1, 1 });

	//広がる速度をセット
	spreadSpeed = 1.5f;

	//威力を設定
	powerLevel = 1;

	//生存可能時間をセット
	aliveTime = 30;

	//衝撃波発射共通処理
	WaveStartCommon(position, powerLevel);
}

void ShockWave::LitteringWaveStart(XMFLOAT3 position)
{
	//色のセット
	shockWaveObject->SetColor({ 1, 1, 1, 1 });

	//広がる速度をセット
	spreadSpeed = 1.5f;

	//威力を設定
	powerLevel = 1;

	//生存可能時間をセット
	aliveTime = 20;

	//衝撃波発射共通処理
	WaveStartCommon(position, powerLevel);
}

void ShockWave::BigWaveStart(XMFLOAT3 position, int powerLevel)
{
	//色のセット
	if (powerLevel == 1) { shockWaveObject->SetColor({ 0, 0, 1, 1 }); }
	else if (powerLevel == 2) { shockWaveObject->SetColor({ 1, 1, 0, 1 }); }
	else if (powerLevel == 3) { shockWaveObject->SetColor({ 1, 0, 0, 1 }); }
	else { return; }

	//広がる速度をセット
	spreadSpeed = 3.0f;

	//威力を設定(通常が1なので2から始める)
	this->powerLevel = powerLevel + 1;

	//生存可能時間をセット
	aliveTime = 40;

	//衝撃波発射共通処理
	WaveStartCommon(position, powerLevel);
}

void ShockWave::Dead()
{
	//衝撃波が知っているガル族のリスト解放
	alreadyGaruEnemys.clear();

	//衝撃波を発射状態ではなくする
	isAlive = false;
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

void ShockWave::WaveSpread()
{
	//生成からの時間を更新
	aliveTimer++;

	//広がる速度をだんだん遅くしていく
	spreadSpeed -= 0.001f * aliveTimer;
	const float spreadSpeedMin = 0.01f;
	if (spreadSpeed <= spreadSpeedMin)
	{
		spreadSpeed = spreadSpeedMin;
	}

	//どんどん半径を大きくしていく
	XMFLOAT3 scale = shockWaveObject->GetScale();
	scale.x += spreadSpeed;
	scale.y += spreadSpeed;
	shockWaveObject->SetScale(scale);

	//回転させる
	XMFLOAT3 rota = shockWaveObject->GetRotation();
	rota.z++;
	shockWaveObject->SetRotation(rota);


	//イージング計算用の時間
	float easeTimer = (float)aliveTimer / aliveTime;
	//色を薄くしていく
	float alpha = Easing::InCubic(1.0f, 0.1f, easeTimer);
	//更新した色の薄さをセット
	XMFLOAT4 color = shockWaveObject->GetColor();
	color.w = alpha;
	shockWaveObject->SetColor(color);


	//生存時間が指定の時間になったら削除する
	if (aliveTimer >= aliveTime)
	{
		Dead();
	}
}

void ShockWave::WaveStartCommon(XMFLOAT3 position, int powerLevel)
{
	//発射位置を設定
	shockWaveObject->SetPosition(position);
	//大きさを0に戻す
	shockWaveObject->SetScale({ 0, 0, 1 });

	//生成からの時間タイマー初期化
	aliveTimer = 0;
	//発射状態にする
	isAlive = true;
}