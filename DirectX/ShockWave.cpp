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
	//衝撃波が知っている敵のリスト解放
	alreadyEnemys.clear();

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

void ShockWave::Reset()
{
	//衝撃が広がる速度
	spreadSpeed = 0;
	//威力段階
	powerLevel = 0;
	//生成からの時間タイマー
	aliveTimer = 0;
	//生存可能時間
	aliveTime = 0;
	//生きてるか
	isAlive = false;
	//衝撃波が知っている敵のリスト解放
	alreadyEnemys.clear();

	//大きさを0に戻す
	shockWaveObject->SetScale({ 0, 0, 1 });
}

void ShockWave::ShockWaveStart(XMFLOAT3 position, int powerLevel)
{
	//色のセット
	shockWaveObject->SetColor({ 0, 1, 1, 1 });

	//広がる速度と生存可能時間をセット
	if (powerLevel == 1) { spreadSpeed = 3.0f; aliveTime = 18; }
	else if (powerLevel == 2) { spreadSpeed = 3.5f; aliveTime = 20; }
	else if (powerLevel == 3) { spreadSpeed = 4.0f; aliveTime = 22; }
	else { return; }

	//威力を設定
	this->powerLevel = powerLevel;

	//発射位置を設定
	shockWaveObject->SetPosition(position);
	//大きさを0に戻す
	shockWaveObject->SetScale({ 0, 0, 1 });
	//威力倍率を1も戻す
	powerMagnification = 1.0f;
	//生成からの時間タイマー初期化
	aliveTimer = 0;
	//発射状態にする
	isAlive = true;
}

void ShockWave::Dead()
{
	//衝撃波が知っている敵のリスト解放
	alreadyEnemys.clear();

	//衝撃波を発射状態ではなくする
	isAlive = false;
}

bool ShockWave::IsKnowEnemy(BaseEnemy* enemy)
{
	//引数の敵を既に知っているか確認
	for (auto itr = alreadyEnemys.begin(); itr != alreadyEnemys.end(); itr++)
	{
		//既に知っていたらtrueを返す
		if (enemy == (*itr))
		{
			return true;
		}
	}

	//全て確認しても知らなかったら新たに追加する
	alreadyEnemys.push_front(enemy);

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
	//威力倍率を低くしていく
	powerMagnification = Easing::Lerp(1.0f, 0.2f, easeTimer);

	if (aliveTimer >= aliveTime / 2)
	{
		//イージング計算用の時間
		float colorTimer = (float)(aliveTimer - aliveTime / 2) / (aliveTime / 2);
		//更新した色の薄さをセット
		XMFLOAT4 color = shockWaveObject->GetColor();
		//色を薄くしていく
		color.w = Easing::OutQuint(1.0f, 0.2f, colorTimer);
		shockWaveObject->SetColor(color);
	}

	//生存時間が指定の時間になったら削除する
	if (aliveTimer >= aliveTime)
	{
		Dead();
	}
}