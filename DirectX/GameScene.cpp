#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"


const float radian = XM_PI / 180.0f;//ラジアン

GameScene::~GameScene()
{
	//newオブジェクトの破棄
	safe_delete(audio);
	safe_delete(sprite);
	safe_delete(player);
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}
	for (int i = 0; i < enemyNum; i++)
	{
		safe_delete(enemy[i]);
	}
}

void GameScene::Initialize(Input *input, Camera *camera)
{
	// nullptrチェック
	assert(input);
	this->input = input;


	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/amm.jpg");

	//プレイヤー生成
	player = Player::Create(1, { 1280 / 2, 720 / 2 }, { 50, 50 });

	//弾生成
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(1);
	}
	//敵生成
	XMFLOAT2 pos = { 0, 0 };
	for (int i = 0; i < enemyNum; i++)
	{
		pos.x += 100;
		enemy[i] = Zakorin::Create(pos);
	}

	//スプライト生成
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 1020, 965 });

	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);


	//サウンド用
	audio = new Audio();
}

void GameScene::Update(Camera *camera)
{
	//プレイヤー更新
	player->Update();

	//弾発射
	if (input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_RB))
	{
		for (int i = 0; i < playerBulletNum; i++)
		{
			//発射されていない弾を探す
			if (!playerBullet[i]->GetIsAlive())
			{
				//プレイヤーの座標と角度を弾も持つ
				XMFLOAT2 pos = player->GetPosition();
				float rota = player->GetRotation();

				//弾発射
				playerBullet[i]->BulletStart(pos, rota);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}
	}
	//弾更新
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Update();
	}

	//敵更新
	for (int j = 0; j < enemyNum; j++)
	{
		enemy[j]->Update();

		//敵が生きていなければ飛ばす
		if (!enemy[j]->GetIsAlive()) continue;

		//弾と敵の当たり判定
		for (int i = 0; i < playerBulletNum; i++)
		{
			//弾が発射状態でなければ飛ばす
			if (!playerBullet[i]->GetIsAlive()) continue;

			//衝突判定を計算
			XMFLOAT2 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetSize().x / 2;
			XMFLOAT2 enemyPos = enemy[j]->GetPosition();
			float enemySize = enemy[j]->GetSize().x / 2;

			bool isCollision = Collision::CircleCollision(
				bulletPos, bulletSize, enemyPos, enemySize);

			//敵と弾が衝突状態でなければ飛ばす
			if (!isCollision) continue;


			//弾は死亡
			playerBullet[i]->Dead();

			//敵はダメージを喰らう
			int damagePower = playerBullet[i]->GetPower();
			enemy[j]->Damage(damagePower);

			//HPが0以下になったら死亡
			if (enemy[j]->GetHP() <= 0)
			{
				enemy[j]->Dead();
			}
		}
	}


	//スプライト更新
	sprite->Update();
	//デバッグテキスト
	if (enemy[1]->GetDeadNum() == 1)
	{
		DebugText::GetInstance()->Print("DEADNUM : 1", 100, 500);
	}
	else if (enemy[1]->GetDeadNum() == 2)
	{
		DebugText::GetInstance()->Print("DEADNUM : 2", 100, 500);
	}

	DebugText::GetInstance()->Print("PlayerMove:LSTICK", 1000, 100);
	DebugText::GetInstance()->Print("BulletAngle:RSTICK", 1000, 150);
	DebugText::GetInstance()->Print("BulletShot:RB", 1000, 200);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//スプライト描画
	Sprite::PreDraw(cmdList);

	//弾描画
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Draw();
	}
	//プレイヤー描画
	player->Draw();

	//敵描画
	for (int i = 0; i < enemyNum; i++)
	{
		enemy[i]->Draw();
	}

	sprite->Draw();


	//デバッグテキスト描画
	DebugText::GetInstance()->DrawAll(cmdList);
	Sprite::PostDraw();
}
