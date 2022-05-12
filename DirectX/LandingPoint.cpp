#include "LandingPoint.h"
#include "SafeDelete.h"
#include "Easing.h"

DirectX::XMFLOAT2 LandingPoint::moveRangeMin = {};
DirectX::XMFLOAT2 LandingPoint::moveRangeMax = {};

LandingPoint* LandingPoint::Create(Model* model)
{
	//インスタンスを生成
	LandingPoint* instance = new LandingPoint();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

LandingPoint::~LandingPoint()
{
	for (int i = 0; i < pointNum; i++)
	{
		safe_delete(pointCircleObject[i]);
	}
}

bool LandingPoint::Initialize(Model* model)
{
	for (int i = 0; i < pointNum; i++)
	{
		//オブジェクト生成
		pointCircleObject[i] = Object3d::Create(model);
		if (pointCircleObject[i] == nullptr) {
			return false;
		}

		//大きさをセット
		pointCircleObject[i]->SetScale({ 4, 4, 1 });
	}
	return true;
}

void LandingPoint::Update(XMFLOAT3 playerPosition, XMFLOAT3 playerRotation)
{
	//生成中
	if (isCreate)
	{
		Create();
	}
	//プレイヤーに追従させる
	if (isChase)
	{
		Chase(playerPosition, playerRotation);
	}

	//オブジェクト更新
	for (int i = 0; i < pointNum; i++)
	{
		pointCircleObject[i]->Update();
	}
}

void LandingPoint::Draw()
{
	//オブジェクト描画
	for (int i = 0; i < pointNum; i++)
	{
		//生きていなければ描画しない
		if (!isAlive[i]) { continue; }
		pointCircleObject[i]->Draw();
	}
}

void LandingPoint::Dead(int pointNum)
{
	//死亡させる
	isAlive[pointNum] = false;
}

void LandingPoint::Revive()
{
	//全て生き返る
	for (int i = 0; i < pointNum; i++)
	{
		isAlive[i] = true;
	}

	//追従状態を戻しておく
	isChase = true;

	//生成タイマーを初期化
	createTimer = 0;
	//生成状態にする
	isCreate = true;
}

bool LandingPoint::CheckAllDead()
{
	for (int i = 0; i < pointNum; i++)
	{
		//一つでも生きていればfalse
		if (isAlive[i]) { return false; }
	}

	//全て死んでいたらtrue
	return true;
}

void LandingPoint::Create()
{
	//生成を行う時間
	const int createTime = 30;

	//タイマー更新
	createTimer++;

	//イージング計算用の時間
	float easeTimer = (float)createTimer / createTime;

	for (int i = 0; i < pointNum; i++)
	{
		//サイズを変更
		XMFLOAT3 scale = pointCircleObject[i]->GetScale();
		const float baseScale = 4.0f;
		scale.x = Easing::OutQuad(0, baseScale, easeTimer);
		scale.y = Easing::OutQuad(0, baseScale, easeTimer);
		//更新したサイズをセット
		pointCircleObject[i]->SetScale(scale);
	}

	//タイマーが指定した時間になったら
	if (createTimer >= createTime)
	{
		//生成終了
		isCreate = false;
	}
}

void LandingPoint::Chase(XMFLOAT3 position, XMFLOAT3 rotation)
{
	for (int i = 0; i < pointNum; i++)
	{
		//プレイヤー角度を追従する
		XMFLOAT3 rota = rotation;
		pointCircleObject[i]->SetRotation(rota);

		float angle = DirectX::XMConvertToRadians(rota.z + 90 + (-30 + i * 30));

		//プレイヤーから離れた距離に設置する
		XMFLOAT3 pos;
		float distance = 60.0f;

		//永遠ループを避けるためにカウンターを用意
		int count = 0;
		//壁に当たっていたら回し続ける
		bool isRoopEnd = false;
		while (!isRoopEnd)
		{
			//プレイヤーから一定距離離す
			pos = position;
			pos.x += distance * cosf(angle);
			pos.y += distance * sinf(angle);

			bool isCollisionWall = false;
			if (pos.x <= moveRangeMin.x) { isCollisionWall = true; }
			else if (pos.x >= moveRangeMax.x) { isCollisionWall = true; }
			if (pos.y <= moveRangeMin.y) { isCollisionWall = true; }
			else if (pos.y >= moveRangeMax.y) { isCollisionWall = true; }

			//壁に当たっていたら距離を詰める
			if (isCollisionWall)
			{
				distance--;
			}
			//壁に当たっていなかったらループ終了
			else
			{
				isRoopEnd = true;
			}

			//永遠ループを防ぐために100回ループしたら終了
			count++;
			if (count >= 100)
			{
				isRoopEnd = true;
			}
		}

		//更新した座標をセット
		pointCircleObject[i]->SetPosition(pos);
	}
}
