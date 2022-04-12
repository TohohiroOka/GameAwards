#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"
#include "Tuff.h"
#include "EnemyCircle.h"
#include "StartSetCircle.h"

const float radian = XM_PI / 180.0f;//ラジアン

GameScene::~GameScene()
{
	XInputManager* Xinput = XInputManager::GetInstance();
	//振動オフ
	Xinput->EndVibration();
	Xinput = nullptr;

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
	safe_delete(razorModel);
	safe_delete(pBullModel);
	safe_delete(enemy01Model);
	safe_delete(enemyPoint01Model);
	safe_delete(enemy02Model);
	safe_delete(enemyPoint02Model);
	safe_delete(eBullModel);
	safe_delete(deadEnemyModel);
	safe_delete(initialCircleCoreModel);
	safe_delete(initialCircleSquareModel);
	safe_delete(hexagonModel);
	safe_delete(happyModel);
	safe_delete(portaModel);
	safe_delete(charoModel);
	safe_delete(tuffModel);
	safe_delete(frameModel);
	safe_delete(waveModel);
	safe_delete(coreModel);

	//プレイヤー解放
	safe_delete(player);

	//プレイヤー弾解放
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}

	//レーザーサイト解放
	safe_delete(laserSite);

	//コア解放
	safe_delete(core);

	//ガル族解放
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		safe_delete(*itrGaruEnemy);
	}
	//ガル族のリスト解放
	garuEnemys.clear();

	//チャロ解放
	for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
	{
		safe_delete(*itrCharo);
	}
	//チャロのリスト解放
	charoEnemys.clear();

	//ポルタ解放
	for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
	{
		safe_delete(*itrPorta);
	}
	//ポルタのリスト解放
	portaEnemys.clear();

	//ボス解放
	for (int i = 0; i < bossNum; i++)
	{
		safe_delete(bossEnemy[i]);
	}

	//敵の弾解放
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
	}

	//固定敵解放
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		safe_delete(*itrFixedEnemy);
	}
	//固定敵のリスト解放
	fixedEnemys.clear();

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

	//画面枠解放
	safe_delete(frame);

	//衝撃波解放
	safe_delete(shockWave);

	//タイトルロゴ解放
	safe_delete(titleLogo);

	//タイトルシーン用UI解放
	safe_delete(titleUI);

	//リザルトシーン用UI解放
	safe_delete(resultUI);

	//スコア解放
	safe_delete(score);

	//エフェクトの解放
	safe_delete(effects);

	//背景の解放
	buckGround->AllDelete();
	safe_delete(buckGround);
}

void GameScene::Initialize(Camera* camera)
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
	razorModel = Model::CreateFromOBJ("razor");//レーザーのモデル
	pBullModel = Model::CreateFromOBJ("playerbullet");//プレイヤーの弾のモデル
	enemy01Model = Model::CreateFromOBJ("enemy");//敵01(ガルタ)のモデル
	enemyPoint01Model = Model::CreateFromOBJ("garutaspown");//敵01(ガルタ)の出現位置のモデル
	enemy02Model = Model::CreateFromOBJ("garutata");//敵02(ガルタタ)のモデル
	enemyPoint02Model = Model::CreateFromOBJ("garutataspown");//敵02(ガルタタ)の出現位置のモデル
	eBullModel = Model::CreateFromOBJ("enemybullet");//敵の弾のモデル
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//死んだ敵のモデル
	initialCircleCoreModel = Model::CreateFromOBJ("initialcirclecore");;//固定敵のコアのモデル
	initialCircleSquareModel = Model::CreateFromOBJ("initialcirclesquare");//固定敵の外枠のモデル
	hexagonModel = Model::CreateFromOBJ("hexagon");//六角形のモデル
	happyModel = Model::CreateFromOBJ("happy");//タバコモデル
	portaModel = Model::CreateFromOBJ("porta");//ポルタのモデル
	charoModel = Model::CreateFromOBJ("charo");//チャロのモデル
	tuffModel = Model::CreateFromOBJ("tuff");//タッフのモデル
	frameModel = Model::CreateFromOBJ("frame");//フレームのモデル
	waveModel = Model::CreateFromOBJ("wave");//衝撃波のモデル
	coreModel = Model::CreateFromOBJ("core");//コアのモデル

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
	laserSite = LaserSite::Create(razorModel);

	//コア生成
	core = Core::Create(coreModel);

	//ボス生成
	bossEnemy[0] = Tuff::Create(tuffModel);

	//敵の弾生成
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/kari.png");
	Sprite::LoadTexture(2, L"Resources/number.png");
	Sprite::LoadTexture(3, L"Resources/result.png");
	Sprite::LoadTexture(4, L"Resources/SCORE.png");
	Sprite::LoadTexture(5, L"Resources/pressButton.png");

	//スプライト生成
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 64, 64 });

	//デバッグテキスト生成
	DebugText::GetInstance()->Initialize(0);

	//サウンド用
	audio = new Audio();

	//画面枠生成
	frame = Frame::Create(frameModel);

	//衝撃波生成
	shockWave = ShockWave::Create(waveModel);

	//タイトルロゴ生成
	titleLogo = TitleLogo::Create(1);
	//タイトルシーン用UI生成
	titleUI = TitleUI::Create(happyModel, happyModel);

	//リザルトシーン用UI生成
	resultUI = ResultUI::Create(3, 4, 2, 5);

	//スコア初期化
	score = Score::Create(2);

	//エフェクト初期化
	effects = new StageEffect();
	effects->Initialize();

	//背景の初期化
	buckGround->Create(hexagonModel);
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//タイトルシーン
	if (scene == SceneName::Title)
	{
		//タイトルシーン用の敵スポーンをセットシーン
		if (titleScene == TitleSceneName::SpawnEnemySet)
		{
			//タイマーを更新
			titleSceneTimer++;

			//タイマーが一定時間経過したら
			const int sceneChangeTime = 60;
			if (titleSceneTimer > sceneChangeTime)
			{
				//敵をスポーン
				TitleSceneEnemySpawn();

				//タイトルロゴを落下中にしておく
				XMFLOAT2 fallStartPos = { 640, -1000 };
				XMFLOAT2 fallEndPos = { 640, 90 };
				titleLogo->FallStart(fallStartPos, fallEndPos);

				//タイマーを初期化しておく
				titleSceneTimer = 0;

				//次のシーンへ
				titleScene = TitleSceneName::SpawnEnemy;
			}
		}
		//敵スポーンシーン
		else if (titleScene == TitleSceneName::SpawnEnemy)
		{
			//タイトルロゴの落下が終了したら
			if (!titleLogo->GetIsFall())
			{
				for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
				{
					//敵を死亡させる(ノックバックさせる)
					float knockBackAngle = 3.14f;
					int knockBackPower = 20;
					(*itrFixedEnemy)->SetKnockBack(knockBackAngle, knockBackPower);
				}

				//振動オン
				Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM);

				//画面をシェイクさせる
				isShake = true;

				//次のシーンへ
				titleScene = TitleSceneName::CreateConnectCircle;
			}
		}
		//敵死亡後コネクトサークル生成シーン
		else if (titleScene == TitleSceneName::CreateConnectCircle)
		{
			//ノックバックが終わった瞬間なら
			for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
			{
				if ((*itrFixedEnemy)->TriggerEndKnockBack())
				{
					float radius = 10.0f;

					//固定敵の座標にコネクトサークルを生成する
					connectCircles.push_back(
						StartSetCircle::Create(circleModel, *itrFixedEnemy, radius));
				}
			}

			//タイマーを更新
			titleSceneTimer++;

			//タイマーが一定以上になったら振動を止める
			if (titleSceneTimer == 10)
			{
				Xinput->EndVibration();
			}

			//タイマーが一定時間経過したら
			const int sceneChangeTime = 200;
			if (titleSceneTimer > sceneChangeTime)
			{
				//コアをスポーン状態にしておく
				XMFLOAT3 coreSpawnPos = { 0, 100, 0 };
				XMFLOAT3 coreStayPos = { 0, 25, 0 };
				core->SetSpawn(coreSpawnPos, coreStayPos);

				//プレイヤーをスポーン状態しておく
				XMFLOAT3 playerSpawnPos = { 0, -100, 0 };
				XMFLOAT3 playerStayPos = { 0, -35, 0 };
				player->SetSpawn(playerSpawnPos, playerStayPos);

				//タイマーを初期化しておく
				titleSceneTimer = 0;

				//次のシーンへ
				titleScene = TitleSceneName::SpawnPlayerCore;
			}
		}
		//コアとプレイヤースポーンシーン
		else if (titleScene == TitleSceneName::SpawnPlayerCore)
		{
			//コアとプレイヤーのスポーンが終わったら
			if (!core->GetIsSpawn() && !player->GetIsSpawn())
			{
				//次のシーンへ
				titleScene = TitleSceneName::PlayerMove;
			}
		}
		//プレイヤー移動可能シーン
		else if (titleScene == TitleSceneName::PlayerMove)
		{
			//プレイヤー弾発射
			if (player->GetIsBulletShot())
			{
				ShotPlayerBullet();
			}
			//プレイヤー弾更新
			for (int i = 0; i < playerBulletNum; i++)
			{
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

					//弾と線が衝突状態
					if (isCollision)
					{
						//デバッグ用線の色変更
						(*itrLine)->SetColor({ 1, 0, 0, 1 });

						//既に衝突したことがあるか確認(衝突中パワーアップし続けてしまうため)
						if (!playerBullet[i]->IsKnowLine((*itrLine)))
						{
							//弾をパワーアップさせる
							playerBullet[i]->PowerUp();
						}
					}
				}

				//弾とコアの当たり判定
				{
					//衝突用に弾の座標と半径、コアの座標と半径を借りる
					XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
					float bulletRadius = playerBullet[i]->GetScale().x;
					XMFLOAT3 corePos = core->GetPosition();
					float coreRadius = core->GetScale().x;

					//衝突判定を計算
					bool isCollision = Collision::CheckCircle2Circle(
						bulletPos, bulletRadius, corePos, coreRadius);

					//弾とコアが衝突状態でなければ飛ばす
					if (!isCollision) { continue; }

					//弾は死亡
					playerBullet[i]->Dead();

					//コアにダメージ
					core->Damage(playerBullet[i]->GetPower());
				}
			}

			//コアが死亡したら
			if (!core->GetIsAlive())
			{
				//振動オン
				Xinput->StartVibration(XInputManager::STRENGTH::MEDIUM);
				//出現位置設定
				explosionPosition = core->GetPosition();
				//次のシーンへ
				titleScene = TitleSceneName::CoreExplosion;
			}
		}
		//コア爆発シーン
		else if (titleScene == TitleSceneName::CoreExplosion)
		{
			//　ここにコア爆発処理
			//進み割合
			float ratio = 0.0f;
			ratio = StageEffect::SetTitleCoreExplosion(explosionPosition);

			//枠にアルファ値を入れる
			frame->ChangeColor(ratio);

			//コアを小さくして行く
			core->ScaleDown(ratio);

			//枠が完全に出現したら次に行く
			if (ratio > 1.0f)
			{
				//振動オフ
				Xinput->EndVibration();
				//シーンをゲームシーンに移行
				scene = SceneName::Game;
				//次タイトルシーンに来た時ときのために初期化しておく
				titleScene = TitleSceneName::SpawnEnemySet;
			}
		}

		//プレイヤー更新
		player->Update();
		//レーザーサイト更新
		laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
		laserSite->Update();
		//プレイヤー弾更新
		for (int i = 0; i < playerBulletNum; i++)
		{
			//更新処理
			playerBullet[i]->Update();
		}
		//コア更新
		core->Update();
		//固定敵更新
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			//更新処理
			(*itrFixedEnemy)->Update();
		}
		//コネクトサークル更新
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			//更新
			(*itrConnectCircle)->Update();

			//サイズ変更状態でない場合は飛ばす
			if (!(*itrConnectCircle)->GetIsChangeRadius()) { continue; }

			//衝突を判定してパワーアップ線を作成
			for (auto itrConnectCircle2 = connectCircles.begin(); itrConnectCircle2 != connectCircles.end(); itrConnectCircle2++)
			{
				CreatePowerUpLine(*itrConnectCircle, *itrConnectCircle2);
			}
		}
		//パワーアップ線更新
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Update(camera);
		}
		//タイトルロゴ更新
		titleLogo->Update();
		//タイトルシーン用UI更新
		titleUI->Update(player->GetPosition());
	}

	//ゲームプレイシーン
	else if (scene == SceneName::Game)
	{
		//プレイヤー更新
		player->Update();

		//レーザーサイト更新
		laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
		laserSite->Update();

		//プレイヤー弾発射
		if (player->GetIsBulletShot())
		{
			ShotPlayerBullet();
		}

		//プレイヤー弾更新
		for (int i = 0; i < playerBulletNum; i++)
		{
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

		//敵生成処理
		SpawnEnemyManager(isBossStage, wave);//敵生成カウント更新や敵更新

		//ガル族生成
		if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
		{
			//ガル族をスポーン
			SpawnGaruEnemy(0, wave);
		}

		//ガル族更新
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//更新処理
			(*itrGaruEnemy)->Update();

			//ノックバックが終わった瞬間なら
			if ((*itrGaruEnemy)->TriggerEndKnockBack())
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

				//ノックバック終了時の座標で、固定敵と当たり判定を取る
				for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
				{
					//衝突用にガル族と固定敵の座標と半径の大きさを借りる
					XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
					float enemyRadius = (*itrGaruEnemy)->GetScale().x;
					XMFLOAT3 fixedObjectPos = (*itrFixedEnemy)->GetPosition();
					float fixedObjectRadius = (*itrFixedEnemy)->GetScale().x;

					//衝突判定を計算
					bool isCollision = Collision::CheckCircle2Circle(
						enemyPos, enemyRadius, fixedObjectPos, fixedObjectRadius);

					//ガル族と固定敵が衝突状態でなければ飛ばす
					if (!isCollision) { continue; }

					//削除状態にする
					(*itrGaruEnemy)->SetDelete();
				}

				//削除状態ならこの先の処理を行わない
				if ((*itrGaruEnemy)->GetIsDelete()) { continue; }

				//オブジェクトのモデルを変更する
				(*itrGaruEnemy)->SetModel(deadEnemyModel);

				//ガル族の座標にコネクトサークルを生成する
				connectCircles.push_back(
					EnemyCircle::Create(circleModel, *itrGaruEnemy));

				//ガル族の存在がなくなったので飛ばす
				continue;
			}

			//ガル族が生きていなければ飛ばす ガル族がスポーン中だったら飛ばす ガル族が逃走中だったら飛ばす
			if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

			//弾発射フラグがtrueなら
			if ((*itrGaruEnemy)->GetIsBulletShot())
			{
				//弾発射
				GaruEnemyShotBullet(*itrGaruEnemy);
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
					if ((*itrConnectCircle)->CheckUseGaruEnemy(*itrGaruEnemy))
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

		//チャロポルタ生成
		if (input->TriggerKey(DIK_LSHIFT) || Xinput->TriggerButton(XInputManager::PAD_LT))
		{
			SpawnCharoPorta(0, wave);
		}

		//チャロ更新
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			//更新処理
			XMFLOAT3 tartgetPos = player->GetPosition();
			(*itrCharo)->Update(tartgetPos);

			//プレイヤーとチャロの当たり判定
			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
			float enemySize = (*itrCharo)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//プレイヤーとチャロが衝突状態
			if (isCollision)
			{
				//チャロも相打ちで死亡
				(*itrCharo)->Dead();

				//プレイヤーがダメージ状態でないなら判定をする
				if (!player->GetIsDamege())
				{
					//プレイヤーはダメージを喰らう
					player->Damage();
					player->SetKnockback();

					//画面をシェイクさせる
					isShake = true;
				}
			}

			//弾とチャロの当たり判定
			for (int i = 0; i < playerBulletNum; i++)
			{
				//弾が発射状態でなければ飛ばす
				if (!playerBullet[i]->GetIsAlive()) { continue; }

				//衝突用に座標と半径の大きさを借りる
				XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
				float bulletSize = playerBullet[i]->GetScale().x;
				XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
				float enemySize = (*itrCharo)->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					bulletPos, bulletSize, enemyPos, enemySize);

				//チャロと弾が衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//弾は死亡
				playerBullet[i]->Dead();

				//チャロはダメージを喰らう
				int bulletPower = playerBullet[i]->GetPower();
				(*itrCharo)->Damage(bulletPower);

				//ダメージを喰らってもHPが残っていたら飛ばす
				if ((*itrCharo)->GetHP() > 0) { continue; }

				//チャロのHPが0以下なので死亡
				(*itrCharo)->Dead();
			}
		}

		//チャロ削除
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end();)
		{
			//削除フラグがtrueなら削除
			if ((*itrCharo)->GetIsDelete())
			{
				//要素を削除、リストから除外する
				safe_delete(*itrCharo);
				itrCharo = charoEnemys.erase(itrCharo);
				continue;
			}
			//for分を回す
			itrCharo++;
		}

		//ポルタ更新
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			//更新処理
			(*itrPorta)->Update();

			//プレイヤーとポルタの当たり判定
			//衝突用に座標と半径の大きさを借りる
			XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
			float enemySize = (*itrPorta)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//衝突判定を計算
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//プレイヤーとポルタが衝突状態
			if (isCollision)
			{
				//ポルタも相打ちで死亡
				(*itrPorta)->Dead();

				//プレイヤーがダメージ状態でないなら判定をする
				if (!player->GetIsDamege())
				{
					//プレイヤーはダメージを喰らう
					player->Damage();
					player->SetKnockback();

					//画面をシェイクさせる
					isShake = true;
				}
			}

			//弾とポルタの当たり判定
			for (int i = 0; i < playerBulletNum; i++)
			{
				//弾が発射状態でなければ飛ばす
				if (!playerBullet[i]->GetIsAlive()) { continue; }

				//衝突用に座標と半径の大きさを借りる
				XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
				float bulletSize = playerBullet[i]->GetScale().x;
				XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
				float enemySize = (*itrPorta)->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					bulletPos, bulletSize, enemyPos, enemySize);

				//ポルタと弾が衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//弾は死亡
				playerBullet[i]->Dead();

				//ポルタはダメージを喰らう
				int bulletPower = playerBullet[i]->GetPower();
				(*itrPorta)->Damage(bulletPower);

				//ダメージを喰らってもHPが残っていたら飛ばす
				if ((*itrPorta)->GetHP() > 0) { continue; }

				//ポルタのHPが0以下なので死亡
				(*itrPorta)->Dead();
			}
		}

		//ポルタ削除
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end();)
		{
			//削除フラグがtrueなら削除
			if ((*itrPorta)->GetIsDelete())
			{
				//要素を削除、リストから除外する
				safe_delete(*itrPorta);
				itrPorta = portaEnemys.erase(itrPorta);
				continue;
			}
			//for分を回す
			itrPorta++;
		}

		//ボス戦開始
		if (!isBossStage && input->TriggerKey(DIK_0)/* || Xinput->TriggerButton(XInputManager::PAD_LT*/)
		{
			BossStageStart();
		}

		//ボス戦中
		if (isBossStage)
		{
			//ボス更新
			XMFLOAT3 targetPos = player->GetPosition();
			bossEnemy[moveBossNumber]->Update(targetPos);

			//ボスがスポーン中以外だったら弾発射と当たり判定を行う
			if (!bossEnemy[moveBossNumber]->GetIsDuringSpawn())
			{
				//弾発射フラグがtrueなら
				if (bossEnemy[moveBossNumber]->GetIsBulletShot())
				{
					//弾発射
					BossEnemyShotBullet(moveBossNumber);
				}

				//衝撃フラグがtrueなら
				if (bossEnemy[moveBossNumber]->TriggerImpact())
				{
					//画面をシェイクさせる
					isShake = true;

					//ガル族を降らせる
					BossImpactFallEnemy();
				}

				//プレイヤーとボスの当たり判定
				//プレイヤーがダメージ状態でないなら判定をする
				if (!player->GetIsDamege())
				{
					//衝突用に座標と半径の大きさを借りる
					XMFLOAT3 bossPos = bossEnemy[moveBossNumber]->GetPosition();
					float bossSize = bossEnemy[moveBossNumber]->GetScale().x;
					XMFLOAT3 playerPos = player->GetPosition();
					float playerSize = player->GetScale().x;

					//衝突判定を計算
					bool isCollision = Collision::CheckCircle2Circle(
						bossPos, bossSize, playerPos, playerSize);

					//プレイヤーとボスが衝突状態
					if (isCollision)
					{
						//プレイヤーはダメージを喰らう
						player->Damage();
						player->SetKnockback();

						//画面をシェイクさせる
						isShake = true;
					}
				}

				//弾とボスの当たり判定
				for (int i = 0; i < playerBulletNum; i++)
				{
					//弾が発射状態でなければ飛ばす
					if (!playerBullet[i]->GetIsAlive()) { continue; }

					//衝突用に座標と半径の大きさを借りる
					XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
					float bulletSize = playerBullet[i]->GetScale().x;
					XMFLOAT3 bossPos = bossEnemy[moveBossNumber]->GetPosition();
					float bossSize = bossEnemy[moveBossNumber]->GetScale().x;

					//衝突判定を計算
					bool isCollision = Collision::CheckCircle2Circle(
						bulletPos, bulletSize, bossPos, bossSize);

					//ボスと弾が衝突状態でなければ飛ばす
					if (!isCollision) { continue; }

					//弾は死亡
					playerBullet[i]->Dead();

					//ボスはダメージを喰らう
					int bulletPower = playerBullet[i]->GetPower();
					bossEnemy[moveBossNumber]->Damage(bulletPower);

					//ダメージを喰らってもHPが残っていたら飛ばす
					if (bossEnemy[moveBossNumber]->GetHP() > 0) { continue; }

					//チャロのHPが0以下なので死亡
					bossEnemy[moveBossNumber]->Dead();
				}
			}

			//ボスが死亡したら
			if (!bossEnemy[moveBossNumber]->GetIsAlive())
			{
				//次のウェーブにチェンジ
				scene = SceneName::ChangeWave;

				//ボス戦終了
				isBossStage = false;
			}
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
		}

		//固定敵更新
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			(*itrFixedEnemy)->Update();
		}

		//コネクトサークル更新
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			//更新
			(*itrConnectCircle)->Update();

			//サイズ変更状態でない場合は飛ばす
			if (!(*itrConnectCircle)->GetIsChangeRadius()) { continue; }

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

		//スコア加算
		if (input->TriggerKey(DIK_Q))
		{
			score->SetAddScore(4000000);
		}
		if (input->TriggerKey(DIK_W))
		{
			score->SetAddScore(20000);
		}
		if (input->TriggerKey(DIK_E))
		{
			score->SetAddScore(1000);
		}

		//	次のウェーブにチェンジ
		if (input->TriggerKey(DIK_R) || Xinput->TriggerButton(XInputManager::PAD_START))
		{
			scene = SceneName::ChangeWave;
		}

		//プレイヤーの死亡したらゲームオーバー
		if (!player->GetIsAlive())
		{
			scene = SceneName::GameOver;
		}

		//スプライト更新
		sprite->Update();


		if (player->GetHP() == 3) { DebugText::GetInstance()->Print("HP : 3", 100, 500); }
		else if (player->GetHP() == 2) { DebugText::GetInstance()->Print("HP : 2", 100, 500); }
		else if (player->GetHP() == 1) { DebugText::GetInstance()->Print("HP : 1", 100, 500); }
		else if (player->GetHP() == 0) { DebugText::GetInstance()->Print("HP : 0", 100, 500); }

		if (player->GetIsAlive()) { DebugText::GetInstance()->Print("PLAYER ALIVE", 100, 550); }
		else { DebugText::GetInstance()->Print("PLAYER DEAD", 100, 550); }


		DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
		DebugText::GetInstance()->Print("RSTICK:ChangeAngle", 1000, 150);
		DebugText::GetInstance()->Print("LB:Sneak", 1000, 200);
		DebugText::GetInstance()->Print("RB:BulletShot", 1000, 250);
	}

	//ウェーブ変更シーン
	else if (scene == SceneName::ChangeWave)
	{
		//ウェーブ更新
		if (changeWaveScene == ChangeWaveSceneName::WaveUpdate)
		{
			//プレイヤーを初期位置に戻す情報をセット
			player->SetResetPosition();
			//ウェーブを次の番号に更新
			wave++;

			//次のシーンへ
			changeWaveScene = ChangeWaveSceneName::PlayerReset;
		}
		//プレイヤーを初期位置に動かす
		else if (changeWaveScene == ChangeWaveSceneName::PlayerReset)
		{
			//プレイヤーを初期位置に移動し終えたら
			if (!player->GetIsResetPos())
			{
				//画面サイズ3→1のとき枠のラインをセット
				if (wave % 3 == 1)
				{
					frame->SetChangeFrameLine(1);

					//画面枠更新のシーンへ
					changeWaveScene = ChangeWaveSceneName::FrameMove;
				}
				//画面サイズ1→2→3のときカメラ距離変更と枠のラインをセット
				else if (wave % 3 == 2)
				{
					SetChangeCameraDistance(-150);
					frame->SetChangeFrameLine(2);

					//画面枠とカメラ更新のシーンへ
					changeWaveScene = ChangeWaveSceneName::FrameCameraMove;
				}
				else if (wave % 3 == 0)
				{
					SetChangeCameraDistance(-200);
					frame->SetChangeFrameLine(3);

					//画面枠とカメラ更新のシーンへ
					changeWaveScene = ChangeWaveSceneName::FrameCameraMove;
				}

				//プレイヤーを停止状態にしておく
				player->SetIsStop(true);
			}
		}
		//画面枠の大きさ変更(画面サイズ3→1用)
		else if (changeWaveScene == ChangeWaveSceneName::FrameMove)
		{
			//画面枠が縮小する場合のみ枠と敵の当たり判定を判定
			for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
			{
				//存在がある場合は飛ばす
				if ((*itrGaruEnemy)->GetIsExistence()) { continue; }
				//枠にぶつかっていなければ飛ばす
				XMFLOAT2 frameLine = frame->GetFrameLine();
				if (!(*itrGaruEnemy)->IsCollisionFrame(frameLine)) { continue; };

				//削除状態にセット
				(*itrGaruEnemy)->SetDelete();

				//スコア加算
				score->SetAddScore(10000);
			}

			//枠のライン変更を終えたら
			if (!frame->GetIsChangeFrameLine())
			{
				//カメラ距離変更をセット
				SetChangeCameraDistance(-100);

				//次のシーンへ
				changeWaveScene = ChangeWaveSceneName::CameraMove;
			}
		}
		//カメラ距離変更(画面サイズ3→1用)
		else if (changeWaveScene == ChangeWaveSceneName::CameraMove)
		{
			//カメラ距離の変更を終えたら
			if (!isChangecameraDis)
			{
				//枠のライン変更に合わせてオブジェクトの様々な境界線も変更
				XMFLOAT2 frameLine = frame->GetFrameLine();
				Player::SetMoveRange({ frameLine.x - 5, frameLine.y - 5 });
				PlayerBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
				EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
				Porta::SetReflectionLine({ frameLine.x - 3, frameLine.y - 2 });
				BossEnemy::SetFrameLine({ frameLine.x - 3, frameLine.y - 2 });

				//1番小さい画面になったときのみシェイク
				isShake = true;

				//プレイヤーの停止状態を解除しておく
				player->SetIsStop(false);

				//シーンをゲームシーンに戻す
				scene = SceneName::Game;
				//次ウェーブ変更シーンに来た時ときのために初期化しておく
				changeWaveScene = ChangeWaveSceneName::WaveUpdate;
			}
		}
		//枠オブジェクトとカメラ距離変更(画面サイズ1→2→3用)
		else if (changeWaveScene == ChangeWaveSceneName::FrameCameraMove)
		{
			//カメラ距離の変更と枠のライン変更を終えたら
			if (!isChangecameraDis && !frame->GetIsChangeFrameLine())
			{
				//枠のライン変更に合わせてオブジェクトの様々な境界線も変更
				XMFLOAT2 frameLine = frame->GetFrameLine();
				if (wave % 3 == 2)
				{
					Player::SetMoveRange({ frameLine.x - 5, frameLine.y - 5 });
					PlayerBullet::SetDeadPos({ frameLine.x + 9, frameLine.y + 7 });
					EnemyBullet::SetDeadPos({ frameLine.x + 9, frameLine.y + 7 });
					Porta::SetReflectionLine({ frameLine.x - 3, frameLine.y - 2 });
					BossEnemy::SetFrameLine({ frameLine.x - 3, frameLine.y - 2 });
				}
				else if (wave % 3 == 0)
				{
					Player::SetMoveRange({ frameLine.x - 5, frameLine.y - 5 });
					PlayerBullet::SetDeadPos({ frameLine.x + 8, frameLine.y + 8 });
					EnemyBullet::SetDeadPos({ frameLine.x + 8, frameLine.y + 8 });
					Porta::SetReflectionLine({ frameLine.x - 4, frameLine.y - 3 });
					BossEnemy::SetFrameLine({ frameLine.x - 4, frameLine.y - 3 });
				}

				//プレイヤーの停止状態を解除しておく
				player->SetIsStop(false);

				//シーンをゲームシーンに戻す
				scene = SceneName::Game;
				//次ウェーブ変更シーンに来た時ときのために初期化しておく
				changeWaveScene = ChangeWaveSceneName::WaveUpdate;
			}
		}

		//プレイヤー更新
		player->Update();
		//レーザーサイト更新
		laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
		laserSite->Update();
		//プレイヤー弾更新
		for (int i = 0; i < playerBulletNum; i++)
		{
			playerBullet[i]->Update();
		}
		//ガル族更新
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//更新処理
			(*itrGaruEnemy)->Update();
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
					if ((*itrConnectCircle)->CheckUseGaruEnemy(*itrGaruEnemy))
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
		//敵弾更新
		for (int i = 0; i < enemyBulletNum; i++)
		{
			enemyBullet[i]->Update();
		}
		//固定敵更新
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			(*itrFixedEnemy)->Update();
		}
		//コネクトサークル更新
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			(*itrConnectCircle)->Update();
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
	}

	//ゲームオーバーシーン
	else if (scene == SceneName::GameOver)
	{
		//プレイヤーと弾を削除
		if (gameOverScene == GameOverSceneName::DeletePlayerAndBullets)
		{
			//振動オン
			Xinput->StartVibration(XInputManager::STRENGTH::LARGE);

			//プレイヤー弾削除
			for (int i = 0; i < playerBulletNum; i++)
			{
				//弾が生きてなければ飛ばす
				if (!playerBullet[i]->GetIsAlive()) { continue; }
				//弾を削除する
				playerBullet[i]->Dead();
			}
			//敵弾削除
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//弾が生きてなければ飛ばす
				if (!enemyBullet[i]->GetIsAlive()) { continue; }
				//弾を削除する
				enemyBullet[i]->Dead();
			}

			//衝撃波を発射する
			shockWave->SetWaveStart(player->GetPosition());

			//次のシーンへ
			gameOverScene = GameOverSceneName::ShockWaveMove;
		}
		//衝撃波を大きくする
		else if (gameOverScene == GameOverSceneName::ShockWaveMove)
		{
			//衝撃波更新
			if (!shockWave->Update())
			{
				//振動オフ
				Xinput->EndVibration();
				//次に行く
				gameOverScene = GameOverSceneName::TitleReturn;
			}

			//衝撃波の当たり判定
			XMFLOAT3 shockWavePos = shockWave->GetPosition();
			float shockWaveSize = shockWave->GetRadius();
			//ガル族との当たり判定
			for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
			{
				//衝撃波との衝突用に座標と半径の大きさを借りる
				XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
				float enemySize = (*itrGaruEnemy)->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemySize, shockWavePos, shockWaveSize);

				//ガル族と衝撃波が衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//削除状態にセット
				(*itrGaruEnemy)->SetDelete();
			}
			//チャロとの当たり判定
			for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
			{
				//衝撃波との衝突用に座標と半径の大きさを借りる
				XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
				float enemySize = (*itrCharo)->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemySize, shockWavePos, shockWaveSize);

				//チャロと衝撃波が衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//削除状態にセット
				(*itrCharo)->SetDelete();
			}
			//ポルタとの当たり判定
			for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
			{
				//衝撃波との衝突用に座標と半径の大きさを借りる
				XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
				float enemySize = (*itrPorta)->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemySize, shockWavePos, shockWaveSize);

				//ポルタと衝撃波が衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//削除状態にセット
				(*itrPorta)->SetDelete();
			}
			//ボス戦中ならボスとの当たり判定
			if (isBossStage)
			{
				//ボスが生きていたら
				if (bossEnemy[moveBossNumber]->GetIsAlive())
				{
					//衝撃波との衝突用に座標と半径の大きさを借りる
					XMFLOAT3 bossPos = bossEnemy[moveBossNumber]->GetPosition();
					float bossSize = bossEnemy[moveBossNumber]->GetScale().x;

					//衝突判定を計算
					bool isCollision = Collision::CheckCircle2Circle(
						bossPos, bossSize, shockWavePos, shockWaveSize);

					//ボスと衝撃波が衝突状態
					if (isCollision)
					{
						//死亡状態にセット
						bossEnemy[moveBossNumber]->Dead();
					}
				}
			}
			//固定敵との当たり判定
			for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
			{
				//衝撃波との衝突用に座標と半径の大きさを借りる
				XMFLOAT3 objectPos = (*itrFixedEnemy)->GetPosition();
				float obejctSize = (*itrFixedEnemy)->GetScale().x;

				//衝突判定を計算
				bool isCollision = Collision::CheckCircle2Circle(
					objectPos, obejctSize, shockWavePos, shockWaveSize);

				//固定敵と衝撃波が衝突状態でなければ飛ばす
				if (!isCollision) { continue; }

				//削除状態にセット
				(*itrFixedEnemy)->SetDelete();
			}
			//枠が壊されていないときのみ枠との当たり判定
			if (!frame->GetIsBreak())
			{
				//上下左右いずれかの枠に衝撃波が当たったら
				bool isCollisionLeft = shockWavePos.x - shockWaveSize < -frame->GetFrameLine().x;
				bool isCollisionRight = shockWavePos.x + shockWaveSize > frame->GetFrameLine().x;
				bool isCollisionUp = shockWavePos.y + shockWaveSize > frame->GetFrameLine().y;
				bool isCollisionDown = shockWavePos.y - shockWaveSize < -frame->GetFrameLine().y;
				if (isCollisionLeft || isCollisionRight || isCollisionUp || isCollisionDown)
				{
					//枠を破壊する
					frame->Break();

					//シェイクさせる
					isShake = true;
				}
			}
		}
		//タイトルに戻る
		else if (gameOverScene == GameOverSceneName::TitleReturn)
		{
			if (input->TriggerKey(DIK_7))
			{
				scene = SceneName::Result;

				//スコアを確定させる
				resultUI->SetFinalScore(score->GetScore());
			}

			DebugText::GetInstance()->Print("GAMEOVER", 640, 200);
		}

		//以下なら更新する
		if (gameOverScene <= GameOverSceneName::ShockWaveMove)
		{
			//プレイヤー更新
			player->Update();
			//ガル族更新
			for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
			{
				//更新処理
				(*itrGaruEnemy)->Update();
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
						if ((*itrConnectCircle)->CheckUseGaruEnemy(*itrGaruEnemy))
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
			//チャロ更新
			for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
			{
				//更新処理
				XMFLOAT3 tartgetPos = player->GetPosition();
				(*itrCharo)->Update(tartgetPos);
			}
			//チャロ削除
			for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end();)
			{
				//削除フラグがtrueなら削除
				if ((*itrCharo)->GetIsDelete())
				{
					//要素を削除、リストから除外する
					safe_delete(*itrCharo);
					itrCharo = charoEnemys.erase(itrCharo);
					continue;
				}
				//for分を回す
				itrCharo++;
			}
			//ポルタ更新
			for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
			{
				//更新処理
				(*itrPorta)->Update();
			}
			//ポルタ削除
			for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end();)
			{
				//削除フラグがtrueなら削除
				if ((*itrPorta)->GetIsDelete())
				{
					//要素を削除、リストから除外する
					safe_delete(*itrPorta);
					itrPorta = portaEnemys.erase(itrPorta);
					continue;
				}
				//for分を回す
				itrPorta++;
			}
			//ボス戦中ならボスとの更新
			if (isBossStage)
			{
				//ボスが生きていたら
				if (bossEnemy[moveBossNumber]->GetIsAlive())
				{
					//ボス更新
					XMFLOAT3 targetPos = player->GetPosition();
					bossEnemy[moveBossNumber]->Update(targetPos);
				}
			}
			//固定敵更新
			for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
			{
				(*itrFixedEnemy)->Update();
			}
			//固定敵削除
			for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end();)
			{
				//削除フラグがtrueなら削除
				if ((*itrFixedEnemy)->GetIsDelete())
				{
					//コネクトサークルが削除するガル族を使用しているか確認
					for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
					{
						//使用していたらコネクトサークルを削除状態にセット
						if ((*itrConnectCircle)->CheckUseFixedEnemy(*itrFixedEnemy))
						{
							(*itrConnectCircle)->SetDelete();
						}
					}

					//要素を削除、リストから除外する
					safe_delete(*itrFixedEnemy);
					itrFixedEnemy = fixedEnemys.erase(itrFixedEnemy);
					continue;
				}
				//for分を回す
				itrFixedEnemy++;
			}
			//コネクトサークル更新
			for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
			{
				(*itrConnectCircle)->Update();
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
		}
	}

	//リザルトシーン
	else if (scene == SceneName::Result)
	{
		//リザルトシーン用UI更新
		resultUI->Update();

		DebugText::GetInstance()->Print("Result", 640, 200);


		//リザルトシーン全てを描画していたら
		if (resultUI->GetIsDrawAll())
		{
			//特定のボタンを押したらタイトル画面に戻る
			if (input->TriggerKey(DIK_A) || Xinput->TriggerButton(XInputManager::PAD_A))
			{
				//ゲーム初期化
				ResetGame();
			}
		}
	}

	//エフェクトの更新
	effects->Update(camera);

	//背景更新
	buckGround->Update();

	//カメラ更新
	CameraUpdate(camera);

	//画面枠更新
	frame->Update();

	//スコア更新
	score->Update();

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//タイトルシーンの描画
	if (scene == SceneName::Title)
	{
		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//画面枠描画
		frame->Draw();

		//プレイヤー移動可能シーン移行のみ描画
		if (titleScene >= TitleSceneName::PlayerMove)
		{
			//タイトルシーン用UI描画
			titleUI->Draw();
		}

		//プレイヤー描画
		player->Draw();

		//レーザーサイト描画
		laserSite->Draw();

		//コア描画
		core->Draw();

		//プレイヤー弾描画
		for (int i = 0; i < playerBulletNum; i++)
		{
			playerBullet[i]->Draw();
		}
		//固定敵描画
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			(*itrFixedEnemy)->Draw();
		}

		Object3d::PostDraw();

		//線3d
		DrawLine3D::PreDraw(cmdList);

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

		//背景
		buckGround->Draw();

		Object3d::PostDraw();

		//エフェクトの描画
		//effects->Draw(cmdList);

		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//タイトルロゴ描画
		titleLogo->Draw();

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//ゲームシーンの描画
	else if (scene == SceneName::Game)
	{
		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//画面枠描画
		frame->Draw();

		//プレイヤー描画
		player->Draw();

		//レーザーサイト描画
		laserSite->Draw();

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

		//ボス戦中
		if (isBossStage)
		{
			//ボス更描画
			bossEnemy[moveBossNumber]->Draw();
		}

		//ガル族描画
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}
		//チャロ描画
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			(*itrCharo)->Draw();
		}
		//ポルタ描画
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			(*itrPorta)->Draw();
		}
		//固定敵描画
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			(*itrFixedEnemy)->Draw();
		}

		Object3d::PostDraw();

		//線3d
		DrawLine3D::PreDraw(cmdList);

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

		//背景
		buckGround->Draw();

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//スコア描画
		score->Draw();

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//ウェーブチェンジシーンの描画
	else if (scene == SceneName::ChangeWave)
	{
		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//画面枠描画
		frame->Draw();

		//プレイヤー描画
		player->Draw();

		//レーザーサイト描画
		laserSite->Draw();

		//プレイヤー弾描画
		for (int i = 0; i < playerBulletNum; i++)
		{
			playerBullet[i]->Draw();
		}
		//ガル族描画
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}
		//チャロ描画
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			(*itrCharo)->Draw();
		}
		//ポルタ描画
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			(*itrPorta)->Draw();
		}
		//固定敵描画
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			(*itrFixedEnemy)->Draw();
		}

		Object3d::PostDraw();

		//線3d
		DrawLine3D::PreDraw(cmdList);

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

		//背景
		buckGround->Draw();

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//スコア描画
		score->Draw();

		Sprite::PostDraw();
	}
	//ゲームオーバーシーンの描画
	else if (scene == SceneName::GameOver)
	{
		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//画面枠描画
		frame->Draw();

		//プレイヤー描画
		player->Draw();

		//衝撃波描画
		shockWave->Draw();

		//ボス戦中
		if (isBossStage)
		{
			//ボスが生きていたら
			if (bossEnemy[moveBossNumber]->GetIsAlive())
			{
				//ボス描画
				bossEnemy[moveBossNumber]->Draw();
			}
		}

		//ガル族描画
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}
		//チャロ描画
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			(*itrCharo)->Draw();
		}
		//ポルタ描画
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			(*itrPorta)->Draw();
		}
		//固定敵描画
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			(*itrFixedEnemy)->Draw();
		}

		Object3d::PostDraw();

		//線3d
		DrawLine3D::PreDraw(cmdList);

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

		//背景
		buckGround->Draw();

		Object3d::PostDraw();


		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		//スコア描画
		score->Draw();

		Sprite::PostDraw();
	}
	//リザルトシーンの描画
	else if (scene == SceneName::Result)
	{
		//オブジェクト描画
		Object3d::PreDraw(cmdList);

		//背景
		buckGround->Draw();

		Object3d::PostDraw();

		//スプライト前面描画
		Sprite::PreDraw(cmdList);

		//リザルトシーン用UI描画
		resultUI->Draw();

		//デバッグテキスト描画
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//全シーン共通の描画
	{
		//エフェクトの描画
		effects->Draw(cmdList);
	}
}

void GameScene::ResetGame()
{
	//プレイヤー初期化
	player->Reset();

	//レーザーをプレイヤーに追従させる
	laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
	laserSite->Update();

	//コア初期化
	core->Reset();

	//ボス戦中ではない
	isBossStage = false;

	//カメラ距離初期化
	cameraPos = { 0, 0, -100 };
	//カメラ距離イージング開始初期化
	cameraDisEaseStart = 0;
	//カメラ距離イージング終了初期化
	cameraDisEaseEnd = 0;
	//カメラ距離を変更中ではない
	isChangecameraDis = false;
	//カメラ距離イージングタイマー初期化
	cameraDisEaseTimer = 0;
	//画面シェイクしない
	isShake = false;
	//画面シェイク時間初期化
	ShakeTime = 0;

	//画面枠初期化
	frame->Reset();
	//枠のライン変更に合わせてオブジェクトの様々な境界線も変更
	XMFLOAT2 frameLine = frame->GetFrameLine();
	Player::SetMoveRange({ frameLine.x - 5, frameLine.y - 5 });
	PlayerBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
	EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
	Porta::SetReflectionLine({ frameLine.x - 3, frameLine.y - 2 });
	BossEnemy::SetFrameLine({ frameLine.x - 3, frameLine.y - 2 });

	//衝撃波初期化
	shockWave->Reset();

	//スポーンパターン
	spawnTimer = 0;//スポーンタイマー初期化

	//タイトルロゴ初期化
	titleLogo->Reset();
	//タイトルシーン用タイマー初期化
	titleSceneTimer = 0;

	//リザルトシーン用UI初期化
	resultUI->Reset();

	//シーン初期化
	scene = SceneName::Title;
	//タイトルシーン初期化
	titleScene = TitleSceneName::SpawnEnemySet;
	//ウェーブ変更シーン初期化
	changeWaveScene = ChangeWaveSceneName::WaveUpdate;
	//ゲームオーバーシーン初期化
	gameOverScene = GameOverSceneName::DeletePlayerAndBullets;
	//ウェーブを1に戻す
	wave = 1;

	//スコア初期化
	score->Reset();
}

void GameScene::TitleSceneEnemySpawn()
{
	//タイトルシーン用の敵の数
	const int spawnEnemyNum = 8;
	XMFLOAT3 spawnPos[spawnEnemyNum] = {};
	XMFLOAT3 stayPos[spawnEnemyNum] = {};

	//スポーン座標をセット
	spawnPos[0] = { -100, 20, 0 };
	spawnPos[1] = { -100, 20, 0 };
	spawnPos[2] = { -100, 20, 0 };
	spawnPos[3] = { -100, 20, 0 };
	spawnPos[4] = { 100, 20, 0 };
	spawnPos[5] = { 100, 20, 0 };
	spawnPos[6] = { 100, 20, 0 };
	spawnPos[7] = { 100, 20, 0 };

	//停止座標をセット
	stayPos[0] = { -28.5, -10, 0 };
	stayPos[1] = { -9.5f, -10, 0 };
	stayPos[2] = { -28.5f, 15, 0 };
	stayPos[3] = { -9.5f,  15, 0 };
	stayPos[4] = { 9.5f, -10, 0 };
	stayPos[5] = { 28.5f, -10, 0 };
	stayPos[6] = { 9.5f, 15, 0 };
	stayPos[7] = { 28.5f, 15, 0 };

	//敵をスポーン
	for (int i = 0; i < spawnEnemyNum; i++)
	{
		fixedEnemys.push_back(FixedEnemy::Create(initialCircleCoreModel, initialCircleSquareModel, spawnPos[i], stayPos[i]));
	}
}

void GameScene::ShotPlayerBullet()
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
}

void GameScene::SpawnEnemyManager(bool isBossStage, int wave)
{
	//ボス戦以外の時はカウントを進める。ボス戦になったら数値をリセットする
	if (!isBossStage)
	{
		spawnTimer++;//敵生成カウント更新
	}
	else
	{
		spawnTimer = 0;//敵生成カウントをリセット
	}

	//spawnIntervalフレーム(300フレーム)毎に敵生成
	if ((spawnTimer % spawnInterval) == 0)
	{
		spawnPattern = (spawnTimer / spawnInterval);//スポーンパターン計算

		//初回スポーン時にセット決定
		if (spawnPattern == 1)
		{
			spawnSet = 1;
			//spawnSet = (int)(rand() % 10) + 1;//スポーンセット計算
		}

		SpawnEnemyGroup(spawnPattern, spawnSet, wave);//敵生成
	}
}

void GameScene::SpawnEnemyGroup(int spawnPattern, int spawnSet, int wave)
{
	//生成時にスポーン座標と移動後の座標を決める(ガルタとガルタタ)
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	//生成時に初期座標と移動方向を決める(ポルタとチャロ)
	XMFLOAT3 startPos = {};
	float angle = 0;

	//ウェーブ、パターン、セットから出現パターンを確定
	if (wave == 1)
	{
		if (spawnPattern == 1)
		{
			if (spawnSet == 1)
			{
				//上にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -18;
				stayPos.y = 13;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 18;
				stayPos.y = 13;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 2)
		{
			if (spawnSet == 1)
			{
				//中央にガルタ2体
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -18;
				stayPos.y = -5;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 18;
				stayPos.y = -5;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 3)
		{
			if (spawnSet == 1)
			{

			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 4)
		{
			if (spawnSet == 1)
			{

			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 5)
		{
			if (spawnSet == 1)
			{

			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 6)
		{
			if (spawnSet == 1)
			{

			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 7)
		{
			if (spawnSet == 1)
			{

			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 8)
		{
			if (spawnSet == 1)
			{

			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 9)
		{
			if (spawnSet == 1)
			{

			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 10)
		{
			if (spawnSet == 1)
			{

			}
			else if (spawnSet == 2)
			{

			}
			else if (spawnSet == 3)
			{

			}
			else if (spawnSet == 4)
			{

			}
			else
			{

			}
		}
		else if (spawnPattern == 11)
		{
			BossStageStart();
		}
	}
	else if (wave == 2)
	{

	}
	else
	{

	}
}

void GameScene::SpawnGaruEnemy(int spawnPattern, int wave)
{
	//生成時にスポーン座標と移動後の座標を決める
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	if (spawnPattern == 0)
	{
		//ランダム生成
		spawnPos.x = (float)(rand() % 400 - 200);
		spawnPos.y = 100;
		stayPos.x = (float)(rand() % 400 - 200);
		stayPos.y = (float)(rand() % 240 - 120);

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
	else if (spawnPattern == 1)
	{
		//上にガルタ4体、ガルタタ2体
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 30;
		stayPos.y = -25;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 30;
		stayPos.y = -45;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 40;
		stayPos.y = -35;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -30;
		stayPos.y = -25;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -30;
		stayPos.y = -45;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -40;
		stayPos.y = -35;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
	}
	else if (spawnPattern == 2)
	{
		//下にガルタ4体、ガルタタ2体
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 30;
		stayPos.y = 25;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 30;
		stayPos.y = 45;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 40;
		stayPos.y = 35;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -30;
		stayPos.y = 25;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -30;
		stayPos.y = 45;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -40;
		stayPos.y = 35;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
	}
	else if (spawnPattern == 3)
	{
		//上にガルタ2体、ガルタタ1体と下にガルタ2体、ガルタタ1体
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 50;
		stayPos.y = 10;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 40;
		stayPos.y = 0;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 50;
		stayPos.y = -10;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -50;
		stayPos.y = 10;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -40;
		stayPos.y = 0;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -50;
		stayPos.y = -10;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}
	else if (spawnPattern == 4)
	{
		//上にガルタ2体、ガルタタ1体と下にガルタ2体、ガルタタ1体
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 40;
		stayPos.y = -15;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 50;
		stayPos.y = -25;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 60;
		stayPos.y = -35;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -40;
		stayPos.y = 15;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -50;
		stayPos.y = 25;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -60;
		stayPos.y = 35;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}
	else if (spawnPattern == 5)
	{
		//上にガルタ2体、ガルタタ1体と下にガルタ2体
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 40;
		stayPos.y = 15;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 50;
		stayPos.y = 25;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 60;
		stayPos.y = 35;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -40;
		stayPos.y = -15;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -50;
		stayPos.y = -25;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -60;
		stayPos.y = -35;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}
	else if (spawnPattern == 6)
	{
		//下にガルタ2体、ガルタタ1体と上にガルタ2体
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -20;
		stayPos.y = 30;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 0;
		stayPos.y = 15;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 20;
		stayPos.y = 30;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -70;
		stayPos.y = -30;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 70;
		stayPos.y = -30;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}
	else if (spawnPattern == 7)
	{
		//左にガルタタ3体と右にガルタタ2体
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -20;
		stayPos.y = -30;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 0;
		stayPos.y = -15;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 20;
		stayPos.y = -30;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -70;
		stayPos.y = 30;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 70;
		stayPos.y = 30;
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}
	else if (spawnPattern == 8)
	{
		//右にガルタタ3体と左にガルタタ2体
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -80;
		stayPos.y = 40;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 80;
		stayPos.y = 20;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -80;
		stayPos.y = 0;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 80;
		stayPos.y = -20;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -80;
		stayPos.y = -40;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
	}
	else if (spawnPattern == 9)
	{
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = 80;
		stayPos.y = 40;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = -100;
		stayPos.x = -80;
		stayPos.y = 20;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 80;
		stayPos.y = 0;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = -80;
		stayPos.y = -20;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = 80;
		stayPos.y = -40;
		garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
	}
}

void GameScene::GaruEnemyShotBullet(GaruEnemy* garuEnemy)
{
	//発射位置は敵の中心
	XMFLOAT3 startPos = garuEnemy->GetPosition();

	//所属するグループがガルタの場合、プレイヤー狙い弾を発射
	if (garuEnemy->GetGroup() == GaruEnemy::GaruGroup::Garuta)
	{
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//弾が発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//標的の座標（プレイヤーの座標）をセット
			XMFLOAT3 targetPos = player->GetPosition();

			//ターゲットに向けて弾発射
			enemyBullet[i]->AimBulletStart(startPos, targetPos);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
	}
	//所属するグループがガルタタの場合、4方向に弾を飛ばす
	else if (garuEnemy->GetGroup() == GaruEnemy::GaruGroup::Garutata)
	{
		//左上の弾発射
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//左上に角度を設定
			float angle = 45;

			//左上に弾発射
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
		//左下の弾発射
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//左下に角度を設定
			float angle = 135;

			//左下に弾発射
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
		//右下の弾発射
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//右下に角度を設定
			float angle = 225;

			//右下に弾発射
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
		//右上の弾発射
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//発射されていたら飛ばす
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//右上に角度を設定
			float angle = 315;

			//右上に弾発射
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
			break;
		}
	}
}

void GameScene::SpawnCharoPorta(int spawnPattern, int wave)
{
	//生成時に初期座標と移動方向を決める
	XMFLOAT3 startPos = {};
	float angle = 0;

	if (spawnPattern == 0)
	{
		//4パターンのランダムで初期座標と移動方向をセット
		int posAngleRand = rand() % 4;
		if (posAngleRand == 0) { startPos = { 0, -65, 0 }; angle = 30; }
		else if (posAngleRand == 1) { startPos = { 115, 0, 0 }; angle = 120; }
		else if (posAngleRand == 2) { startPos = { 0, 65, 0 }; angle = 210; }
		else if (posAngleRand == 3) { startPos = { -115, 0, 0 }; angle = 300; }

		//20%の確率でハゲタタ　80%の確率でハゲタを生成
		int enemyKindRand = rand() % 5;
		if (enemyKindRand == 0)
		{
			charoEnemys.push_back(Charo::Create(charoModel, startPos));
		}
		else
		{
			portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
		}
	}
}

void GameScene::BossStageStart()
{
	//どのボスを動かすか
	if (wave >= 1) { moveBossNumber = BossEnemy::BossName::Tuff; }

	//ボスを初期化
	bossEnemy[moveBossNumber]->Reset();

	//ボス戦開始
	isBossStage = true;
}

void GameScene::BossEnemyShotBullet(int moveBossNumber)
{
	//発射位置は敵の中心
	XMFLOAT3 startPos = bossEnemy[moveBossNumber]->GetPosition();

	//ボスがタッフの場合
	if (moveBossNumber == BossEnemy::BossName::Tuff)
	{
		//3WAY弾発射シーンの場合
		if (bossEnemy[moveBossNumber]->GetAction() == Tuff::MovementPattern::ThreeWayBullet)
		{
			//弾の弾速を速くする
			float bulletSpeed = 1.0f;
			//標的をプレイヤーに設定
			XMFLOAT3 targetPos = player->GetPosition();
			//弾発射1
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//標的に向かって発射
				enemyBullet[i]->AimBulletStart(startPos, targetPos, bulletSpeed);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射2
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//発射角度を設定する（標的に向かって一直線）
				float radian = atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);
				//オブジェクトの角度を設定
				float degree = DirectX::XMConvertToDegrees(radian);
				//右方向を0に設定するために90ずらす
				degree -= 90;

				//3WAYの左の弾
				degree += 15;

				//標的から少し左にずらして弾発射
				enemyBullet[i]->StraightBulletStart(startPos, degree, bulletSpeed);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射3
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//発射角度を設定する（標的に向かって一直線）
				float radian = atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);
				//オブジェクトの角度を設定
				float degree = DirectX::XMConvertToDegrees(radian);
				//右方向を0に設定するために90ずらす
				degree -= 90;

				//3WAYの右の弾
				degree -= 15;

				//標的から少し右にずらして弾発射
				enemyBullet[i]->StraightBulletStart(startPos, degree, bulletSpeed);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}
		//5WAY弾発射シーンの場合
		else if (bossEnemy[moveBossNumber]->GetAction() == Tuff::MovementPattern::FiveWayBullet)
		{
			//弾発射1
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//左上に角度を設定
				float angle = 0;

				//左上に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射2
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//左下に角度を設定
				float angle = 72;

				//左下に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射3
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//右下に角度を設定
				float angle = 144;

				//右下に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射4
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//左上に角度を設定
				float angle = 216;

				//左上に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
			//弾発射5
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//発射されていたら飛ばす
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//左下に角度を設定
				float angle = 288;

				//左下に弾発射
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1つ発射したらループを抜ける(一気に全ての弾を撃ってしまうため)
				break;
			}
		}
	}
}

void GameScene::BossImpactFallEnemy()
{
	//生成時にスポーン座標と移動後の座標を決める
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	//ランダム生成(テキトーに5体降らせる)
	for (int i = 0; i < 5; i++)
	{
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
}

void GameScene::CreatePowerUpLine(ConnectCircle* startPoint, ConnectCircle* endPoint)
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

	//線が引かれたときのエフェクト
	StageEffect::SetConnectLine(startPoint->GetPosition(), endPoint->GetPosition());

	//繋がれた線の始点と終点の円を大きくする
	startPoint->BigRadius();
	endPoint->BigRadius();
}

void GameScene::CameraUpdate(Camera* camera)
{
	//カメラ距離変更
	if (isChangecameraDis)
	{
		//カメラ距離変更を行う時間
		const int changeTime = 100;

		//カメラ距離変更タイマー更新
		cameraDisEaseTimer++;

		//イージング計算用の時間
		float easeTimer = (float)cameraDisEaseTimer / changeTime;

		//イージングでサイズ変更
		float newDistance = Easing::InQuint(cameraDisEaseStart, cameraDisEaseEnd, easeTimer);

		//カメラ距離を更新
		cameraPos.z = newDistance;

		//タイマーが指定した時間になったら
		if (cameraDisEaseTimer >= changeTime)
		{
			//サイズ変更状態終了
			isChangecameraDis = false;
		}
	}

	//シェイク
	if (isShake)
	{
		camera->StartCameraShake(5);
		ShakeTime++;

		//シェイク開始から一定時間経ったら
		if (ShakeTime > 10) {
			isShake = false;
			ShakeTime = 0;

			//シェイク終了
			camera->EndCameraShake();
		}
	}
	//カメラ距離をセット
	camera->TpsCamera(cameraPos);
	//カメラ更新
	camera->Update();
}

void GameScene::SetChangeCameraDistance(float distance)
{
	//変更前のカメラ距離をセット
	cameraDisEaseStart = cameraPos.z;
	//変更後のカメラ距離をセット
	cameraDisEaseEnd = distance;
	//カメラ距離変更タイマーを初期化
	cameraDisEaseTimer = 0;
	//サイズを変更中にする
	isChangecameraDis = true;
}