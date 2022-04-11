#include "ShockWave.h"
#include "SafeDelete.h"

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
	//衝撃波を広げる
	if (isSpreadWave)
	{
		WaveSpread();
	}

	//オブジェクト更新
	shockWaveObject->Update();
}

void ShockWave::Draw()
{
	//オブジェクト更新
	shockWaveObject->Draw();
}

void ShockWave::Reset()
{
	//衝撃波円の半径初期化
	radius = 0;
	//衝撃が広がる速度初期化
	spreadSpeed = 0;
	//衝撃が広がる速度の速度初期化
	spreadSpeedAccle = 0;
	//衝撃波を広げない
	isSpreadWave = false;
	//オブジェクト更新
	shockWaveObject->Update();
}

void ShockWave::SetWaveStart(XMFLOAT3 startPos)
{
	//座標をセット
	position = startPos;
	//座標を引数の発射地点に持っていく
	shockWaveObject->SetPosition(position);

	//半径を0に初期化
	radius = 0;
	//大きさを0にする
	shockWaveObject->SetScale({ radius, radius, 1 });

	//広がる速度を初期化しておく
	spreadSpeed = 1.0f;

	//広がる速度の速度を初期化しておく
	spreadSpeedAccle = 0;

	//衝撃波を広げる状態にセット
	isSpreadWave = true;
}

void ShockWave::WaveSpread()
{
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
}
