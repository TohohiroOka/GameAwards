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

const float radian = XM_PI / 180.0f;//���W�A��

GameScene::~GameScene()
{
	//new�I�u�W�F�N�g�̔j��
	safe_delete(audio);
	safe_delete(light);
	safe_delete(sprite);

	//���f�����
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
	safe_delete(hexagonModel);
	safe_delete(happyModel);
	safe_delete(portaModel);
	safe_delete(charoModel);
	safe_delete(tuffModel);
	safe_delete(frameModel);
	safe_delete(waveModel);

	//�v���C���[���
	safe_delete(player);

	//�v���C���[�e���
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}

	//���[�U�[�T�C�g���
	safe_delete(laserSite);

	//�R�A���
	safe_delete(core);

	//�K�������
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		safe_delete(*itrGaruEnemy);
	}
	//�K�����̃��X�g���
	garuEnemys.clear();

	//�`�������
	for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
	{
		safe_delete(*itrCharo);
	}
	//�`�����̃��X�g���
	charoEnemys.clear();

	//�|���^���
	for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
	{
		safe_delete(*itrPorta);
	}
	//�|���^�̃��X�g���
	portaEnemys.clear();

	//�{�X���
	for (int i = 0; i < bossNum; i++)
	{
		safe_delete(bossEnemy[i]);
	}

	//�G�̒e���
	for (int i = 0; i < enemyBulletNum; i++)
	{
		safe_delete(enemyBullet[i]);
	}

	//�Œ�I�u�W�F�N�g���
	for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
	{
		safe_delete(*itrFixedObject);
	}
	//�Œ�I�u�W�F�N�g�̃��X�g���
	fixedObjects.clear();

	//�R�l�N�g�T�[�N�����
	for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
	{
		safe_delete(*itrConnectCircle);
	}
	//�R�l�N�g�T�[�N���̃��X�g���
	connectCircles.clear();

	//�p���[�A�b�v�����
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		safe_delete(*itrLine);
	}
	//�p���[�A�b�v���̃��X�g���
	powerUpLines.clear();

	//��ʘg���
	safe_delete(frame);

	//�Ռ��g���
	safe_delete(shockWave);

	//�^�C�g�����S���
	safe_delete(titleLogo);

	//�^�C�g���V�[���pUI���
	safe_delete(titleUI);

	//���U���g�V�[���pUI���
	safe_delete(resultUI);

	//�X�R�A���
	safe_delete(score);

	//�G�t�F�N�g�̉��
	safe_delete(effects);

	//�w�i�̉��
	buckGround->AllDelete();
	safe_delete(buckGround);
}

void GameScene::Initialize(Camera* camera)
{
	// nullptr�`�F�b�N
	assert(camera);

	//���C�g
	light = LightGroup::Create();
	light->DefaultLightSetting();
	Object3d::SetLightGroup(light);

	circleModel = Model::CreateFromOBJ("circle");//�^�o�R�̃��f��
	playerModel = Model::CreateFromOBJ("player");//�v���C���[�̃��f��
	pBodyModel = Model::CreateFromOBJ("playerbody");//�v���C���[�̑̂̃��f��
	pHead01Model = Model::CreateFromOBJ("playerhead1");//�v���C���[�̓��̃��f��(HP1)
	pHead02Model = Model::CreateFromOBJ("playerhead2");//�v���C���[�̓��̃��f��(HP2)
	pHead03Model = Model::CreateFromOBJ("playerhead3");//�v���C���[�̓��̃��f��(HP3)
	razorModel = Model::CreateFromOBJ("razor");//���[�U�[�̃��f��
	pBullModel = Model::CreateFromOBJ("playerbullet");//�v���C���[�̒e�̃��f��
	enemy01Model = Model::CreateFromOBJ("enemy");//�G01(�K���^)�̃��f��
	enemyPoint01Model = Model::CreateFromOBJ("garutaspown");//�G01(�K���^)�̏o���ʒu�̃��f��
	enemy02Model = Model::CreateFromOBJ("garutata");//�G02(�K���^�^)�̃��f��
	enemyPoint02Model = Model::CreateFromOBJ("garutataspown");//�G02(�K���^�^)�̏o���ʒu�̃��f��
	eBullModel = Model::CreateFromOBJ("enemybullet");//�G�̒e�̃��f��
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//���񂾓G�̃��f��
	hexagonModel = Model::CreateFromOBJ("hexagon");//�Z�p�`�̃��f��
	happyModel = Model::CreateFromOBJ("happy");//�^�o�R���f��
	portaModel = Model::CreateFromOBJ("porta");//�|���^�̃��f��
	charoModel = Model::CreateFromOBJ("charo");//�`�����̃��f��
	tuffModel = Model::CreateFromOBJ("tuff");//�^�b�t�̃��f��
	frameModel = Model::CreateFromOBJ("frame");//�t���[���̃��f��
	waveModel = Model::CreateFromOBJ("wave");//�Ռ��g�̃��f��

	//�v���C���[�E�G�|���̃��f�����Z�b�g
	Player::SetWeaponModel(pHead01Model, pHead02Model, pHead03Model);
	//�v���C���[����
	player = Player::Create(pBodyModel);

	//�e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		playerBullet[i] = PlayerBullet::Create(pBullModel);
	}

	//���[�U�[�T�C�g����
	laserSite = LaserSite::Create(razorModel);

	//�R�A����
	core = Core::Create(happyModel);

	//�{�X����
	bossEnemy[0] = Tuff::Create(tuffModel);

	//�G�̒e����
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//�Œ�I�u�W�F�N�g���Z�b�g
	//SetFixedObject();

	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/kari.png");
	Sprite::LoadTexture(2, L"Resources/number.png");
	Sprite::LoadTexture(3, L"Resources/result.png");
	Sprite::LoadTexture(4, L"Resources/SCORE.png");
	Sprite::LoadTexture(5, L"Resources/pressButton.png");

	//�X�v���C�g����
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 64, 64 });

	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);

	//�T�E���h�p
	audio = new Audio();

	//��ʘg����
	frame = Frame::Create(frameModel);

	//�Ռ��g����
	shockWave = ShockWave::Create(waveModel);

	//�^�C�g�����S����
	titleLogo = TitleLogo::Create(1);
	//�^�C�g���V�[���pUI����
	titleUI = TitleUI::Create(happyModel, happyModel);

	//���U���g�V�[���pUI����
	resultUI = ResultUI::Create(3, 4, 2, 5);

	//�X�R�A������
	score = Score::Create(2);

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();

	//�w�i�̏�����
	buckGround->Create(hexagonModel);
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//�^�C�g���V�[��
	if (scene == SceneName::Title)
	{
		//�^�C�g���V�[���p�̓G�X�|�[�����Z�b�g�V�[��
		if (titleScene == TitleSceneName::SpawnEnemySet)
		{
			//�^�C�}�[���X�V
			titleSceneTimer++;

			//�^�C�}�[����莞�Ԍo�߂�����
			const int sceneChangeTime = 60;
			if (titleSceneTimer > sceneChangeTime)
			{
				//�G���X�|�[��
				TitleSceneEnemySpawn();

				//�^�C�g�����S�𗎉����ɂ��Ă���
				XMFLOAT2 fallStartPos = { 640, -1000 };
				XMFLOAT2 fallEndPos = { 640, 90 };
				titleLogo->FallStart(fallStartPos, fallEndPos);

				//�^�C�}�[�����������Ă���
				titleSceneTimer = 0;

				//���̃V�[����
				titleScene = TitleSceneName::SpawnEnemy;
			}
		}
		//�G�X�|�[���V�[��
		else if (titleScene == TitleSceneName::SpawnEnemy)
		{
			//�^�C�g�����S�̗������I��������
			if (!titleLogo->GetIsFall())
			{
				for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
				{
					//�G�����S������(�m�b�N�o�b�N������)
					float knockBackAngle = 3.14f;
					int knockBackPower = 20;
					(*itrGaruEnemy)->SetKnockBack(knockBackAngle, knockBackPower);
				}

				//��ʂ��V�F�C�N������
				isShake = true;

				//���̃V�[����
				titleScene = TitleSceneName::CreateConnectCircle;
			}
		}
		//�G���S��R�l�N�g�T�[�N�������V�[��
		else if (titleScene == TitleSceneName::CreateConnectCircle)
		{
			//�m�b�N�o�b�N���I������u�ԂȂ�
			for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
			{
				if ((*itrGaruEnemy)->TriggerEndKnockBack())
				{
					//�I�u�W�F�N�g�̃��f����ύX����
					(*itrGaruEnemy)->SetModel(deadEnemyModel);

					//�K�����̍��W�ɃR�l�N�g�T�[�N���𐶐�����
					connectCircles.push_back(
						EnemyCircle::Create(circleModel, *itrGaruEnemy));
				}
			}

			//�^�C�}�[���X�V
			titleSceneTimer++;

			//�^�C�}�[����莞�Ԍo�߂�����
			const int sceneChangeTime = 200;
			if (titleSceneTimer > sceneChangeTime)
			{
				//�R�A���X�|�[����Ԃɂ��Ă���
				XMFLOAT3 coreSpawnPos = { 0, 100, 0 };
				XMFLOAT3 coreStayPos = { 0, 25, 0 };
				core->SetSpawn(coreSpawnPos, coreStayPos);

				//�v���C���[���X�|�[����Ԃ��Ă���
				XMFLOAT3 playerSpawnPos = { 0, -100, 0 };
				XMFLOAT3 playerStayPos = { 0, -35, 0 };
				player->SetSpawn(playerSpawnPos, playerStayPos);

				//�^�C�}�[�����������Ă���
				titleSceneTimer = 0;

				//���̃V�[����
				titleScene = TitleSceneName::SpawnPlayerCore;
			}
		}
		//�R�A�ƃv���C���[�X�|�[���V�[��
		else if (titleScene == TitleSceneName::SpawnPlayerCore)
		{
			//�R�A�ƃv���C���[�̃X�|�[�����I�������
			if (!core->GetIsSpawn() && !player->GetIsSpawn())
			{
				//���̃V�[����
				titleScene = TitleSceneName::PlayerMove;
			}
		}
		//�v���C���[�ړ��\�V�[��
		else if (titleScene == TitleSceneName::PlayerMove)
		{
			//�v���C���[�e����
			if (player->GetIsBulletShot())
			{
				ShotPlayerBullet();
			}
			//�v���C���[�e�X�V
			for (int i = 0; i < playerBulletNum; i++)
			{
				//�e�������Ă��Ȃ���Δ�΂�
				if (!playerBullet[i]->GetIsAlive()) { continue; }

				//�e�ƃp���[�A�b�v���̓����蔻��
				for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
				{
					//�Փ˗p�ɒe�̍��W�Ɣ��a�A���̎n�_�ƏI�_���؂��
					XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
					float bulletRadius = playerBullet[i]->GetScale().x;
					XMFLOAT3 lineStartPoint = (*itrLine)->GetStartPoint();
					XMFLOAT3 lineEndPoint = (*itrLine)->GetEndPoint();

					//�Փ˔�����v�Z
					bool isCollision = Collision::CheckCircle2Line(
						bulletPos, bulletRadius, lineStartPoint, lineEndPoint);

					//�e�Ɛ����Փˏ��
					if (isCollision)
					{
						//�f�o�b�O�p���̐F�ύX
						(*itrLine)->SetColor({ 1, 0, 0, 1 });

						//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��p���[�A�b�v�������Ă��܂�����)
						if (!playerBullet[i]->IsKnowLine((*itrLine)))
						{
							//�e���p���[�A�b�v������
							playerBullet[i]->PowerUp();
						}
					}
				}

				//�e�ƃR�A�̓����蔻��
				{
					//�Փ˗p�ɒe�̍��W�Ɣ��a�A�R�A�̍��W�Ɣ��a���؂��
					XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
					float bulletRadius = playerBullet[i]->GetScale().x;
					XMFLOAT3 corePos = core->GetPosition();
					float coreRadius = core->GetScale().x;

					//�Փ˔�����v�Z
					bool isCollision = Collision::CheckCircle2Circle(
						bulletPos, bulletRadius, corePos, coreRadius);

					//�e�ƃR�A���Փˏ�ԂłȂ���Δ�΂�
					if (!isCollision) { continue; }

					//�e�͎��S
					playerBullet[i]->Dead();

					//�R�A�Ƀ_���[�W
					core->Damage(playerBullet[i]->GetPower());
				}
			}

			//�R�A�����S������
			if (!core->GetIsAlive())
			{
				//���̃V�[����
				titleScene = TitleSceneName::CoreExplosion;
			}
		}
		//�R�A�����V�[��
		else if (titleScene == TitleSceneName::CoreExplosion)
		{
			//�@�����ɃR�A���������������Ă��������B
			//�u�������I�������v�Ƃ���������������if���ɓ���Ă�������
			//�@�Q�[���V�[���Ɉڍs���܂�


			//�f�o�b�O�p�Ƀe�L�g�[�ȏ�����������
			int a = 0;
			if (a == 0)
			{
				//�V�[�����Q�[���V�[���Ɉڍs
				scene = SceneName::Game;
				//���^�C�g���V�[���ɗ������Ƃ��̂��߂ɏ��������Ă���
				titleScene = TitleSceneName::SpawnEnemySet;
			}
		}

		//�v���C���[�X�V
		player->Update();
		//���[�U�[�T�C�g�X�V
		laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
		laserSite->Update();
		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Update();
		}
		//�R�A�X�V
		core->Update();
		//�K�����X�V
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//�X�V����
			(*itrGaruEnemy)->Update();
		}
		//�K�����폜
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrGaruEnemy)->GetIsDelete())
			{
				//�R�l�N�g�T�[�N�����폜����K�������g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N�����폜��ԂɃZ�b�g
					if ((*itrConnectCircle)->CheckUseEnemy(*itrGaruEnemy))
					{
						(*itrConnectCircle)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrGaruEnemy);
				itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
				continue;
			}
			//for������
			itrGaruEnemy++;
		}
		//�R�l�N�g�T�[�N���X�V
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			//�X�V
			(*itrConnectCircle)->Update();

			//�T�C�Y�ύX��ԂłȂ��ꍇ�͔�΂�
			if (!(*itrConnectCircle)->GetIsChangeRadius()) { continue; }

			//�Փ˂𔻒肵�ăp���[�A�b�v�����쐬
			for (auto itrConnectCircle2 = connectCircles.begin(); itrConnectCircle2 != connectCircles.end(); itrConnectCircle2++)
			{
				CreatePowerUpLine(*itrConnectCircle, *itrConnectCircle2);
			}
		}
		//�R�l�N�g�T�[�N���폜
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrConnectCircle)->GetIsDelete())
			{
				//�p���[�A�b�v�����폜����R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
				for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
				{
					//�g�p���Ă���������폜��ԂɃZ�b�g
					if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
					{
						(*itrLine)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrConnectCircle);
				itrConnectCircle = connectCircles.erase(itrConnectCircle);
				continue;
			}
			//for������
			itrConnectCircle++;
		}
		//�p���[�A�b�v���X�V
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Update(camera);
		}
		//�p���[�A�b�v���폜
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrLine)->GetIsDelete())
			{
				//�폜����p���[�A�b�v�����R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N��������������i��������̂Łj
					if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
					{
						(*itrConnectCircle)->SmallRadius();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrLine);
				itrLine = powerUpLines.erase(itrLine);
				continue;
			}
			//for������
			itrLine++;
		}
		//�^�C�g�����S�X�V
		titleLogo->Update();
		//�^�C�g���V�[���pUI�X�V
		titleUI->Update(player->GetPosition());
	}
	//�Q�[���v���C�V�[��
	else if (scene == SceneName::Game)
	{
		//�v���C���[�X�V
		player->Update();

		//���[�U�[�T�C�g�X�V
		laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
		laserSite->Update();

		//�v���C���[�e����
		if (player->GetIsBulletShot())
		{
			ShotPlayerBullet();
		}

		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�X�V����
			playerBullet[i]->Update();

			//�e�������Ă��Ȃ���Δ�΂�
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//�e�ƃp���[�A�b�v���̓����蔻��
			for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
			{
				//�Փ˗p�ɒe�̍��W�Ɣ��a�A���̎n�_�ƏI�_���؂��
				XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
				float bulletRadius = playerBullet[i]->GetScale().x;
				XMFLOAT3 lineStartPoint = (*itrLine)->GetStartPoint();
				XMFLOAT3 lineEndPoint = (*itrLine)->GetEndPoint();

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Line(
					bulletPos, bulletRadius, lineStartPoint, lineEndPoint);

				//�e�Ɛ����Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�f�o�b�O�p���̐F�ύX
				(*itrLine)->SetColor({ 1, 0, 0, 1 });

				//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��p���[�A�b�v�������Ă��܂�����)
				if (playerBullet[i]->IsKnowLine((*itrLine))) { continue; }

				//�e���p���[�A�b�v������
				playerBullet[i]->PowerUp();
			}
		}

		//�K��������
		if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
		{
			//�K�������X�|�[��
			SpawnGaruEnemy(0);
		}

		//�K�����X�V
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//�X�V����
			(*itrGaruEnemy)->Update();

			//�m�b�N�o�b�N���I������u�ԂȂ�
			if ((*itrGaruEnemy)->TriggerEndKnockBack())
			{
				//�m�b�N�o�b�N�I�����̍��W�ŁA���̎��S��Ԃ̃K�����Ƃ̓����蔻������
				for (auto itrGaruEnemy2 = garuEnemys.begin(); itrGaruEnemy2 != garuEnemys.end(); itrGaruEnemy2++)
				{
					//�Փˑ��肪���݂������Ă������΂�
					if ((*itrGaruEnemy2)->GetIsExistence()) { continue; }

					//�������g�Ƃ̓����蔻��͍s��Ȃ�
					if (itrGaruEnemy == itrGaruEnemy2) { continue; }

					//�Փ˗p�ɃK����1�ƃK����2�̍��W�Ɣ��a�̑傫�����؂��
					XMFLOAT3 enemyPos1 = (*itrGaruEnemy)->GetPosition();
					float enemyRadius1 = (*itrGaruEnemy)->GetScale().x;
					XMFLOAT3 enemyPos2 = (*itrGaruEnemy2)->GetPosition();
					float enemyRadius2 = (*itrGaruEnemy2)->GetScale().x;

					//�Փ˔�����v�Z
					bool isCollision = Collision::CheckCircle2Circle(
						enemyPos1, enemyRadius1, enemyPos2, enemyRadius2);

					//�K����1�ƃK����2���Փˏ�ԂłȂ���Δ�΂�
					if (!isCollision) { continue; }

					//�K����1�ƃK����2�����폜��Ԃɂ���
					(*itrGaruEnemy)->SetDelete();
					(*itrGaruEnemy2)->SetDelete();
				}

				//�폜��ԂȂ炱�̐�̏������s��Ȃ�
				if ((*itrGaruEnemy)->GetIsDelete()) { continue; }

				//�m�b�N�o�b�N�I�����̍��W�ŁA�Œ�I�u�W�F�N�g�Ɠ����蔻������
				for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
				{
					//�Փ˗p�ɃK�����ƌŒ�I�u�W�F�N�g�̍��W�Ɣ��a�̑傫�����؂��
					XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
					float enemyRadius = (*itrGaruEnemy)->GetScale().x;
					XMFLOAT3 fixedObjectPos = (*itrFixedObject)->GetPosition();
					float fixedObjectRadius = (*itrFixedObject)->GetScale().x;

					//�Փ˔�����v�Z
					bool isCollision = Collision::CheckCircle2Circle(
						enemyPos, enemyRadius, fixedObjectPos, fixedObjectRadius);

					//�K�����ƌŒ�I�u�W�F�N�g���Փˏ�ԂłȂ���Δ�΂�
					if (!isCollision) { continue; }

					//�폜��Ԃɂ���
					(*itrGaruEnemy)->SetDelete();
				}

				//�폜��ԂȂ炱�̐�̏������s��Ȃ�
				if ((*itrGaruEnemy)->GetIsDelete()) { continue; }

				//�I�u�W�F�N�g�̃��f����ύX����
				(*itrGaruEnemy)->SetModel(deadEnemyModel);

				//�K�����̍��W�ɃR�l�N�g�T�[�N���𐶐�����
				connectCircles.push_back(
					EnemyCircle::Create(circleModel, *itrGaruEnemy));

				//�K�����̑��݂��Ȃ��Ȃ����̂Ŕ�΂�
				continue;
			}

			//�K�����������Ă��Ȃ���Δ�΂� �K�������X�|�[�������������΂� �K���������������������΂�
			if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

			//�e���˃t���O��true�Ȃ�
			if ((*itrGaruEnemy)->GetIsBulletShot())
			{
				//�e����
				GaruEnemyShotBullet(*itrGaruEnemy);
			}

			//�v���C���[�ƃK�����̓����蔻��
			//�v���C���[���_���[�W��ԂłȂ��Ȃ画�������
			if (!player->GetIsDamege())
			{
				//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
				float enemySize = (*itrGaruEnemy)->GetScale().x;
				XMFLOAT3 playerPos = player->GetPosition();
				float playerSize = player->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemySize, playerPos, playerSize);

				//�K�����ƃv���C���[���Փˏ��
				if (isCollision)
				{
					//�v���C���[�̓_���[�W����炤
					player->Damage();
					player->SetKnockback();

					isShake = true;

					//HP��0�Ȃ�
					if (player->GetHP() <= 0)
					{
						//�v���C���[���S
						player->Dead();
					}
				}
			}

			//�e�ƃK�����̓����蔻��
			for (int i = 0; i < playerBulletNum; i++)
			{
				//�e�����ˏ�ԂłȂ���Δ�΂�
				if (!playerBullet[i]->GetIsAlive()) { continue; }

				//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
				float bulletSize = playerBullet[i]->GetScale().x;
				XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
				float enemySize = (*itrGaruEnemy)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					bulletPos, bulletSize, enemyPos, enemySize);

				//�K�����ƒe���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�e�͎��S
				playerBullet[i]->Dead();

				//�K�����̓_���[�W����炤
				int bulletPower = playerBullet[i]->GetPower();
				(*itrGaruEnemy)->Damage(bulletPower);

				//�_���[�W�������Ă�HP���c���Ă������΂�
				if ((*itrGaruEnemy)->GetHP() > 0) { continue; }

				//�K������HP��0�ȉ��Ȃ̂Ńm�b�N�o�b�N���J�n����
				float bulletAngle = playerBullet[i]->GetAngle();
				(*itrGaruEnemy)->SetKnockBack(bulletAngle, bulletPower);
			}
		}

		//�K�����폜
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrGaruEnemy)->GetIsDelete())
			{
				//�R�l�N�g�T�[�N�����폜����K�������g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N�����폜��ԂɃZ�b�g
					if ((*itrConnectCircle)->CheckUseEnemy(*itrGaruEnemy))
					{
						(*itrConnectCircle)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrGaruEnemy);
				itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
				continue;
			}
			//for������
			itrGaruEnemy++;
		}

		//�`�����|���^����
		if (input->TriggerKey(DIK_LSHIFT) || Xinput->TriggerButton(XInputManager::PAD_LT))
		{
			SpawnCharoPorta(0);
		}

		//�`�����X�V
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			//�X�V����
			XMFLOAT3 tartgetPos = player->GetPosition();
			(*itrCharo)->Update(tartgetPos);

			//�v���C���[�ƃ`�����̓����蔻��
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
			float enemySize = (*itrCharo)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//�v���C���[�ƃ`�������Փˏ��
			if (isCollision)
			{
				//�`���������ł��Ŏ��S
				(*itrCharo)->Dead();

				//�v���C���[���_���[�W��ԂłȂ��Ȃ画�������
				if (!player->GetIsDamege())
				{
					//�v���C���[�̓_���[�W����炤
					player->Damage();
					player->SetKnockback();

					//��ʂ��V�F�C�N������
					isShake = true;

					//HP��0�Ȃ�
					if (player->GetHP() <= 0)
					{
						//�v���C���[���S
						player->Dead();
					}
				}
			}

			//�e�ƃ`�����̓����蔻��
			for (int i = 0; i < playerBulletNum; i++)
			{
				//�e�����ˏ�ԂłȂ���Δ�΂�
				if (!playerBullet[i]->GetIsAlive()) { continue; }

				//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
				float bulletSize = playerBullet[i]->GetScale().x;
				XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
				float enemySize = (*itrCharo)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					bulletPos, bulletSize, enemyPos, enemySize);

				//�`�����ƒe���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�e�͎��S
				playerBullet[i]->Dead();

				//�`�����̓_���[�W����炤
				int bulletPower = playerBullet[i]->GetPower();
				(*itrCharo)->Damage(bulletPower);

				//�_���[�W�������Ă�HP���c���Ă������΂�
				if ((*itrCharo)->GetHP() > 0) { continue; }

				//�`������HP��0�ȉ��Ȃ̂Ŏ��S
				(*itrCharo)->Dead();
			}
		}

		//�`�����폜
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrCharo)->GetIsDelete())
			{
				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrCharo);
				itrCharo = charoEnemys.erase(itrCharo);
				continue;
			}
			//for������
			itrCharo++;
		}

		//�|���^�X�V
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			//�X�V����
			(*itrPorta)->Update();

			//�v���C���[�ƃ|���^�̓����蔻��
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
			float enemySize = (*itrPorta)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//�v���C���[�ƃ|���^���Փˏ��
			if (isCollision)
			{
				//�|���^�����ł��Ŏ��S
				(*itrPorta)->Dead();

				//�v���C���[���_���[�W��ԂłȂ��Ȃ画�������
				if (!player->GetIsDamege())
				{
					//�v���C���[�̓_���[�W����炤
					player->Damage();
					player->SetKnockback();

					//��ʂ��V�F�C�N������
					isShake = true;

					//HP��0�Ȃ�
					if (player->GetHP() <= 0)
					{
						//�v���C���[���S
						player->Dead();
					}
				}
			}

			//�e�ƃ|���^�̓����蔻��
			for (int i = 0; i < playerBulletNum; i++)
			{
				//�e�����ˏ�ԂłȂ���Δ�΂�
				if (!playerBullet[i]->GetIsAlive()) { continue; }

				//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
				float bulletSize = playerBullet[i]->GetScale().x;
				XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
				float enemySize = (*itrPorta)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					bulletPos, bulletSize, enemyPos, enemySize);

				//�|���^�ƒe���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�e�͎��S
				playerBullet[i]->Dead();

				//�|���^�̓_���[�W����炤
				int bulletPower = playerBullet[i]->GetPower();
				(*itrPorta)->Damage(bulletPower);

				//�_���[�W�������Ă�HP���c���Ă������΂�
				if ((*itrPorta)->GetHP() > 0) { continue; }

				//�|���^��HP��0�ȉ��Ȃ̂Ŏ��S
				(*itrPorta)->Dead();
			}
		}

		//�|���^�폜
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrPorta)->GetIsDelete())
			{
				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrPorta);
				itrPorta = portaEnemys.erase(itrPorta);
				continue;
			}
			//for������
			itrPorta++;
		}

		//�{�X��J�n
		if (!isBossStage && input->TriggerKey(DIK_0)/* || Xinput->TriggerButton(XInputManager::PAD_LT*/)
		{
			BossStageStart();
		}

		//�{�X�풆
		if (isBossStage)
		{
			//�{�X�X�V
			XMFLOAT3 targetPos = player->GetPosition();
			bossEnemy[moveBossNumber]->Update(targetPos);

			//�{�X���X�|�[�����ȊO��������e���˂Ɠ����蔻����s��
			if (!bossEnemy[moveBossNumber]->GetIsDuringSpawn())
			{
				//�e���˃t���O��true�Ȃ�
				if (bossEnemy[moveBossNumber]->GetIsBulletShot())
				{
					//�e����
					BossEnemyShotBullet(moveBossNumber);
				}

				//�Ռ��t���O��true�Ȃ�
				if (bossEnemy[moveBossNumber]->TriggerImpact())
				{
					//��ʂ��V�F�C�N������
					isShake = true;

					//�K�������~�点��
					BossImpactFallEnemy();
				}

				//�v���C���[�ƃ{�X�̓����蔻��
				//�v���C���[���_���[�W��ԂłȂ��Ȃ画�������
				if (!player->GetIsDamege())
				{
					//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
					XMFLOAT3 bossPos = bossEnemy[moveBossNumber]->GetPosition();
					float bossSize = bossEnemy[moveBossNumber]->GetScale().x;
					XMFLOAT3 playerPos = player->GetPosition();
					float playerSize = player->GetScale().x;

					//�Փ˔�����v�Z
					bool isCollision = Collision::CheckCircle2Circle(
						bossPos, bossSize, playerPos, playerSize);

					//�v���C���[�ƃ{�X���Փˏ��
					if (isCollision)
					{
						//�v���C���[�̓_���[�W����炤
						player->Damage();
						player->SetKnockback();

						//��ʂ��V�F�C�N������
						isShake = true;

						//HP��0�Ȃ�
						if (player->GetHP() <= 0)
						{
							//�v���C���[���S
							player->Dead();
						}
					}
				}

				//�e�ƃ{�X�̓����蔻��
				for (int i = 0; i < playerBulletNum; i++)
				{
					//�e�����ˏ�ԂłȂ���Δ�΂�
					if (!playerBullet[i]->GetIsAlive()) { continue; }

					//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
					XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
					float bulletSize = playerBullet[i]->GetScale().x;
					XMFLOAT3 bossPos = bossEnemy[moveBossNumber]->GetPosition();
					float bossSize = bossEnemy[moveBossNumber]->GetScale().x;

					//�Փ˔�����v�Z
					bool isCollision = Collision::CheckCircle2Circle(
						bulletPos, bulletSize, bossPos, bossSize);

					//�{�X�ƒe���Փˏ�ԂłȂ���Δ�΂�
					if (!isCollision) { continue; }

					//�e�͎��S
					playerBullet[i]->Dead();

					//�{�X�̓_���[�W����炤
					int bulletPower = playerBullet[i]->GetPower();
					bossEnemy[moveBossNumber]->Damage(bulletPower);

					//�_���[�W�������Ă�HP���c���Ă������΂�
					if (bossEnemy[moveBossNumber]->GetHP() > 0) { continue; }

					//�`������HP��0�ȉ��Ȃ̂Ŏ��S
					bossEnemy[moveBossNumber]->Dead();
				}
			}

			//�{�X�����S������
			if (!bossEnemy[moveBossNumber]->GetIsAlive())
			{
				//���̃E�F�[�u�Ƀ`�F���W
				scene = SceneName::ChangeWave;

				//�{�X��I��
				isBossStage = false;
			}
		}

		//�G�̒e�X�V
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//�e�������Ă��Ȃ���Δ�΂�
			if (!enemyBullet[i]->GetIsAlive()) { continue; }

			//�X�V����
			enemyBullet[i]->Update();

			//�v���C���[���_���[�W��Ԃ��������΂�
			if (player->GetIsDamege()) { continue; };

			//�v���C���[�ƓG�̒e�̓����蔻��
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, playerPos, playerSize);

			//�v���C���[�ƓG�̒e���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�G�̒e�͎��S
			enemyBullet[i]->Dead();
			//�v���C���[�̓_���[�W����炤
			player->Damage();

			//�_���[�W�������Ă�HP���c���Ă������΂�
			if (player->GetHP() > 0) { continue; }

			//�v���C���[���S
			player->Dead();
		}

		//�Œ�I�u�W�F�N�g�X�V
		for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
		{
			(*itrFixedObject)->Update();
		}

		//�R�l�N�g�T�[�N���X�V
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			//�X�V
			(*itrConnectCircle)->Update();

			//�T�C�Y�ύX��ԂłȂ��ꍇ�͔�΂�
			if (!(*itrConnectCircle)->GetIsChangeRadius()) { continue; }

			//�Փ˂𔻒肵�ăp���[�A�b�v�����쐬
			for (auto itrConnectCircle2 = connectCircles.begin(); itrConnectCircle2 != connectCircles.end(); itrConnectCircle2++)
			{
				CreatePowerUpLine(*itrConnectCircle, *itrConnectCircle2);
			}
		}

		//�R�l�N�g�T�[�N���폜
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrConnectCircle)->GetIsDelete())
			{
				//�p���[�A�b�v�����폜����R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
				for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
				{
					//�g�p���Ă���������폜��ԂɃZ�b�g
					if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
					{
						(*itrLine)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrConnectCircle);
				itrConnectCircle = connectCircles.erase(itrConnectCircle);
				continue;
			}
			//for������
			itrConnectCircle++;
		}

		//�p���[�A�b�v���X�V
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Update(camera);
		}

		//�p���[�A�b�v���폜
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrLine)->GetIsDelete())
			{
				//�폜����p���[�A�b�v�����R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N��������������i��������̂Łj
					if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
					{
						(*itrConnectCircle)->SmallRadius();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrLine);
				itrLine = powerUpLines.erase(itrLine);
				continue;
			}
			//for������
			itrLine++;
		}

		//�X�R�A���Z
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

		//	���̃E�F�[�u�Ƀ`�F���W
		if (input->TriggerKey(DIK_R) || Xinput->TriggerButton(XInputManager::PAD_START))
		{
			scene = SceneName::ChangeWave;
		}

		//�v���C���[�̎��S������Q�[���I�[�o�[
		if (!player->GetIsAlive())
		{
			scene = SceneName::GameOver;
		}

		//�X�v���C�g�X�V
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

	//�E�F�[�u�ύX�V�[��
	else if (scene == SceneName::ChangeWave)
	{
		//�E�F�[�u�X�V
		if (changeWaveScene == ChangeWaveSceneName::WaveUpdate)
		{
			//�v���C���[�������ʒu�ɖ߂������Z�b�g
			player->SetResetPosition();
			//�E�F�[�u�����̔ԍ��ɍX�V
			wave++;

			//���̃V�[����
			changeWaveScene = ChangeWaveSceneName::PlayerReset;
		}
		//�v���C���[�������ʒu�ɓ�����
		else if (changeWaveScene == ChangeWaveSceneName::PlayerReset)
		{
			//�v���C���[�������ʒu�Ɉړ����I������
			if (!player->GetIsResetPos())
			{
				//��ʃT�C�Y3��1�̂Ƃ��g�̃��C�����Z�b�g
				if (wave % 3 == 1)
				{
					frame->SetChangeFrameLine(1);

					//��ʘg�X�V�̃V�[����
					changeWaveScene = ChangeWaveSceneName::FrameMove;
				}
				//��ʃT�C�Y1��2��3�̂Ƃ��J���������ύX�Ƙg�̃��C�����Z�b�g
				else if (wave % 3 == 2)
				{
					SetChangeCameraDistance(-150);
					frame->SetChangeFrameLine(2);

					//��ʘg�ƃJ�����X�V�̃V�[����
					changeWaveScene = ChangeWaveSceneName::FrameCameraMove;
				}
				else if (wave % 3 == 0)
				{
					SetChangeCameraDistance(-200);
					frame->SetChangeFrameLine(3);

					//��ʘg�ƃJ�����X�V�̃V�[����
					changeWaveScene = ChangeWaveSceneName::FrameCameraMove;
				}

				//�v���C���[���~��Ԃɂ��Ă���
				player->SetIsStop(true);
			}
		}
		//��ʘg�̑傫���ύX(��ʃT�C�Y3��1�p)
		else if (changeWaveScene == ChangeWaveSceneName::FrameMove)
		{
			//��ʘg���k������ꍇ�̂ݘg�ƓG�̓����蔻��𔻒�
			for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
			{
				//���݂�����ꍇ�͔�΂�
				if ((*itrGaruEnemy)->GetIsExistence()) { continue; }
				//�g�ɂԂ����Ă��Ȃ���Δ�΂�
				XMFLOAT2 frameLine = frame->GetFrameLine();
				if (!(*itrGaruEnemy)->IsCollisionFrame(frameLine)) { continue; };

				//�폜��ԂɃZ�b�g
				(*itrGaruEnemy)->SetDelete();

				//�X�R�A���Z
				score->SetAddScore(10000);
			}

			//�g�̃��C���ύX���I������
			if (!frame->GetIsChangeFrameLine())
			{
				//�J���������ύX���Z�b�g
				SetChangeCameraDistance(-100);

				//���̃V�[����
				changeWaveScene = ChangeWaveSceneName::CameraMove;
			}
		}
		//�J���������ύX(��ʃT�C�Y3��1�p)
		else if (changeWaveScene == ChangeWaveSceneName::CameraMove)
		{
			//�J���������̕ύX���I������
			if (!isChangecameraDis)
			{
				//�g�̃��C���ύX�ɍ��킹�ăI�u�W�F�N�g�̗l�X�ȋ��E�����ύX
				XMFLOAT2 frameLine = frame->GetFrameLine();
				Player::SetMoveRange({ frameLine.x - 5, frameLine.y - 5 });
				PlayerBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
				EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
				Porta::SetReflectionLine({ frameLine.x - 3, frameLine.y - 2 });
				BossEnemy::SetFrameLine({ frameLine.x - 3, frameLine.y - 2 });

				//1�ԏ�������ʂɂȂ����Ƃ��̂݃V�F�C�N
				isShake = true;

				//�v���C���[�̒�~��Ԃ��������Ă���
				player->SetIsStop(false);

				//�V�[�����Q�[���V�[���ɖ߂�
				scene = SceneName::Game;
				//���E�F�[�u�ύX�V�[���ɗ������Ƃ��̂��߂ɏ��������Ă���
				changeWaveScene = ChangeWaveSceneName::WaveUpdate;
			}
		}
		//�g�I�u�W�F�N�g�ƃJ���������ύX(��ʃT�C�Y1��2��3�p)
		else if (changeWaveScene == ChangeWaveSceneName::FrameCameraMove)
		{
			//�J���������̕ύX�Ƙg�̃��C���ύX���I������
			if (!isChangecameraDis && !frame->GetIsChangeFrameLine())
			{
				//�g�̃��C���ύX�ɍ��킹�ăI�u�W�F�N�g�̗l�X�ȋ��E�����ύX
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

				//�v���C���[�̒�~��Ԃ��������Ă���
				player->SetIsStop(false);

				//�V�[�����Q�[���V�[���ɖ߂�
				scene = SceneName::Game;
				//���E�F�[�u�ύX�V�[���ɗ������Ƃ��̂��߂ɏ��������Ă���
				changeWaveScene = ChangeWaveSceneName::WaveUpdate;
			}
		}

		//�v���C���[�X�V
		player->Update();
		//���[�U�[�T�C�g�X�V
		laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
		laserSite->Update();
		//�v���C���[�e�X�V
		for (int i = 0; i < playerBulletNum; i++)
		{
			playerBullet[i]->Update();
		}
		//�K�����X�V
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//�X�V����
			(*itrGaruEnemy)->Update();
		}
		//�K�����폜
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrGaruEnemy)->GetIsDelete())
			{
				//�R�l�N�g�T�[�N�����폜����K�������g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N�����폜��ԂɃZ�b�g
					if ((*itrConnectCircle)->CheckUseEnemy(*itrGaruEnemy))
					{
						(*itrConnectCircle)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrGaruEnemy);
				itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
				continue;
			}
			//for������
			itrGaruEnemy++;
		}
		//�G�e�X�V
		for (int i = 0; i < enemyBulletNum; i++)
		{
			enemyBullet[i]->Update();
		}
		//�Œ�I�u�W�F�N�g�X�V
		for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
		{
			(*itrFixedObject)->Update();
		}
		//�R�l�N�g�T�[�N���X�V
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			(*itrConnectCircle)->Update();
		}
		//�R�l�N�g�T�[�N���폜
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrConnectCircle)->GetIsDelete())
			{
				//�p���[�A�b�v�����폜����R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
				for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
				{
					//�g�p���Ă���������폜��ԂɃZ�b�g
					if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
					{
						(*itrLine)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrConnectCircle);
				itrConnectCircle = connectCircles.erase(itrConnectCircle);
				continue;
			}
			//for������
			itrConnectCircle++;
		}
		//�p���[�A�b�v���X�V
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Update(camera);
		}
		//�p���[�A�b�v���폜
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrLine)->GetIsDelete())
			{
				//�폜����p���[�A�b�v�����R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N��������������i��������̂Łj
					if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
					{
						(*itrConnectCircle)->SmallRadius();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrLine);
				itrLine = powerUpLines.erase(itrLine);
				continue;
			}
			//for������
			itrLine++;
		}
	}

	//�Q�[���I�[�o�[�V�[��
	else if (scene == SceneName::GameOver)
	{
		//�v���C���[�ƒe���폜
		if (gameOverScene == GameOverSceneName::DeletePlayerAndBullets)
		{
			//�v���C���[�e�폜
			for (int i = 0; i < playerBulletNum; i++)
			{
				//�e�������ĂȂ���Δ�΂�
				if (!playerBullet[i]->GetIsAlive()) { continue; }
				//�e���폜����
				playerBullet[i]->Dead();
			}
			//�G�e�폜
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//�e�������ĂȂ���Δ�΂�
				if (!enemyBullet[i]->GetIsAlive()) { continue; }
				//�e���폜����
				enemyBullet[i]->Dead();
			}

			//�Ռ��g�𔭎˂���
			shockWave->SetWaveStart(player->GetPosition());

			//���̃V�[����
			gameOverScene = GameOverSceneName::ShockWaveMove;
		}
		//�Ռ��g��傫������
		else if (gameOverScene == GameOverSceneName::ShockWaveMove)
		{
			//�Ռ��g�X�V
			shockWave->Update();

			//�Ռ��g�̓����蔻��
			XMFLOAT3 shockWavePos = shockWave->GetPosition();
			float shockWaveSize = shockWave->GetRadius();
			//�K�����Ƃ̓����蔻��
			for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
			{
				//�Ռ��g�Ƃ̏Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
				float enemySize = (*itrGaruEnemy)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemySize, shockWavePos, shockWaveSize);

				//�K�����ƏՌ��g���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�폜��ԂɃZ�b�g
				(*itrGaruEnemy)->SetDelete();
			}
			//�`�����Ƃ̓����蔻��
			for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
			{
				//�Ռ��g�Ƃ̏Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
				float enemySize = (*itrCharo)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemySize, shockWavePos, shockWaveSize);

				//�`�����ƏՌ��g���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�폜��ԂɃZ�b�g
				(*itrCharo)->SetDelete();
			}
			//�|���^�Ƃ̓����蔻��
			for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
			{
				//�Ռ��g�Ƃ̏Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
				float enemySize = (*itrPorta)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemySize, shockWavePos, shockWaveSize);

				//�|���^�ƏՌ��g���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�폜��ԂɃZ�b�g
				(*itrPorta)->SetDelete();
			}
			//�{�X�풆�Ȃ�{�X�Ƃ̓����蔻��
			if (isBossStage)
			{
				//�{�X�������Ă�����
				if (bossEnemy[moveBossNumber]->GetIsAlive())
				{
					//�Ռ��g�Ƃ̏Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
					XMFLOAT3 bossPos = bossEnemy[moveBossNumber]->GetPosition();
					float bossSize = bossEnemy[moveBossNumber]->GetScale().x;

					//�Փ˔�����v�Z
					bool isCollision = Collision::CheckCircle2Circle(
						bossPos, bossSize, shockWavePos, shockWaveSize);

					//�{�X�ƏՌ��g���Փˏ��
					if (isCollision)
					{
						//���S��ԂɃZ�b�g
						bossEnemy[moveBossNumber]->Dead();
					}
				}
			}
			//�Œ�I�u�W�F�N�g�Ƃ̓����蔻��
			for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
			{
				//�Ռ��g�Ƃ̏Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 objectPos = (*itrFixedObject)->GetPosition();
				float obejctSize = (*itrFixedObject)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					objectPos, obejctSize, shockWavePos, shockWaveSize);

				//�Œ�I�u�W�F�N�g�ƏՌ��g���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�폜��ԂɃZ�b�g
				(*itrFixedObject)->SetDelete();
			}
			//�g���󂳂�Ă��Ȃ��Ƃ��̂ݘg�Ƃ̓����蔻��
			if (!frame->GetIsBreak())
			{
				//�㉺���E�����ꂩ�̘g�ɏՌ��g������������
				bool isCollisionLeft = shockWavePos.x - shockWaveSize < -frame->GetFrameLine().x;
				bool isCollisionRight = shockWavePos.x + shockWaveSize > frame->GetFrameLine().x;
				bool isCollisionUp = shockWavePos.y + shockWaveSize > frame->GetFrameLine().y;
				bool isCollisionDown = shockWavePos.y - shockWaveSize < -frame->GetFrameLine().y;
				if (isCollisionLeft || isCollisionRight || isCollisionUp || isCollisionDown)
				{
					//�g��j�󂷂�
					frame->Break();

					//�V�F�C�N������
					isShake = true;
				}
			}
		}

		//�v���C���[�X�V
		player->Update();
		//�K�����X�V
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//�X�V����
			(*itrGaruEnemy)->Update();
		}
		//�K�����폜
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrGaruEnemy)->GetIsDelete())
			{
				//�R�l�N�g�T�[�N�����폜����K�������g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N�����폜��ԂɃZ�b�g
					if ((*itrConnectCircle)->CheckUseEnemy(*itrGaruEnemy))
					{
						(*itrConnectCircle)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrGaruEnemy);
				itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
				continue;
			}
			//for������
			itrGaruEnemy++;
		}
		//�`�����X�V
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			//�X�V����
			XMFLOAT3 tartgetPos = player->GetPosition();
			(*itrCharo)->Update(tartgetPos);
		}
		//�`�����폜
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrCharo)->GetIsDelete())
			{
				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrCharo);
				itrCharo = charoEnemys.erase(itrCharo);
				continue;
			}
			//for������
			itrCharo++;
		}
		//�|���^�X�V
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			//�X�V����
			(*itrPorta)->Update();
		}
		//�|���^�폜
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrPorta)->GetIsDelete())
			{
				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrPorta);
				itrPorta = portaEnemys.erase(itrPorta);
				continue;
			}
			//for������
			itrPorta++;
		}
		//�{�X�풆�Ȃ�{�X�Ƃ̍X�V
		if (isBossStage)
		{
			//�{�X�������Ă�����
			if (bossEnemy[moveBossNumber]->GetIsAlive())
			{
				//�{�X�X�V
				XMFLOAT3 targetPos = player->GetPosition();
				bossEnemy[moveBossNumber]->Update(targetPos);
			}
		}
		//�Œ�I�u�W�F�N�g�X�V
		for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
		{
			(*itrFixedObject)->Update();
		}
		//�Œ�I�u�W�F�N�g�폜
		for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrFixedObject)->GetIsDelete())
			{
				//�R�l�N�g�T�[�N�����폜����K�������g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N�����폜��ԂɃZ�b�g
					if ((*itrConnectCircle)->CheckUseFixedObject(*itrFixedObject))
					{
						(*itrConnectCircle)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrFixedObject);
				itrFixedObject = fixedObjects.erase(itrFixedObject);
				continue;
			}
			//for������
			itrFixedObject++;
		}
		//�R�l�N�g�T�[�N���X�V
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			(*itrConnectCircle)->Update();
		}
		//�R�l�N�g�T�[�N���폜
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrConnectCircle)->GetIsDelete())
			{
				//�p���[�A�b�v�����폜����R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
				for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
				{
					//�g�p���Ă���������폜��ԂɃZ�b�g
					if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
					{
						(*itrLine)->SetDelete();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrConnectCircle);
				itrConnectCircle = connectCircles.erase(itrConnectCircle);
				continue;
			}
			//for������
			itrConnectCircle++;
		}
		//�p���[�A�b�v���X�V
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Update(camera);
		}
		//�p���[�A�b�v���폜
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrLine)->GetIsDelete())
			{
				//�폜����p���[�A�b�v�����R�l�N�g�T�[�N�����g�p���Ă��邩�m�F
				for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
				{
					//�g�p���Ă�����R�l�N�g�T�[�N��������������i��������̂Łj
					if ((*itrLine)->CheckUsePoints(*itrConnectCircle))
					{
						(*itrConnectCircle)->SmallRadius();
					}
				}

				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrLine);
				itrLine = powerUpLines.erase(itrLine);
				continue;
			}
			//for������
			itrLine++;
		}


		if (input->TriggerKey(DIK_7))
		{
			scene = SceneName::Result;

			//�X�R�A���m�肳����
			resultUI->SetFinalScore(score->GetScore());
		}

		DebugText::GetInstance()->Print("GAMEOVER", 640, 200);
	}

	//���U���g�V�[��
	else if (scene == SceneName::Result)
	{
		//���U���g�V�[���pUI�X�V
		resultUI->Update();

		DebugText::GetInstance()->Print("Result", 640, 200);


		//���U���g�V�[���S�Ă�`�悵�Ă�����
		if (resultUI->GetIsDrawAll())
		{
			//����̃{�^������������^�C�g����ʂɖ߂�
			if (input->TriggerKey(DIK_A))
			{
				//�Q�[��������
				ResetGame();
			}
		}
	}

	//�G�t�F�N�g�̍X�V
	effects->Update(camera);

	//�w�i�X�V
	buckGround->Update();

	//�J�����X�V
	CameraUpdate(camera);

	//��ʘg�X�V
	frame->Update();

	//�X�R�A�X�V
	score->Update();
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//�^�C�g���V�[���̕`��
	if (scene == SceneName::Title)
	{
		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//��ʘg�`��
		//frame->Draw();

		//�v���C���[�ړ��\�V�[���ڍs�̂ݕ`��
		if (titleScene >= TitleSceneName::PlayerMove)
		{
			//�^�C�g���V�[���pUI�`��
			titleUI->Draw();
		}

		//�v���C���[�`��
		player->Draw();

		//���[�U�[�T�C�g�`��
		laserSite->Draw();

		//�R�A�`��
		core->Draw();

		//�v���C���[�e�`��
		for (int i = 0; i < playerBulletNum; i++)
		{
			playerBullet[i]->Draw();
		}
		//�K�����`��
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}

		Object3d::PostDraw();

		//��3d
		DrawLine3D::PreDraw(cmdList);

		//�p���[�A�b�v���`��
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Draw();
		}

		DrawLine3D::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�R�l�N�g�T�[�N���`��
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			(*itrConnectCircle)->Draw();
		}

		//�w�i
		buckGround->Draw();

		Object3d::PostDraw();

		//�G�t�F�N�g�̕`��
		//effects->Draw(cmdList);

		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//�^�C�g�����S�`��
		titleLogo->Draw();

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//�Q�[���V�[���̕`��
	else if (scene == SceneName::Game)
	{
		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//��ʘg�`��
		frame->Draw();

		//�v���C���[�`��
		player->Draw();

		//���[�U�[�T�C�g�`��
		laserSite->Draw();

		//�v���C���[�e�`��
		for (int i = 0; i < playerBulletNum; i++)
		{
			playerBullet[i]->Draw();
		}
		//�G�̒e�`��
		for (int i = 0; i < enemyBulletNum; i++)
		{
			enemyBullet[i]->Draw();
		}

		//�{�X�풆
		if (isBossStage)
		{
			//�{�X�X�`��
			bossEnemy[moveBossNumber]->Draw();
		}

		//�K�����`��
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}
		//�`�����`��
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			(*itrCharo)->Draw();
		}
		//�|���^�`��
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			(*itrPorta)->Draw();
		}
		//�Œ�I�u�W�F�N�g�`��
		for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
		{
			(*itrFixedObject)->Draw();
		}

		Object3d::PostDraw();

		//��3d
		DrawLine3D::PreDraw(cmdList);

		//�p���[�A�b�v���`��
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Draw();
		}

		DrawLine3D::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�R�l�N�g�T�[�N���`��
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			(*itrConnectCircle)->Draw();
		}

		//�w�i
		buckGround->Draw();

		Object3d::PostDraw();


		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//�X�R�A�`��
		score->Draw();

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//�E�F�[�u�`�F���W�V�[���̕`��
	else if (scene == SceneName::ChangeWave)
	{
		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//��ʘg�`��
		frame->Draw();

		//�v���C���[�`��
		player->Draw();

		//���[�U�[�T�C�g�`��
		laserSite->Draw();

		//�v���C���[�e�`��
		for (int i = 0; i < playerBulletNum; i++)
		{
			playerBullet[i]->Draw();
		}
		//�K�����`��
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}
		//�`�����`��
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			(*itrCharo)->Draw();
		}
		//�|���^�`��
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			(*itrPorta)->Draw();
		}
		//�Œ�I�u�W�F�N�g�`��
		for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
		{
			(*itrFixedObject)->Draw();
		}

		Object3d::PostDraw();

		//��3d
		DrawLine3D::PreDraw(cmdList);

		//�p���[�A�b�v���`��
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Draw();
		}

		DrawLine3D::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�R�l�N�g�T�[�N���`��
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			(*itrConnectCircle)->Draw();
		}

		//�w�i
		buckGround->Draw();

		Object3d::PostDraw();


		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//�X�R�A�`��
		score->Draw();

		Sprite::PostDraw();
	}
	//�Q�[���I�[�o�[�V�[���̕`��
	else if (scene == SceneName::GameOver)
	{
		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//��ʘg�`��
		frame->Draw();

		//�v���C���[�`��
		player->Draw();

		//�Ռ��g�`��
		shockWave->Draw();

		//�{�X�풆
		if (isBossStage)
		{
			//�{�X�������Ă�����
			if (bossEnemy[moveBossNumber]->GetIsAlive())
			{
				//�{�X�`��
				bossEnemy[moveBossNumber]->Draw();
			}
		}

		//�K�����`��
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			(*itrGaruEnemy)->Draw();
		}
		//�`�����`��
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			(*itrCharo)->Draw();
		}
		//�|���^�`��
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			(*itrPorta)->Draw();
		}
		//�Œ�I�u�W�F�N�g�`��
		for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
		{
			(*itrFixedObject)->Draw();
		}

		Object3d::PostDraw();

		//��3d
		DrawLine3D::PreDraw(cmdList);

		//�p���[�A�b�v���`��
		for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
		{
			(*itrLine)->Draw();
		}

		DrawLine3D::PostDraw();

		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�R�l�N�g�T�[�N���`��
		for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
		{
			(*itrConnectCircle)->Draw();
		}

		//�w�i
		buckGround->Draw();

		Object3d::PostDraw();


		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		//�X�R�A�`��
		score->Draw();

		Sprite::PostDraw();
	}
	//���U���g�V�[���̕`��
	else if (scene == SceneName::Result)
	{
		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//�w�i
		buckGround->Draw();

		Object3d::PostDraw();

		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//���U���g�V�[���pUI�`��
		resultUI->Draw();

		//�f�o�b�O�e�L�X�g�`��
		DebugText::GetInstance()->DrawAll(cmdList);

		Sprite::PostDraw();
	}
	//�S�V�[�����ʂ̕`��
	{
		//�G�t�F�N�g�̕`��
		effects->Draw(cmdList);
	}
}

void GameScene::ResetGame()
{
	//�v���C���[������
	player->Reset();

	//���[�U�[���v���C���[�ɒǏ]������
	laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
	laserSite->Update();

	//�R�A������
	core->Reset();

	//�{�X�풆�ł͂Ȃ�
	isBossStage = false;

	//�J��������������
	cameraPos = { 0, 0, -100 };
	//�J���������C�[�W���O�J�n������
	cameraDisEaseStart = 0;
	//�J���������C�[�W���O�I��������
	cameraDisEaseEnd = 0;
	//�J����������ύX���ł͂Ȃ�
	isChangecameraDis = false;
	//�J���������C�[�W���O�^�C�}�[������
	cameraDisEaseTimer = 0;
	//��ʃV�F�C�N���Ȃ�
	isShake = false;
	//��ʃV�F�C�N���ԏ�����
	ShakeTime = 0;

	//��ʘg������
	frame->Reset();
	//�g�̃��C���ύX�ɍ��킹�ăI�u�W�F�N�g�̗l�X�ȋ��E�����ύX
	XMFLOAT2 frameLine = frame->GetFrameLine();
	Player::SetMoveRange({ frameLine.x - 5, frameLine.y - 5 });
	PlayerBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
	EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
	Porta::SetReflectionLine({ frameLine.x - 3, frameLine.y - 2 });
	BossEnemy::SetFrameLine({ frameLine.x - 3, frameLine.y - 2 });

	//�Ռ��g������
	shockWave->Reset();

	//�X�|�[���p�^�[��
	spawnTimer = 0;//�X�|�[���^�C�}�[������

	//�^�C�g�����S������
	titleLogo->Reset();
	//�^�C�g���V�[���p�^�C�}�[������
	titleSceneTimer = 0;

	//���U���g�V�[���pUI������
	resultUI->Reset();

	//�V�[��������
	scene = SceneName::Title;
	//�^�C�g���V�[��������
	titleScene = TitleSceneName::SpawnEnemySet;
	//�E�F�[�u�ύX�V�[��������
	changeWaveScene = ChangeWaveSceneName::WaveUpdate;
	//�Q�[���I�[�o�[�V�[��������
	gameOverScene = GameOverSceneName::DeletePlayerAndBullets;
	//�E�F�[�u��1�ɖ߂�
	wave = 1;

	//�X�R�A������
	score->Reset();
}

void GameScene::TitleSceneEnemySpawn()
{
	//�^�C�g���V�[���p�̓G�̐�
	const int spawnEnemyNum = 8;
	XMFLOAT3 spawnPos[spawnEnemyNum] = {};
	XMFLOAT3 stayPos[spawnEnemyNum] = {};

	//�X�|�[�����W���Z�b�g
	spawnPos[0] = { -100, 20, 0 };
	spawnPos[1] = { -100, 20, 0 };
	spawnPos[2] = { -100, 20, 0 };
	spawnPos[3] = { -100, 20, 0 };
	spawnPos[4] = { 100, 20, 0 };
	spawnPos[5] = { 100, 20, 0 };
	spawnPos[6] = { 100, 20, 0 };
	spawnPos[7] = { 100, 20, 0 };

	//��~���W���Z�b�g
	stayPos[0] = { -28.5, -10, 0 };
	stayPos[1] = { -9.5f, -10, 0 };
	stayPos[2] = { -28.5f, 15, 0 };
	stayPos[3] = { -9.5f,  15, 0 };
	stayPos[4] = { 9.5f, -10, 0 };
	stayPos[5] = { 28.5f, -10, 0 };
	stayPos[6] = { 9.5f, 15, 0 };
	stayPos[7] = { 28.5f, 15, 0 };

	//�G���X�|�[��
	for (int i = 0; i < spawnEnemyNum; i++)
	{
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos[i], stayPos[i]));
	}
}

void GameScene::ShotPlayerBullet()
{
	//�v���C���[�E�G�|���̍��W�Ɗp�x��e������
	XMFLOAT3 pos = player->GetWeaponPosition();
	XMFLOAT3 rota = player->GetWeaponRotation();

	//�e�̔��˂��v���C���[�E�G�|���̐^��ɐݒ�
	float angle = DirectX::XMConvertToRadians(rota.z + 90);
	pos.x += 8.0f * cosf(angle);
	pos.y += 8.0f * sinf(angle);

	//�����̒e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		//���˂���Ă������΂�
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//�e�̔��ˈʒu�������ɂ��炷
		float angle2 = DirectX::XMConvertToRadians(rota.z + 180);
		XMFLOAT3 shotPosLeft = {};
		shotPosLeft.x = 1.5f * cosf(angle2) + pos.x;
		shotPosLeft.y = 1.5f * sinf(angle2) + pos.y;

		//�e����
		playerBullet[i]->BulletStart(shotPosLeft, rota);

		//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
		break;
	}

	//�E���̒e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		//���˂���Ă������΂�
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//�e�̔��ˈʒu���E���ɂ��炷
		float angle2 = DirectX::XMConvertToRadians(rota.z);
		XMFLOAT3 shotPosRight = {};
		shotPosRight.x = 1.5f * cosf(angle2) + pos.x;
		shotPosRight.y = 1.5f * sinf(angle2) + pos.y;

		//�e����
		playerBullet[i]->BulletStart(shotPosRight, rota);

		//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
		break;
	}
}

void GameScene::SpawnEnemyManager()
{
	//�X�|�[���^�C�}�[�X�V
	spawnTimer++;

	if (spawnTimer == 50)
	{
		SpawnCharoPorta(1);
	}
	else if (spawnTimer == 200)
	{
		SpawnGaruEnemy(2);
	}
	else if (spawnTimer == 600)
	{
		SpawnGaruEnemy(1);
	}
	else if (spawnTimer == 1000)
	{
		SpawnGaruEnemy(3);
	}
	else if (spawnTimer == 1400)
	{
		SpawnGaruEnemy(4);
	}
	else if (spawnTimer == 1800)
	{
		SpawnGaruEnemy(5);
	}
	else if (spawnTimer == 2300)
	{
		SpawnCharoPorta(2);
	}
	else if (spawnTimer == 2450)
	{
		SpawnGaruEnemy(6);
	}
	else if (spawnTimer == 2850)
	{
		SpawnGaruEnemy(7);
	}
	else if (spawnTimer == 3250)
	{
		SpawnGaruEnemy(8);
	}
	else if (spawnTimer == 3650)
	{
		SpawnGaruEnemy(9);
	}
	else if (spawnTimer == 4150)
	{
		SpawnCharoPorta(3);
	}
}

void GameScene::SpawnGaruEnemy(int spawnPattern)
{
	//�������ɃX�|�[�����W�ƈړ���̍��W�����߂�
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	if (spawnPattern == 0)
	{
		//�����_������
		spawnPos.x = (float)(rand() % 400 - 200);
		spawnPos.y = 100;
		stayPos.x = (float)(rand() % 400 - 200);
		stayPos.y = (float)(rand() % 240 - 120);

		//20%�̊m���ŃK���^�^�@80%�̊m���ŃK���^�𐶐�
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
		//��ɃK���^4�́A�K���^�^2��
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
		//���ɃK���^4�́A�K���^�^2��
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
		//��ɃK���^2�́A�K���^�^1�̂Ɖ��ɃK���^2�́A�K���^�^1��
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
		//��ɃK���^2�́A�K���^�^1�̂Ɖ��ɃK���^2�́A�K���^�^1��
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
		//��ɃK���^2�́A�K���^�^1�̂Ɖ��ɃK���^2��
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
		//���ɃK���^2�́A�K���^�^1�̂Ə�ɃK���^2��
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
		//���ɃK���^�^3�̂ƉE�ɃK���^�^2��
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
		//�E�ɃK���^�^3�̂ƍ��ɃK���^�^2��
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
	//���ˈʒu�͓G�̒��S
	XMFLOAT3 startPos = garuEnemy->GetPosition();

	//��������O���[�v���K���^�̏ꍇ�A�v���C���[�_���e�𔭎�
	if (garuEnemy->GetGroup() == GaruEnemy::GaruGroup::Garuta)
	{
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//�e�����˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//�W�I�̍��W�i�v���C���[�̍��W�j���Z�b�g
			XMFLOAT3 targetPos = player->GetPosition();

			//�^�[�Q�b�g�Ɍ����Ēe����
			enemyBullet[i]->AimBulletStart(startPos, targetPos);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
	}
	//��������O���[�v���K���^�^�̏ꍇ�A4�����ɒe���΂�
	else if (garuEnemy->GetGroup() == GaruEnemy::GaruGroup::Garutata)
	{
		//����̒e����
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//����Ɋp�x��ݒ�
			float angle = 45;

			//����ɒe����
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
		//�����̒e����
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//�����Ɋp�x��ݒ�
			float angle = 135;

			//�����ɒe����
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
		//�E���̒e����
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//�E���Ɋp�x��ݒ�
			float angle = 225;

			//�E���ɒe����
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
		//�E��̒e����
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//���˂���Ă������΂�
			if (enemyBullet[i]->GetIsAlive()) { continue; }

			//�E��Ɋp�x��ݒ�
			float angle = 315;

			//�E��ɒe����
			enemyBullet[i]->StraightBulletStart(startPos, angle);

			//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
			break;
		}
	}
}

void GameScene::SpawnCharoPorta(int spawnPattern)
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	if (spawnPattern == 0)
	{
		//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
		int posAngleRand = rand() % 4;
		if (posAngleRand == 0) { startPos = { 0, -65, 0 }; angle = 30; }
		else if (posAngleRand == 1) { startPos = { 115, 0, 0 }; angle = 120; }
		else if (posAngleRand == 2) { startPos = { 0, 65, 0 }; angle = 210; }
		else if (posAngleRand == 3) { startPos = { -115, 0, 0 }; angle = 300; }

		//20%�̊m���Ńn�Q�^�^�@80%�̊m���Ńn�Q�^�𐶐�
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
	//�ǂ̃{�X�𓮂�����
	if (wave >= 1) { moveBossNumber = BossEnemy::BossName::Tuff; }

	//�{�X��������
	bossEnemy[moveBossNumber]->Reset();

	//�{�X��J�n
	isBossStage = true;
}

void GameScene::BossEnemyShotBullet(int moveBossNumber)
{
	//���ˈʒu�͓G�̒��S
	XMFLOAT3 startPos = bossEnemy[moveBossNumber]->GetPosition();

	//�{�X���^�b�t�̏ꍇ
	if (moveBossNumber == BossEnemy::BossName::Tuff)
	{
		//3WAY�e���˃V�[���̏ꍇ
		if (bossEnemy[moveBossNumber]->GetAction() == Tuff::MovementPattern::ThreeWayBullet)
		{
			//�e�̒e���𑬂�����
			float bulletSpeed = 1.0f;
			//�W�I���v���C���[�ɐݒ�
			XMFLOAT3 targetPos = player->GetPosition();
			//�e����1
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//���˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//�W�I�Ɍ������Ĕ���
				enemyBullet[i]->AimBulletStart(startPos, targetPos, bulletSpeed);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
			//�e����2
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//���˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//���ˊp�x��ݒ肷��i�W�I�Ɍ������Ĉ꒼���j
				float radian = atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);
				//�I�u�W�F�N�g�̊p�x��ݒ�
				float degree = DirectX::XMConvertToDegrees(radian);
				//�E������0�ɐݒ肷�邽�߂�90���炷
				degree -= 90;

				//3WAY�̍��̒e
				degree += 15;

				//�W�I���班�����ɂ��炵�Ēe����
				enemyBullet[i]->StraightBulletStart(startPos, degree, bulletSpeed);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
			//�e����3
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//���˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//���ˊp�x��ݒ肷��i�W�I�Ɍ������Ĉ꒼���j
				float radian = atan2f(targetPos.y - startPos.y, targetPos.x - startPos.x);
				//�I�u�W�F�N�g�̊p�x��ݒ�
				float degree = DirectX::XMConvertToDegrees(radian);
				//�E������0�ɐݒ肷�邽�߂�90���炷
				degree -= 90;

				//3WAY�̉E�̒e
				degree -= 15;

				//�W�I���班���E�ɂ��炵�Ēe����
				enemyBullet[i]->StraightBulletStart(startPos, degree, bulletSpeed);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
		}
		//5WAY�e���˃V�[���̏ꍇ
		else if (bossEnemy[moveBossNumber]->GetAction() == Tuff::MovementPattern::FiveWayBullet)
		{
			//�e����1
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//���˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//����Ɋp�x��ݒ�
				float angle = 0;

				//����ɒe����
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
			//�e����2
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//���˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//�����Ɋp�x��ݒ�
				float angle = 72;

				//�����ɒe����
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
			//�e����3
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//���˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//�E���Ɋp�x��ݒ�
				float angle = 144;

				//�E���ɒe����
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
			//�e����4
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//���˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//����Ɋp�x��ݒ�
				float angle = 216;

				//����ɒe����
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
			//�e����5
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//���˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//�����Ɋp�x��ݒ�
				float angle = 288;

				//�����ɒe����
				enemyBullet[i]->StraightBulletStart(startPos, angle);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
		}
	}
}

void GameScene::BossImpactFallEnemy()
{
	//�������ɃX�|�[�����W�ƈړ���̍��W�����߂�
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	//�����_������(�e�L�g�[��5�̍~�点��)
	for (int i = 0; i < 5; i++)
	{
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = (float)(rand() % 200 - 100);
		stayPos.y = (float)(rand() % 120 - 60);

		//20%�̊m���ŃK���^�^�@80%�̊m���ŃK���^�𐶐�
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

void GameScene::SetFixedObject()
{
	//�Œ�I�u�W�F�N�g����
	fixedObjects.push_back(FixedObject::Create(happyModel, { -9, -12, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { -9, 12, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 9, -12, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 9, 12, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { -29, -12, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { -29, 12, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 29, -12, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 29, 12, 0 }));

	//�Œ�R�l�N�g�T�[�N���̔��a
	float circleRadius = 10.0f;

	//�Œ�I�u�W�F�N�g�𐶐��������R�l�N�g�T�[�N���𐶐�
	for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
	{
		connectCircles.push_back(StartSetCircle::Create(circleModel, (*itrFixedObject)->GetPosition(), circleRadius));
	}
}

void GameScene::CreatePowerUpLine(ConnectCircle* startPoint, ConnectCircle* endPoint)
{
	//�n�_�ƏI�_�������ꍇ�͔�΂�(�n�_�ƏI�_�������ʒu�̐����o���Ă��܂�����)
	if (startPoint == endPoint) { return; }

	//���ɐ���������Ă������΂�
	for (auto line = powerUpLines.begin(); line != powerUpLines.end(); line++)
	{
		if ((*line)->IsAlreadyCreateLine(startPoint, endPoint)) { return; }
	}

	//�Փ˗p�ɉ~1�Ɖ~2�̍��W�Ɣ��a�̑傫�����؂��
	XMFLOAT3 deadPoint1 = startPoint->GetPosition();
	float deadRadius1 = startPoint->GetRadius();
	XMFLOAT3 deadPoint2 = endPoint->GetPosition();
	float deadRadius2 = endPoint->GetRadius();

	//�Փ˔�����v�Z
	bool isCollision = Collision::CheckCircle2Circle(
		deadPoint1, deadRadius1, deadPoint2, deadRadius2);

	//�~1�Ɖ~2���Փˏ�ԂłȂ���Δ�΂�
	if (!isCollision) { return; }

	//�Փ˂��Ă���~1�Ɖ~2���q���V�����������
	powerUpLines.push_back(PowerUpLine::Create(
		startPoint, endPoint));

	//���������ꂽ�Ƃ��̃G�t�F�N�g
	StageEffect::SetConnectLine(startPoint->GetPosition(), endPoint->GetPosition());

	//�q���ꂽ���̎n�_�ƏI�_�̉~��傫������
	startPoint->BigRadius();
	endPoint->BigRadius();
}

void GameScene::CameraUpdate(Camera* camera)
{
	//�J���������ύX
	if (isChangecameraDis)
	{
		//�J���������ύX���s������
		const int changeTime = 100;

		//�J���������ύX�^�C�}�[�X�V
		cameraDisEaseTimer++;

		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)cameraDisEaseTimer / changeTime;

		//�C�[�W���O�ŃT�C�Y�ύX
		float newDistance = Easing::InQuint(cameraDisEaseStart, cameraDisEaseEnd, easeTimer);

		//�J�����������X�V
		cameraPos.z = newDistance;

		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (cameraDisEaseTimer >= changeTime)
		{
			//�T�C�Y�ύX��ԏI��
			isChangecameraDis = false;
		}
	}

	//�V�F�C�N
	if (isShake)
	{
		camera->StartCameraShake(5);
		ShakeTime++;

		//�V�F�C�N�J�n�����莞�Ԍo������
		if (ShakeTime > 10) {
			isShake = false;
			ShakeTime = 0;

			//�V�F�C�N�I��
			camera->EndCameraShake();
		}
	}
	//�J�����������Z�b�g
	camera->TpsCamera(cameraPos);
	//�J�����X�V
	camera->Update();
}

void GameScene::SetChangeCameraDistance(float distance)
{
	//�ύX�O�̃J�����������Z�b�g
	cameraDisEaseStart = cameraPos.z;
	//�ύX��̃J�����������Z�b�g
	cameraDisEaseEnd = distance;
	//�J���������ύX�^�C�}�[��������
	cameraDisEaseTimer = 0;
	//�T�C�Y��ύX���ɂ���
	isChangecameraDis = true;
}