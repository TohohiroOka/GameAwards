#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"
#include "Hageta.h"
#include "Hagetata.h"
#include "EnemyCircle.h"
#include "StartSetCircle.h"

const float radian = XM_PI / 180.0f;//ラジアン

GameScene::~GameScene()
{
	//newオブジェクトの破棄
	safe_delete(audio);
	safe_delete(light);
	safe_delete(sprite);

	//モデル解放
	safe_delete(circleModel);
	safe_delete(playerModel);
	safe_delete(pBodyModel);
	safe_delete(pHead01Model);
	safe_delete(pHead02Model);
	safe_delete(pHead03Model);
	safe_delete(pBullModel);
	safe_delete(enemy01Model);
	safe_delete(enemyPoint01Model);
	safe_delete(enemy02Model);
	safe_delete(enemyPoint02Model);
	safe_delete(eBullModel);
	safe_delete(deadEnemyModel);
	safe_delete(hexagonModel);
	safe_delete(happyModel);

	//プレイヤー解放
	safe_delete(player);

	//プレイヤー弾解放
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}

	//レーザーサイト解放
	safe_delete(laserSite);

	//ガル族解放
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		safe_delete(*itrGaruEnemy);
	}
	//ガル族のリスト解放
	garuEnemys.clear();

	//ハゲ族解放
	for (auto itrHageEnemy = hageEnemys.begin(); itrHageEnemy != hageEnemys.end(); itrHageEnemy++)
	{
		safe_delete(*itrHageEnemy);
	}
	//ハゲ族のリスト解放
	hageEnemys.clear();

	//敵の弾解放
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
	}

	//固定オブジェクト解放
	for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
	{
		safe_delete(*itrFixedObject);
	}
	//固定オブジェクトのリスト解放
	fixedObjects.clear();

	//コネクトサークル解放
	for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
	{
		safe_delete(*itrConnectCircle);
	}
	//コネクトサークルのリスト解放
	connectCircles.clear();

	//パワーアップ線解放
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		safe_delete(*itrLine);
	}
	//パワーアップ線のリスト解放
	powerUpLines.clear();

	//エフェクトの解放
	safe_delete(effects);
}

void GameScene::Initialize(Camera *camera)
{
	// nullptrチェック
	assert(camera);

	//ライト
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	circleModel = Model::CreateFromOBJ("circle");//タバコのモデル
	playerModel = Model::CreateFromOBJ("player");//プレイヤーのモデル
	pBodyModel = Model::CreateFromOBJ("playerbody");//プレイヤーの体のモデル
	pHead01Model = Model::CreateFromOBJ("playerhead1");//プレイヤーの頭のモデル(HP1)
	pHead02Model = Model::CreateFromOBJ("playerhead2");//プレイヤーの頭のモデル(HP2)
	pHead03Model = Model::CreateFromOBJ("playerhead3");//プレイヤーの頭のモデル(HP3)
	pBullModel = Model::CreateFromOBJ("playerbullet");//プレイヤーの弾のモデル
	enemy01Model = Model::CreateFromOBJ("enemy");//敵01(ガルタ)のモデル
	enemyPoint01Model = Model::CreateFromOBJ("garutaspown");//敵01(ガルタ)の出現位置のモデル
	enemy02Model = Model::CreateFromOBJ("garutata");//敵02(ガルタタ)のモデル
	enemyPoint02Model = Model::CreateFromOBJ("garutataspown");//敵02(ガルタタ)の出現位置のモデル
	eBullModel = Model::CreateFromOBJ("enemybullet");//敵の弾のモデル
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//死んだ敵のモデル
	hexagonModel = Model::CreateFromOBJ("hexagon");//六角形のモデル
	happyModel = Model::CreateFromOBJ("happy");//タバコモデル

	//プレイヤーウエポンのモデルをセット
	Player::SetWeaponModel(pHead01Model, pHead02Model, pHead03Model);
	//プレイヤー生成
	player = Player::Create(pBodyModel);

	//弾生成
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(pBullModel);
	}

	//レーザーサイト生成
	laserSite = LaserSite::Create();

	//敵の弾生成
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//固定オブジェクトをセット
	SetFixedObject();

	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/kari.png");

	//スプライト生成
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 64, 64 });

	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);

	//サウンド用
	audio = new Audio();

	//エフェクト初期化
	effects = new StageEffect();
	effects->Initialize();
}

void GameScene::Update(Camera *camera)
{
	Input *input = Input::GetInstance();
	XInputManager *Xinput = XInputManager::GetInstance();

	//プレイヤー更新
	player->Update(effects);

	//レーザーサイト更新
	laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
	laserSite->Update(camera);

	//デバッグ用線の色
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		(*itrLine)->SetColor({ 0.4f, 1, 0.2f, 1 });
	}

	//プレイヤー弾発射
	bulletShotTimer--;
	if (bulletShotTimer <= 0 && (input->PushKey(DIK_SPACE) || Xinput->PushButton(XInputManager::PAD_RB)))
	{
		//プレイヤーウエポンの座標と角度を弾も持つ
		XMFLOAT3 pos = player->GetWeaponPosition();
		XMFLOAT3 rota = player->GetWeaponRotation();

		//弾の発射をプレイヤーウエポンの真上に設定
		float angle = DirectX::XMConvertToRadians(rota.z + 90);
		pos.x += 8.0f * cosf(angle);
		pos.y += 8.0f * sinf(angle);

		//左側の弾発射
		for (int i = 0; i < playerBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (playerBullet[i]->GetIsAlive()) { continue; }

			//弾の発射位置を左側にずらす
			float angle2 = DirectX::XMConvertToRadians(rota.z + 180);
			XMFLOAT3 shotPosLeft = {};
			shotPosLeft.x = 1.5f * cosf(angle2) + pos.x;
			shotPosLeft.y = 1.5f * sinf(angle2) + pos.y;

			//弾発射
			playerBullet[i]->BulletStart(shotPosLeft, rota);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}

		//右側の弾発射
		for (int i = 0; i < playerBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (playerBullet[i]->GetIsAlive()) { continue; }

			//弾の発射位置を右側にずらす
			float angle2 = DirectX::XMConvertToRadians(rota.z);
			XMFLOAT3 shotPosRight = {};
			shotPosRight.x = 1.5f * cosf(angle2) + pos.x;
			shotPosRight.y = 1.5f * sinf(angle2) + pos.y;

			//弾発射
			playerBullet[i]->BulletStart(shotPosRight, rota);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}

		//次の弾発射までのタイマーを初期化する
		bulletShotTimer = 10;
	}

	//プレイヤー弾更新
	for (int i = 0; i < playerBulletNum; i++)
	{
		//弾が生きていなければ飛ばす
		if (!playerBullet[i]->GetIsAlive()) { continue; }

		//更新処理
		playerBullet[i]->Update();

		//弾が生きていなければ飛ばす
		if (!playerBullet[i]->GetIsAlive()) { continue; }

		//弾とパワーアップ線の当たり判定
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			//衝突用に弾の座標と半径、線の始点と終点を借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletRadius = playerBullet[i]->GetScale().x;
			XMFLOAT3 lineStartPoint = (*itrLine)->GetStartPoint();
			XMFLOAT3 lineEndPoint = (*itrLine)->GetEndPoint();

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Line(
				bulletPos, bulletRadius, lineStartPoint, lineEndPoint);

			//弾と線が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//デバッグ用線の色変更
			(*itrLine)->SetColor({ 1, 0, 0, 1 });

			//既に衝突したことがあるか確認(衝突中パワーアップし続けてしまうため)
			if (playerBullet[i]->IsKnowLine((*itrLine))) { continue; }

			//弾をパワーアップさせる
			playerBullet[i]->PowerUp();
		}
	}

	//ガル族生成
	if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//ガル族をスポーン
		SpawnGaruEnemy();
	}

	//ガル族更新
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		//存在がなかったら飛ばす
		if (!(*itrGaruEnemy)->GetIsExistence()) { continue; }

		//更新処理
		(*itrGaruEnemy)->Update(effects);

		//ノックバックが終わり、存在がなくなったら
		if (!(*itrGaruEnemy)->GetIsExistence())
		{
			//ノックバック終了時の座標で、他の死亡状態のガル族との当たり判定を取る
			for (auto itrGaruEnemy2 = garuEnemys.begin(); itrGaruEnemy2 != garuEnemys.end(); itrGaruEnemy2++)
			{
				//衝突相手が存在を持っていたら飛ばす
				if ((*itrGaruEnemy2)->GetIsExistence()) { continue; }

				//自分自身との当たり判定は行わない
				if (itrGaruEnemy == itrGaruEnemy2) { continue; }

				//衝突用にガル族1とガル族2の座標と半径の大きさを借りる
				XMFLOAT3 enemyPos1 = (*itrGaruEnemy)->GetPosition();
				float enemyRadius1 = (*itrGaruEnemy)->GetScale().x;
				XMFLOAT3 enemyPos2 = (*itrGaruEnemy2)->GetPosition();
				float enemyRadius2 = (*itrGaruEnemy2)->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos1, enemyRadius1, enemyPos2, enemyRadius2);

				//ガル族1とガル族2が衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//ガル族1とガル族2両方削除状態にする
				(*itrGaruEnemy)->SetDelete();
				(*itrGaruEnemy2)->SetDelete();
			}

			//削除状態ならこの先の処理を行わない
			if ((*itrGaruEnemy)->GetIsDelete()) { continue; }

			//ノックバック終了時の座標で、固定オブジェクトと当たり判定を取る
			for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
			{
				//衝突用にガル族と固定オブジェクトの座標と半径の大きさを借りる
				XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
				float enemyRadius = (*itrGaruEnemy)->GetScale().x;
				XMFLOAT3 fixedObjectPos = (*itrFixedObject)->GetPosition();
				float fixedObjectRadius = (*itrFixedObject)->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemyRadius, fixedObjectPos, fixedObjectRadius);

				//ガル族と固定オブジェクトが衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//削除状態にする
				(*itrGaruEnemy)->SetDelete();
			}

			//削除状態ならこの先の処理を行わない
			if ((*itrGaruEnemy)->GetIsDelete()) { continue; }

			//オブジェクトのモデルを変更する
			(*itrGaruEnemy)->SetModel(deadEnemyModel);

			//コネクトサークルの半径をセットする（ ガル族の大きさ×（ 倒された時の弾の強さ / 4 ））
			float radius = (*itrGaruEnemy)->GetScale().x * ((float)(*itrGaruEnemy)->GetKillBulletPower() / 4);
			connectCircles.push_back(
				EnemyCircle::Create(circleModel, *itrGaruEnemy, radius));

			//ガル族の存在がなくなったので飛ばす
			continue;
		}

		//ガル族が生きていなければ飛ばす ガル族がスポーン中だったら飛ばす ガル族が逃走中だったら飛ばす
		if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

		//弾発射フラグがtrueなら
		if ((*itrGaruEnemy)->GetIsBulletShot())
		{
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//弾が発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//ガル族の座標と標的の座標（プレイヤーの座標）を弾も持つ
				XMFLOAT3 startPos = (*itrGaruEnemy)->GetPosition();
				XMFLOAT3 targetPos = player->GetPosition();

				//弾発射
				enemyBullet[i]->BulletStart(startPos, targetPos);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}

		//プレイヤーとガル族の当たり判定
		//プレイヤーがダメージ状態でないなら判定をする
		if (!player->GetIsDamege())
		{
			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//ガル族とプレイヤーが衝突状態
			if (isCollision)
			{
				//プレイヤーはダメージを喰らう
				player->Damage();
				player->SetKnockback();

				isShake = true;

				//HPが0なら
				if (player->GetHP() <= 0)
				{
					//プレイヤー死亡
					player->Dead();
				}
			}
		}

		//弾とガル族の当たり判定
		for (int i = 0; i < playerBulletNum; i++)
		{
			//弾が発射状態でなければ飛ばす
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;
			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//ガル族と弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//弾は死亡
			playerBullet[i]->Dead();

			//ガル族はダメージを喰らう
			int bulletPower = playerBullet[i]->GetPower();
			(*itrGaruEnemy)->Damage(bulletPower);

			//ダメージを喰らってもHPが残っていたら飛ばす
			if ((*itrGaruEnemy)->GetHP() > 0) { continue; }

			//ガル族のHPが0以下なのでノックバックを開始する
			float bulletAngle = playerBullet[i]->GetAngle();
			(*itrGaruEnemy)->SetKnockBack(bulletAngle, bulletPower);
		}
	}

	//ガル族削除
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrGaruEnemy)->GetIsDelete())
		{
			//コネクトサークルが削除するガル族を使用しているか確認
			for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
			{
				//使用していたらコネクトサークルを削除状態にセット
				if ((*itrConnectCircle)->CheckUseEnemy(*itrGaruEnemy))
				{
					(*itrConnectCircle)->SetDelete();
				}
			}

			//要素を削除、リストから除外する
			safe_delete(*itrGaruEnemy);
			itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
			continue;
		}
		//for分を回す
		itrGaruEnemy++;
	}

	//ハゲ族生成
	if (input->TriggerKey(DIK_LSHIFT) || Xinput->TriggerButton(XInputManager::PAD_LT))
	{
		//ハゲ族をスポーン
		SpawnHageEnemy();
	}

	//ハゲ族更新
	for (auto itrHageEnemy = hageEnemys.begin(); itrHageEnemy != hageEnemys.end(); itrHageEnemy++)
	{
		//更新処理
		(*itrHageEnemy)->Update();

		//弾発射フラグがtrueなら
		if ((*itrHageEnemy)->GetIsBulletShot())
		{
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//弾が発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//ハゲ族の座標と標的の座標（プレイヤーの座標）を弾も持つ
				XMFLOAT3 startPos = (*itrHageEnemy)->GetPosition();
				XMFLOAT3 targetPos = player->GetPosition();

				//弾発射
				enemyBullet[i]->BulletStart(startPos, targetPos);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}

		//プレイヤーとハゲ族の当たり判定
		//プレイヤーがダメージ状態でないなら判定をする
		if (!player->GetIsDamege())
		{
			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 enemyPos = (*itrHageEnemy)->GetPosition();
			float enemySize = (*itrHageEnemy)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//プレイヤーと弾が衝突状態
			if (isCollision)
			{
				//プレイヤーはダメージを喰らう
				player->Damage();
				player->SetKnockback();

				isShake = true;

				//HPが0なら
				if (player->GetHP() <= 0)
				{
					//プレイヤー死亡
					player->Dead();
				}
			}
		}

		//弾とハゲ族の当たり判定
		for (int i = 0; i < playerBulletNum; i++)
		{
			//弾が発射状態でなければ飛ばす
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;
			XMFLOAT3 enemyPos = (*itrHageEnemy)->GetPosition();
			float enemySize = (*itrHageEnemy)->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//ハゲ族と弾が衝突状態でなければ飛ばす
			if (!isCollision) { continue; }

			//弾は死亡
			playerBullet[i]->Dead();

			//ハゲ族はダメージを喰らう
			int bulletPower = playerBullet[i]->GetPower();
			(*itrHageEnemy)->Damage(bulletPower);

			//ダメージを喰らってもHPが残っていたら飛ばす
			if ((*itrHageEnemy)->GetHP() > 0) { continue; }

			//ハゲ族のHPが0以下なので死亡
			(*itrHageEnemy)->Dead();
		}
	}

	//ハゲ族削除
	for (auto itrHageEnemy = hageEnemys.begin(); itrHageEnemy != hageEnemys.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrHageEnemy)->GetIsDelete())
		{
			//要素を削除、リストから除外する
			safe_delete(*itrHageEnemy);
			itrHageEnemy = hageEnemys.erase(itrHageEnemy);
			continue;
		}
		//for分を回す
		itrHageEnemy++;
	}

	//敵の弾更新
	for (int i = 0; i < enemyBulletNum; i++)
	{
		//弾が生きていなければ飛ばす
		if (!enemyBullet[i]->GetIsAlive()) { continue; }

		//更新処理
		enemyBullet[i]->Update();

		//プレイヤーがダメージ状態だったら飛ばす
		if (player->GetIsDamege()) { continue; };

		//プレイヤーと敵の弾の当たり判定
		//衝突用に座標と半径の大きさを借りる
		XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
		float bulletSize = enemyBullet[i]->GetScale().x;
		XMFLOAT3 playerPos = player->GetPosition();
		float playerSize = player->GetScale().x;

		//衝突判定を計算
		bool isCollision = Collision::CheckCircle2Circle(
			bulletPos, bulletSize, playerPos, playerSize);

		//プレイヤーと敵の弾が衝突状態でなければ飛ばす
		if (!isCollision) { continue; }

		//敵の弾は死亡
		enemyBullet[i]->Dead();
		//プレイヤーはダメージを喰らう
		player->Damage();

		//ダメージを喰らってもHPが残っていたら飛ばす
		if (player->GetHP() > 0) { continue; }

		//プレイヤー死亡
		player->Dead();
	}

	//固定オブジェクト更新
	for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
	{
		(*itrFixedObject)->Update();
	}

	//コネクトサークル更新
	for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
	{
		//サイズ変更状態でない場合は飛ばす
		if (!(*itrConnectCircle)->GetIsChangeRadius()) { continue; }

		//更新
		(*itrConnectCircle)->Update();

		//衝突を判定してパワーアップ線を作成
		for (auto itrConnectCircle2 = connectCircles.begin(); itrConnectCircle2 != connectCircles.end(); itrConnectCircle2++)
		{
			CreatePowerUpLine(*itrConnectCircle, *itrConnectCircle2);
		}
	}

	//コネクトサークル削除
	for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrConnectCircle)->GetIsDelete())
		{
			//パワーアップ線が削除するコネクトサークルを使用しているか確認
			for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
			{
				//使用していたら線を削除状態にセット
				if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
				{
					(*itrLine)->SetDelete();
				}
			}

			//要素を削除、リストから除外する
			safe_delete(*itrConnectCircle);
			itrConnectCircle = connectCircles.erase(itrConnectCircle);
			continue;
		}
		//for分を回す
		itrConnectCircle++;
	}

	//パワーアップ線更新
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		(*itrLine)->Update(camera);
	}

	//パワーアップ線削除
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end();)
	{
		//削除フラグがtrueなら削除
		if ((*itrLine)->GetIsDelete())
		{
			//削除するパワーアップ線がコネクトサークルを使用しているか確認
			for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
			{
				//使用していたらコネクトサークルを小さくする（線が減るので）
				if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
				{
					(*itrConnectCircle)->SmallRadius();
				}
			}

			//要素を削除、リストから除外する
			safe_delete(*itrLine);
			itrLine = powerUpLines.erase(itrLine);
			continue;
		}
		//for分を回す
		itrLine++;
	}

	//カメラセット
	if (isShake)
	{
		camera->CameraShake(5);
		ShakeTime++;
		if (ShakeTime > 10) {
			isShake = false;
			ShakeTime = 0;
		}
	}
	camera->TpsCamera({ 0,0,-100 });
	camera->Update();

	//スプライト更新
	sprite->Update();


	if (player->GetHP() == 3) { DebugText::GetInstance()->Print("HP : 3", 100, 500); }
	else if (player->GetHP() == 2) { DebugText::GetInstance()->Print("HP : 2", 100, 500); }
	else if (player->GetHP() == 1) { DebugText::GetInstance()->Print("HP : 1", 100, 500); }
	else if (player->GetHP() == 0) { DebugText::GetInstance()->Print("HP : 0", 100, 500); }

	if (player->GetIsAlive()) { DebugText::GetInstance()->Print("PLAYER ALIVE", 100, 550); }
	else { DebugText::GetInstance()->Print("PLAYER DEAD", 100, 550); }

	if (hageEnemys.size() == 0) { DebugText::GetInstance()->Print("ENEMY : 0", 100, 600); }
	else if (hageEnemys.size() == 1) { DebugText::GetInstance()->Print("ENEMY : 1", 100, 600); }
	else if (hageEnemys.size() == 2) { DebugText::GetInstance()->Print("ENEMY : 2", 100, 600); }
	else if (hageEnemys.size() == 3) { DebugText::GetInstance()->Print("ENEMY : 3", 100, 600); }
	else if (hageEnemys.size() == 4) { DebugText::GetInstance()->Print("ENEMY : 4", 100, 600); }
	else if (hageEnemys.size() == 5) { DebugText::GetInstance()->Print("ENEMY : 5", 100, 600); }
	else if (hageEnemys.size() == 6) { DebugText::GetInstance()->Print("ENEMY : 6", 100, 600); }
	else if (hageEnemys.size() == 7) { DebugText::GetInstance()->Print("ENEMY : 7", 100, 600); }
	else if (hageEnemys.size() == 8) { DebugText::GetInstance()->Print("ENEMY : 8", 100, 600); }
	else if (hageEnemys.size() == 9) { DebugText::GetInstance()->Print("ENEMY : 9", 100, 600); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("LB:Sneak", 1000, 150);
	DebugText::GetInstance()->Print("RB:BulletShot", 1000, 200);
	DebugText::GetInstance()->Print("ENTERKEY:CreateEnemy", 1000, 250);

	//エフェクトの更新
	effects->Update(camera);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//スプライト背面描画
	//Sprite::PreDraw(cmdList);

	//sprite->Draw();
	//Sprite::PostDraw();

	//エフェクトの描画
	effects->Draw(cmdList);

	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	//オブジェクト描画
	player->Draw();

	//プレイヤー弾描画
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i]->Draw();
	}
	//敵の弾描画
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i]->Draw();
	}
	//ガル族描画
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		(*itrGaruEnemy)->Draw();
	}
	//ハゲ族描画
	for (auto itrHageEnemy = hageEnemys.begin(); itrHageEnemy != hageEnemys.end(); itrHageEnemy++)
	{
		(*itrHageEnemy)->Draw();
	}
	//固定オブジェクト描画
	for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
	{
		(*itrFixedObject)->Draw();
	}

	Object3d::PostDraw();

	//線3d
	DrawLine3D::PreDraw(cmdList);

	//レーザーサイト描画
	laserSite->Draw();

	//パワーアップ線描画
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		(*itrLine)->Draw();
	}

	DrawLine3D::PostDraw();

	//オブジェクト描画
	Object3d::PreDraw(cmdList);

	//コネクトサークル描画
	for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
	{
		(*itrConnectCircle)->Draw();
	}

	Object3d::PostDraw();


	//スプライト前面描画
	Sprite::PreDraw(cmdList);

	//デバッグテキスト描画
	DebugText::GetInstance()->DrawAll(cmdList);

	Sprite::PostDraw();
}

void GameScene::SpawnGaruEnemy()
{
	//生成時にスポーン座標と移動後の座標を決める
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	spawnPos.x = (float)(rand() % 200 - 100);
	spawnPos.y = 100;

	stayPos.x = (float)(rand() % 200 - 100);
	stayPos.y = (float)(rand() % 120 - 60);


	//20%の確率でガルタタ　80%の確率でガルタを生成
	int enemyKindRand = rand() % 5;
	if (enemyKindRand == 0)
	{
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
	}
	else
	{
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}
}

void GameScene::SpawnHageEnemy()
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	//4パターンのランダムで初期座標と移動方向をセット
	int posAngleRand = rand() % 4;
	if (posAngleRand == 0) { startPos = { 0, -65, 0 }; angle = 0; }
	else if (posAngleRand == 1) { startPos = { 115, 0, 0 }; angle = 90; }
	else if (posAngleRand == 2) { startPos = { 0, 65, 0 }; angle = 180; }
	else if (posAngleRand == 3) { startPos = { -115, 0, 0 }; angle = 270; }

	//20%の確率でハゲタタ　80%の確率でハゲタを生成
	int enemyKindRand = rand() % 5;
	if (enemyKindRand == 0)
	{
		hageEnemys.push_back(Hagetata::Create(happyModel, startPos, player->GetPosition()));
	}
	else
	{
		hageEnemys.push_back(Hageta::Create(happyModel, startPos, angle));
	}
}

void GameScene::SetFixedObject()
{
	//固定オブジェクト生成
	fixedObjects.push_back(FixedObject::Create(happyModel, { 20, 0, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 20, 19, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 39, 0, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 39, 19, 0 }));

	//固定コネクトサークルの半径
	float circleRadius = 10.0f;

	//固定オブジェクトを生成した数コネクトサークルを生成
	for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
	{
		connectCircles.push_back(StartSetCircle::Create(circleModel, (*itrFixedObject)->GetPosition(), circleRadius));
	}
}

void GameScene::CreatePowerUpLine(ConnectCircle *startPoint, ConnectCircle *endPoint)
{
	//始点と終点が同じ場合は飛ばす(始点と終点が同じ位置の線が出来てしまうため)
	if (startPoint == endPoint) { return; }

	//既に線が引かれていたら飛ばす
	for (auto line = powerUpLines.begin(); line != powerUpLines.end(); line++)
	{
		if ((*line)->IsAlreadyCreateLine(startPoint, endPoint)) { return; }
	}

	//衝突用に円1と円2の座標と半径の大きさを借りる
	XMFLOAT3 deadPoint1 = startPoint->GetPosition();
	float deadRadius1 = startPoint->GetRadius();
	XMFLOAT3 deadPoint2 = endPoint->GetPosition();
	float deadRadius2 = endPoint->GetRadius();

	//衝突判定を計算
	bool isCollision = Collision::CheckCircle2Circle(
		deadPoint1, deadRadius1, deadPoint2, deadRadius2);

	//円1と円2が衝突状態でなければ飛ばす
	if (!isCollision) { return; }

	//衝突している円1と円2を繋ぐ新しい線を作る
	powerUpLines.push_back(PowerUpLine::Create(
		startPoint, endPoint));

	//繋がれた線の始点と終点の円を大きくする
	startPoint->BigRadius();
	endPoint->BigRadius();
}
