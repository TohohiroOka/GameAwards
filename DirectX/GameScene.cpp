#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"
#include "Tuff.h"
#include "PinCircle.h"
#include "StartSetCircle.h"

const float radian = XM_PI / 180.0f;//���W�A��

GameScene::~GameScene()
{
	XInputManager* Xinput = XInputManager::GetInstance();
	//�U���I�t
	Xinput->EndVibration();
	Xinput = nullptr;

	//new�I�u�W�F�N�g�̔j��
	safe_delete(audio);
	safe_delete(light);

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
	safe_delete(RBbuttonModel);

	//�v���C���[���
	safe_delete(player);

	//�v���C���[�e���
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}

	//���[�U�[�T�C�g���
	safe_delete(laserSite);

	//�G�l���M�[�|�C���g���
	safe_delete(energy);

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

	//�s�����
	for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
	{
		safe_delete(*itrPin);
	}
	//�s���̃��X�g���
	pins.clear();

	//�Œ�G���
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		safe_delete(*itrFixedEnemy);
	}
	//�Œ�G�̃��X�g���
	fixedEnemys.clear();

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

	//�Ռ��g���
	for (auto itrShockWave = shockWaves.begin(); itrShockWave != shockWaves.end(); itrShockWave++)
	{
		safe_delete(*itrShockWave);
	}
	//�Ռ��g�̃��X�g���
	shockWaves.clear();

	//�z���~���
	for (auto itrAbsorptionCircle = absorptionCircles.begin(); itrAbsorptionCircle != absorptionCircles.end(); itrAbsorptionCircle++) 
	{
		safe_delete(*itrAbsorptionCircle);
	}
	//�z���~�̃��X�g���
	absorptionCircles.clear();

	//��ʘg���
	safe_delete(frame);

	//�������ԉ��
	safe_delete(timeLimit);

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
	initialCircleCoreModel = Model::CreateFromOBJ("initialcirclecore");;//�Œ�G�̃R�A�̃��f��
	initialCircleSquareModel = Model::CreateFromOBJ("initialcirclesquare");//�Œ�G�̊O�g�̃��f��
	hexagonModel = Model::CreateFromOBJ("hexagon");//�Z�p�`�̃��f��
	happyModel = Model::CreateFromOBJ("happy");//�^�o�R���f��
	portaModel = Model::CreateFromOBJ("porta");//�|���^�̃��f��
	charoModel = Model::CreateFromOBJ("charo");//�`�����̃��f��
	tuffModel = Model::CreateFromOBJ("tuff");//�^�b�t�̃��f��
	frameModel = Model::CreateFromOBJ("frame");//�t���[���̃��f��
	waveModel = Model::CreateFromOBJ("wave");//�Ռ��g�̃��f��
	coreModel = Model::CreateFromOBJ("core");//�R�A�̃��f��
	RBbuttonModel = Model::CreateFromOBJ("RB");//RB�{�^���̃��f��


	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/title.png");
	Sprite::LoadTexture(2, L"Resources/number.png");
	Sprite::LoadTexture(3, L"Resources/result.png");
	Sprite::LoadTexture(4, L"Resources/SCORE.png");
	Sprite::LoadTexture(5, L"Resources/pressButton.png");
	Sprite::LoadTexture(6, L"Resources/white1x1.png");
	Sprite::LoadTexture(7, L"Resources/gaugeIn.png");
	Sprite::LoadTexture(8, L"Resources/gaugeOut.png");
	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);


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

	//�G�l���M�[�|�C���g����
	energy = Energy::Create(8, 7);

	//�R�A����
	core = Core::Create(coreModel);

	//�{�X����
	bossEnemy[0] = Tuff::Create(tuffModel);

	//�G�̒e����
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//��ʘg����
	frame = Frame::Create(frameModel);

	//�������Ԑ���
	timeLimit = TimeLimit::Create(2);

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();

	//�w�i�̏�����
	buckGround->Create(hexagonModel);


	//�T�E���h�p
	audio = new Audio();
}

void GameScene::Update(Camera* camera)
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//�v���C���[�X�V
	player->Update();

	//LB�{�^�����́B�X�e�b�v��Ԃł͂Ȃ��B�G�l���M�[�|�C���g������ʈȏ゠��ꍇ�X�e�b�v��Ԃɂ���
	if ((input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_LB))
		&& !(player->GetIsStep()) && energy->CheckPercent(20))
	{
		//�v���C���[���X�e�b�v��Ԃɂ���
		player->SetStep();

		//�G�l���M�[�|�C���g������
		energy->UseTwentyPercent();
	}

	//���[�U�[�T�C�g�X�V
	laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
	laserSite->Update();

	//�v���C���[�e���ߊJ�n
	if (input->TriggerKey(DIK_SPACE) || Xinput->TriggerButton(XInputManager::PAD_RB))
	{
		//�v���C���[���X�e�b�v���łȂ���Η��ߊJ�n
		if (!player->GetIsStep())
		{
			//���߂��J�n
			isBulletPowerUp = true;
			//���߃^�C�}�[��������
			bulletPowerUpTimer = 0;
			//�З͂�������
			bulletPowerLevel = 0;
		}
	}
	//���߂Ă����
	if (isBulletPowerUp)
	{
		//�З͗��� ����
		PowerUpPlayerBullet();
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

			//���̎n�_�ƏI�_�̃R�l�N�g�T�[�N������Ռ��g���o��
			for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
			{
				//�����R�l�N�g�T�[�N�����g�p���Ă��Ȃ��������΂�
				if (!(*itrLine)->CheckUsePoints(*itrConnectCircle)) { continue; }

				//�g�p���Ă�����Ռ��g���R�l�N�g�T�[�N�����甭�˂���
				const XMFLOAT3 pos = (*itrConnectCircle)->GetPosition();
				const int power = playerBullet[i]->GetPower();
				shockWaves.push_back(ShockWave::Create(waveModel, pos, power));
			}
		}
	}


	if (input->TriggerKey(DIK_5) || Xinput->TriggerButton(XInputManager::PAD_BUCK))
	{
		//�G�l���M�[�|�C���g�𑝂₷
		const int point = 10;
		energy->AddEnergyPoint(point);
	}
	//�G�l���M�[�X�V
	energy->Update();

	//�G��������
	SpawnEnemyManager(isBossStage, wave);//�G�����J�E���g�X�V��G�X�V

	//�K��������
	if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//�K�������X�|�[��
		SpawnGaruEnemy(0, wave);
	}

	//�K�����X�V
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		//�X�V����
		(*itrGaruEnemy)->Update();

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
				//�G�l���M�[�|�C���g������
				energy->LoseEnergyPoint();

				isShake = true;
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

		//�s���ƃK�����̓����蔻��
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			//�s�����_���[�W��ԂȂ��΂�
			if ((*itrPin)->GetIsDamege()) { continue; }

			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;
			XMFLOAT3 pinPos = (*itrPin)->GetPosition();
			float pinSize = (*itrPin)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, pinPos, pinSize);

			//�K�����ƃs�����Փˏ�ԂłȂ��Ȃ��΂�
			if (!isCollision) { continue; }

			//�s���̓_���[�W����炤
			int damage = 10;
			(*itrPin)->Damage(damage);
		}
	}

	//�`�����|���^����
	if (input->TriggerKey(DIK_LSHIFT) || Xinput->TriggerButton(XInputManager::PAD_LT))
	{
		SpawnCharoPorta(0, wave);
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
				//�G�l���M�[�|�C���g������
				energy->LoseEnergyPoint();

				//��ʂ��V�F�C�N������
				isShake = true;
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
		}

		//�s���ƃ`�����̓����蔻��
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			//�s�����_���[�W��ԂȂ��΂�
			if ((*itrPin)->GetIsDamege()) { continue; }

			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 pinPos = (*itrPin)->GetPosition();
			float pinSize = (*itrPin)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, pinPos, pinSize);

			//�`�����ƃs�����Փˏ�ԂłȂ��Ȃ��΂�
			if (!isCollision) { continue; }

			//�s���̓_���[�W����炤
			int damage = 10;
			(*itrPin)->Damage(damage);

			//�`���������ł��Ŏ��S
			(*itrCharo)->Dead();
		}
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
				//�G�l���M�[�|�C���g������
				energy->LoseEnergyPoint();

				//��ʂ��V�F�C�N������
				isShake = true;
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
		}

		//�s���ƃ|���^�̓����蔻��
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			//�s�����_���[�W��ԂȂ��΂�
			if ((*itrPin)->GetIsDamege()) { continue; }

			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 pinPos = (*itrPin)->GetPosition();
			float pinSize = (*itrPin)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, pinPos, pinSize);

			//�|���^�ƃs�����Փˏ�ԂłȂ��Ȃ��΂�
			if (!isCollision) { continue; }

			//�s���̓_���[�W����炤
			int damage = 10;
			(*itrPin)->Damage(damage);

			//�|���^�����ł��Ŏ��S
			(*itrPorta)->Dead();
		}
	}

	//�{�X��J�n
	if (!isBossStage && input->TriggerKey(DIK_0))
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
					//�G�l���M�[�|�C���g������
					energy->LoseEnergyPoint();

					//��ʂ��V�F�C�N������
					isShake = true;
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
			//���S���ăT�C�Y��ύX��Ԃɂ���
			bossEnemy[moveBossNumber]->SetDeadChangeScale();

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

		//�v���C���[�ƓG�̒e�̓����蔻��
		//�v���C���[���_���[�W��ԂłȂ��Ȃ�
		if (!player->GetIsDamege())
		{
			//�v���C���[�ƓG�̒e�̓����蔻��
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, playerPos, playerSize);

			//�v���C���[�ƓG�̒e���Փˏ��
			if (isCollision)
			{
				//�G�̒e�͎��S
				enemyBullet[i]->Dead();
				//�v���C���[�̓_���[�W����炤
				player->Damage();
				//�G�l���M�[�|�C���g������
				energy->LoseEnergyPoint();
			}
		}

		//�s���ƓG�̒e�̓����蔻��
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			//�s�����_���[�W��ԂȂ��΂�
			if ((*itrPin)->GetIsDamege()) { continue; }

			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;
			XMFLOAT3 pinPos = (*itrPin)->GetPosition();
			float pinSize = (*itrPin)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, pinPos, pinSize);

			//�G�̒e�ƃs�����Փˏ�ԂłȂ��Ȃ��΂�
			if (!isCollision) { continue; }

			//�s���̓_���[�W����炤
			int damage = 10;
			(*itrPin)->Damage(damage);

			//�G�̒e�����S
			enemyBullet[i]->Dead();
		}
	}

	//�s���������J�n����
	if (input->TriggerKey(DIK_A) || Xinput->TriggerButton(XInputManager::PAD_A))
	{
		CreatePinStart();
	}
	//�s���쐬��ԂȂ�쐬�𑱂���
	if (isCreatePin)
	{
		CreatePin();
	}

	//�s���X�V
	for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
	{
		//�X�V����
		(*itrPin)->Update();
	}

	//�Œ�G�X�V
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		(*itrFixedEnemy)->Update();
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

	//�p���[�A�b�v���X�V
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		//�X�V����
		(*itrLine)->Update(camera);

		//�X�e�b�v���̃v���C���[�Ƃ̓����蔻��
		if (!player->GetIsStep()) { continue; }

		//�Փ˗p�ɍ��W�Ɣ��a�A���̎n�_�ƏI�_���؂��
		XMFLOAT3 playerPos = player->GetPosition();
		float playerRadius = player->GetScale().x;
		XMFLOAT3 lineStartPoint = (*itrLine)->GetStartPoint();
		XMFLOAT3 lineEndPoint = (*itrLine)->GetEndPoint();

		//�Փ˔�����v�Z
		bool isCollision = Collision::CheckCircle2Line(
			playerPos, playerRadius, lineStartPoint, lineEndPoint);

		//�v���C���[�Ɛ����Փˏ�ԂłȂ���Δ�΂�
		if (!isCollision) { continue; }

		//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��z���~���o�������Ă��܂�����)
		if (player->IsKnowLine((*itrLine))) { continue; }

		//�z���~�𐶐�����
		XMFLOAT3 pos = player->GetPosition();
		absorptionCircles.push_back(AbsorptionCircle::Create(
			circleModel, pos));
	}

	if (input->TriggerKey(DIK_9)) 
	{
		//�z���~�𐶐�����
		XMFLOAT3 pos = player->GetPosition();
		absorptionCircles.push_back(AbsorptionCircle::Create(
			circleModel, pos));
	}

	//�Ռ��g�X�V
	for (auto itrShockWave = shockWaves.begin(); itrShockWave != shockWaves.end(); itrShockWave++)
	{
		//�X�V����
		(*itrShockWave)->Update();

		//�����蔻��p�ϐ�
		XMFLOAT3 wavePos = (*itrShockWave)->GetPosition();
		float waveSize = (*itrShockWave)->GetRadius();

		//�Ռ��g�ƃK�����̓����蔻��
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//�K�����������Ă��Ȃ���Δ�΂� �K�������X�|�[�������������΂� �K���������������������΂�
			if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//�Ռ��g�ƃK�������Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��_���[�W��H�炢�����Ă��܂�����)
			if ((*itrShockWave)->IsKnowGaruEnemy((*itrGaruEnemy))) { continue; }

			//�K�����Ƀ_���[�W�����킹��
			int damagePower = (*itrShockWave)->GetPower();
			(*itrGaruEnemy)->Damage(damagePower);
		}
		//�Ռ��g�ƃ`�����̓����蔻��
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
			float enemySize = (*itrCharo)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//�Ռ��g�ƃ`�������Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��_���[�W��H�炢�����Ă��܂�����)
			if ((*itrShockWave)->IsKnowCharo((*itrCharo))) { continue; }

			//�`�����Ƀ_���[�W�����킹��
			int damagePower = (*itrShockWave)->GetPower();
			(*itrCharo)->Damage(damagePower);
		}
		//�Ռ��g�ƃ|���^�̓����蔻��
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
			float enemySize = (*itrPorta)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//�Ռ��g�ƃ|���^���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��_���[�W��H�炢�����Ă��܂�����)
			if ((*itrShockWave)->IsKnowPorta((*itrPorta))) { continue; }

			//�|���^�Ƀ_���[�W�����킹��
			int damagePower = (*itrShockWave)->GetPower();
			(*itrPorta)->Damage(damagePower);
		}
		//�Ռ��g�ƓG�̒e�̓����蔻��
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//�e�������Ă��Ȃ���Δ�΂�
			if (!enemyBullet[i]->GetIsAlive()) { continue; }

			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, bulletPos, bulletSize);

			//�Ռ��g�ƓG�̒e���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�G�̒e������
			enemyBullet[i]->Dead();
		}
	}

	//�z���~�X�V
	for (auto itrAbsorptionCircle = absorptionCircles.begin(); itrAbsorptionCircle != absorptionCircles.end(); itrAbsorptionCircle++)
	{
		//�X�V����
		(*itrAbsorptionCircle)->Update();

		//�����蔻��p�ϐ�
		XMFLOAT3 circlePos = (*itrAbsorptionCircle)->GetPosition();
		float circleSize = (*itrAbsorptionCircle)->GetScale().x;

		//�z���~�ƃK�����̓����蔻��
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//�K�����������Ă��Ȃ���Δ�΂� �K�������X�|�[�������������΂� �K���������������������΂�
			if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				circlePos, circleSize, enemyPos, enemySize);

			//�z���~�ƃK�������Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�K���������S������
			(*itrGaruEnemy)->Dead();

			//�G�l���M�[�|�C���g�𑝂₷
			const int point = 2;
			energy->AddEnergyPoint(point);
		}
		//�z���~�ƃ`�����̓����蔻��
		for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end(); itrCharo++)
		{
			XMFLOAT3 enemyPos = (*itrCharo)->GetPosition();
			float enemySize = (*itrCharo)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				circlePos, circleSize, enemyPos, enemySize);

			//�z���~�ƃ`�������Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�`���������S������
			(*itrCharo)->Dead();

			//�G�l���M�[�|�C���g�𑝂₷
			const int point = 2;
			energy->AddEnergyPoint(point);
		}
		//�z���~�ƃ|���^�̓����蔻��
		for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end(); itrPorta++)
		{
			XMFLOAT3 enemyPos = (*itrPorta)->GetPosition();
			float enemySize = (*itrPorta)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				circlePos, circleSize, enemyPos, enemySize);

			//�z���~�ƃ|���^���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�|���^�����S������
			(*itrPorta)->Dead();

			//�G�l���M�[�|�C���g�𑝂₷
			const int point = 2;
			energy->AddEnergyPoint(point);
		}
		//�z���~�ƓG�̒e�̓����蔻��
		for (int i = 0; i < enemyBulletNum; i++)
		{
			//�e�������Ă��Ȃ���Δ�΂�
			if (!enemyBullet[i]->GetIsAlive()) { continue; }

			XMFLOAT3 bulletPos = enemyBullet[i]->GetPosition();
			float bulletSize = enemyBullet[i]->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				circlePos, circleSize, bulletPos, bulletSize);

			//�z���~�ƓG�̒e���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�G�̒e������
			enemyBullet[i]->Dead();

			//�G�l���M�[�|�C���g�𑝂₷
			const int point = 2;
			energy->AddEnergyPoint(point);
		}
	}

	//�K�����폜
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrGaruEnemy)->GetIsDelete())
		{
			//�G�l���M�[�|�C���g�𑝂₷
			const int point = 1;
			energy->AddEnergyPoint(point);

			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrGaruEnemy);
			itrGaruEnemy = garuEnemys.erase(itrGaruEnemy);
			continue;
		}
		//for������
		itrGaruEnemy++;
	}

	//�`�����폜
	for (auto itrCharo = charoEnemys.begin(); itrCharo != charoEnemys.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrCharo)->GetIsDelete())
		{
			//�G�l���M�[�|�C���g�𑝂₷
			const int point = 1;
			energy->AddEnergyPoint(point);

			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrCharo);
			itrCharo = charoEnemys.erase(itrCharo);
			continue;
		}
		//for������
		itrCharo++;
	}

	//�|���^�폜
	for (auto itrPorta = portaEnemys.begin(); itrPorta != portaEnemys.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrPorta)->GetIsDelete())
		{
			//�G�l���M�[�|�C���g�𑝂₷
			const int point = 1;
			energy->AddEnergyPoint(point);

			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrPorta);
			itrPorta = portaEnemys.erase(itrPorta);
			continue;
		}
		//for������
		itrPorta++;
	}

	//�s���폜
	for (auto itrPin = pins.begin(); itrPin != pins.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrPin)->GetIsDelete())
		{
			//�R�l�N�g�T�[�N�����폜����s�����g�p���Ă��邩�m�F
			for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
			{
				//�g�p���Ă�����R�l�N�g�T�[�N�����폜��ԂɃZ�b�g
				if ((*itrConnectCircle)->CheckUsePin(*itrPin))
				{
					(*itrConnectCircle)->SetDelete();
				}
			}

			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrPin);
			itrPin = pins.erase(itrPin);
			continue;
		}
		//for������
		itrPin++;
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

	//�p���[�A�b�v���폜
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrLine)->GetIsDelete())
		{
			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrLine);
			itrLine = powerUpLines.erase(itrLine);
			continue;
		}
		//for������
		itrLine++;
	}

	//�Ռ��g�폜
	for (auto itrShockWave = shockWaves.begin(); itrShockWave != shockWaves.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrShockWave)->GetIsDelete())
		{
			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrShockWave);
			itrShockWave = shockWaves.erase(itrShockWave);
			continue;
		}
		//for������
		itrShockWave++;
	}

	//�z���~�폜
	for (auto itrAbsorptionCircle = absorptionCircles.begin(); itrAbsorptionCircle != absorptionCircles.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrAbsorptionCircle)->GetIsDelete())
		{
			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrAbsorptionCircle);
			itrAbsorptionCircle = absorptionCircles.erase(itrAbsorptionCircle);
			continue;
		}
		//for������
		itrAbsorptionCircle++;
	}

	if (timeLimit->GetTime() <= 0)
	{
		if (powerUpLines.size() == 0)
		{
			DebugText::GetInstance()->Print("NotClear", 100, 600);
		}
		else
		{
			DebugText::GetInstance()->Print("Clear", 100, 600);
		}
	}

	if (bulletPowerLevel == 0) { DebugText::GetInstance()->Print("BulletPowerLevel 0", 100, 650); }
	else if (bulletPowerLevel == 1) { DebugText::GetInstance()->Print("BulletPowerLevel 1", 100, 650); }
	else if (bulletPowerLevel == 2) { DebugText::GetInstance()->Print("BulletPowerLevel 2", 100, 650); }
	else if (bulletPowerLevel == 3) { DebugText::GetInstance()->Print("BulletPowerLevel 3", 100, 650); }
	else if (bulletPowerLevel == 4) { DebugText::GetInstance()->Print("BulletPowerLevel 4", 100, 650); }
	else if (bulletPowerLevel == 5) { DebugText::GetInstance()->Print("BulletPowerLevel 5", 100, 650); }

	if (absorptionCircles.size() == 0) { DebugText::GetInstance()->Print("absorptionCircles 0", 100, 600); }
	else if (absorptionCircles.size() == 1) { DebugText::GetInstance()->Print("absorptionCircles 1", 100, 600); }
	else if (absorptionCircles.size() == 2) { DebugText::GetInstance()->Print("absorptionCircles 2", 100, 600); }
	else if (absorptionCircles.size() == 3) { DebugText::GetInstance()->Print("absorptionCircles 3", 100, 600); }
	else if (absorptionCircles.size() == 4) { DebugText::GetInstance()->Print("absorptionCircles 4", 100, 600); }
	else if (absorptionCircles.size() == 5) { DebugText::GetInstance()->Print("absorptionCircles 5", 100, 600); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("RSTICK:ChangeAngle", 1000, 150);
	DebugText::GetInstance()->Print("LB:Step", 1000, 200);
	DebugText::GetInstance()->Print("RB:BulletShot", 1000, 250);
	DebugText::GetInstance()->Print("A :SetPin", 1000, 300);

	//�������ԍX�V
	timeLimit->Update();

	//�G�t�F�N�g�̍X�V
	effects->Update(camera);

	//�w�i�X�V
	buckGround->Update();

	//�J�����X�V
	CameraUpdate(camera);

	//��ʘg�X�V
	frame->Update();

	input = nullptr;
}

void GameScene::Draw(ID3D12GraphicsCommandList* cmdList)
{
	{
		//�I�u�W�F�N�g�`��
		Object3d::PreDraw(cmdList);

		//��ʘg�`��
		frame->Draw();

		//�v���C���[�`��
		player->Draw();

		//���[�U�[�T�C�g�`��
		laserSite->Draw();

		//�Ռ��g�`��
		for (auto itrShockWave = shockWaves.begin(); itrShockWave != shockWaves.end(); itrShockWave++)
		{
			(*itrShockWave)->Draw();
		}

		//�s���`��
		for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
		{
			(*itrPin)->Draw();
		}

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
		//�Œ�G�`��
		for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
		{
			(*itrFixedEnemy)->Draw();
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

		//�z���~�`��
		for (auto itrAbsorptionCircle = absorptionCircles.begin(); itrAbsorptionCircle != absorptionCircles.end(); itrAbsorptionCircle++)
		{
			(*itrAbsorptionCircle)->Draw();
		}

		//�w�i
		buckGround->Draw();

		Object3d::PostDraw();


		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

		//�G�l���M�[�|�C���g�`��
		energy->Draw();

		//�������ԕ`��
		timeLimit->Draw();

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


	//�X�|�[���p�^�[��
	spawnTimer = 0;//�X�|�[���^�C�}�[������
}

void GameScene::PowerUpPlayerBullet()
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//�v���C���[����e�����痭�ߏ�Ԃ𖳌��ɂ���
	if (player->GetIsDamege())
	{
		//���߂��I��
		isBulletPowerUp = false;
		//���߃^�C�}�[��������
		bulletPowerUpTimer = 0;

		return;
	}

	//�{�^�����͂��I�������甭��
	if (!(input->PushKey(DIK_SPACE) || Xinput->PushButton(XInputManager::PAD_RB)))
	{
		//���߂��I��
		isBulletPowerUp = false;
		//���߃^�C�}�[��������
		bulletPowerUpTimer = 0;
		//�e�𔭎�
		ShotPlayerBullet(bulletPowerLevel);
		//�e�̈З͂������Ă����
		if (bulletPowerLevel >= 1)
		{
			//�G�l���M�[�|�C���g�������
			for (int i = 0; i < bulletPowerLevel; i++)
			{
				energy->UseTwentyPercent();
			}
		}

		return;
	}

	if (input->PushKey(DIK_SPACE) || Xinput->PushButton(XInputManager::PAD_RB))
	{
		//�З̓A�b�v�ɕK�v�ȃG�l���M�[�|�C���g�������Ă����痭�߉\
		const int percent = (bulletPowerLevel + 1) * 20;
		if (energy->CheckPercent(percent))
		{
			//���߃^�C�}�[���X�V
			bulletPowerUpTimer++;
		}

		//��莞�ԗ��߂���e�̈З͂��A�b�v������
		const int levelUpTime = 60;
		if (bulletPowerUpTimer >= levelUpTime)
		{
			bulletPowerLevel++;

			//���߃^�C�}�[��������
			bulletPowerUpTimer = 0;
		}
	}
}

void GameScene::ShotPlayerBullet(const int bulletPowerLevel)
{
	//�v���C���[�E�G�|���̍��W�Ɗp�x��e������
	XMFLOAT3 pos = player->GetWeaponPosition();
	XMFLOAT3 rota = player->GetWeaponRotation();

	//�e�̔��˂��v���C���[�E�G�|���̐^��ɐݒ�
	float angle = DirectX::XMConvertToRadians(rota.z + 90);
	pos.x += 8.0f * cosf(angle);
	pos.y += 8.0f * sinf(angle);

	//�e�̈З͂�ݒ�
	int power = 0;
	if (bulletPowerLevel == 0) { power = 10; }
	else if (bulletPowerLevel == 1) { power = 20; }
	else if (bulletPowerLevel == 2) { power = 30; }
	else if (bulletPowerLevel == 3) { power = 40; }
	else if (bulletPowerLevel == 4) { power = 50; }
	else if (bulletPowerLevel == 5) { power = 60; }

	//�e����
	for (int i = 0; i < playerBulletNum; i++)
	{
		//���˂���Ă������΂�
		if (playerBullet[i]->GetIsAlive()) { continue; }

		//�e����
		playerBullet[i]->BulletStart(pos, rota, power);

		//1���˂����甲����(��C�ɑS�Ă̒e�������Ă��܂�����)
		return;
	}
}

void GameScene::SpawnEnemyManager(bool isBossStage, int wave)
{
	//�{�X��ȊO�̎��̓J�E���g��i�߂�B�{�X��ɂȂ����琔�l�����Z�b�g����
	if (!isBossStage)
	{
		spawnTimer++;//�G�����J�E���g�X�V
	}
	else
	{
		spawnTimer = 0;//�G�����J�E���g�����Z�b�g
	}

	//WAVE���ɏo���Ԋu����
	if ((wave % 3) == 1)
	{
		spawnInterval = 300;
	}
	else if ((wave % 3) == 2)
	{
		spawnInterval = 450;
	}
	else if ((wave % 3) == 0)
	{
		spawnInterval = 600;
	}

	//spawnInterval�t���[�����ɓG����
	if (((spawnTimer + ((spawnInterval / 3) * 2)) % spawnInterval) == 0)
	{
		spawnPattern = ((spawnTimer + ((spawnInterval / 3) * 2)) / spawnInterval);//�X�|�[���p�^�[���v�Z

		//����X�|�[�����ɃZ�b�g����
		if (spawnPattern == 1)
		{
			spawnSet = 1;
			//spawnSet = (int)(rand() % 10) + 1;//�X�|�[���Z�b�g�v�Z
		}

		SpawnEnemyGroup(spawnPattern, spawnSet, (wave % 3));//�G����
	}
}

void GameScene::SpawnEnemyGroup(int spawnPattern, int spawnSet, int wave)
{
	//�������ɃX�|�[�����W�ƈړ���̍��W�����߂�(�K���^�ƃK���^�^)
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	//�������ɏ������W�ƈړ����������߂�(�|���^�ƃ`����)
	XMFLOAT3 startPos = {};
	float angle = 0;

	//�E�F�[�u�A�p�^�[���A�Z�b�g����o���p�^�[�����m��
	if (wave == 1)
	{
		if (spawnPattern == 1)
		{
			if (spawnSet == 1)
			{
				//��ɃK���^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -20;
				stayPos.y = 15;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 20;
				stayPos.y = 15;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 2)
		{
			if (spawnSet == 1)
			{
				//�����ɃK���^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -15;
				stayPos.y = -8;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = 15;
				stayPos.y = -8;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 3)
		{
			if (spawnSet == 1)
			{
				//���̃K���^�^1�́A�E�ɃK���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = -40;
				stayPos.y = -5;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 40;
				stayPos.y = -5;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 4)
		{
			if (spawnSet == 1)
			{
				//�ォ��|���^4��
				startPos = { 10, 65, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -10, 65, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 30, 65, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -30, 65, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 5)
		{
			if (spawnSet == 1)
			{
				//���ɃK���^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = -40;
				stayPos.y = -30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = 40;
				stayPos.y = -30;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 6)
		{
			if (spawnSet == 1)
			{
				//�E��ɃK���^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 60;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = 60;
				stayPos.y = 20;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 7)
		{
			if (spawnSet == 1)
			{
				//�E���ɃK���^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = 60;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = 60;
				stayPos.y = -20;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 8)
		{
			if (spawnSet == 1)
			{
				//�ォ��|���^4��
				startPos = { 115, 10, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 115, 30, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 115, -10, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 115, -30, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 9)
		{
			if (spawnSet == 1)
			{
				//�����ɃK���^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = -60;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -100;
				stayPos.x = -60;
				stayPos.y = -20;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 10)
		{
			if (spawnSet == 1)
			{
				//����ɃK���^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -60;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 100;
				stayPos.x = -60;
				stayPos.y = 20;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
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
		if (spawnPattern == 1)
		{
			if (spawnSet == 1)
			{
				//�E�ɃK���^2�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 80;
				stayPos.y = 50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 80;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 70;
				stayPos.y = 0;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//�E�ɃK���^1�́A�K���^�^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 110;
				stayPos.y = 30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 110;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 90;
				stayPos.y = 0;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 2)
		{
			if (spawnSet == 1)
			{
				//���ɃK���^2�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -80;
				stayPos.y = 50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -80;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -70;
				stayPos.y = 0;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//���ɃK���^1�́A�K���^�^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -110;
				stayPos.y = 30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -110;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -90;
				stayPos.y = 0;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 3)
		{
			if (spawnSet == 1)
			{
				//���E�ɃK���^�^1�́A���ɃK���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -90;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 90;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 0;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//���E�ɃK���^�^1�́A���ɃK���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -100;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 100;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 0;
				stayPos.y = -50;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 4)
		{
			if (spawnSet == 1)
			{
				//������|���^6�́A�`����1��
				startPos = { -165, 20, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, 40, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, 60, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, -20, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, -40, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, -60, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, 0, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 5)
		{
			if (spawnSet == 1)
			{
				//���ɃK���^2�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -120;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -110;
				stayPos.y = 20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -100;
				stayPos.y = 40;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//���ɃK���^2�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -140;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -130;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = -120;
				stayPos.y = -10;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 6)
		{
			if (spawnSet == 1)
			{
				//�E�ɃK���^2�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 120;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 110;
				stayPos.y = 20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -150;
				stayPos.x = 100;
				stayPos.y = 40;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//�E�ɃK���^2�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 140;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 130;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 120;
				stayPos.y = -10;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 7)
		{
			if (spawnSet == 1)
			{
				//��ɃK���^2�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 20;
				stayPos.y = 65;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -20;
				stayPos.y = 65;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 0;
				stayPos.y = 55;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//��ɃK���^2�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 30;
				stayPos.y = 35;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -30;
				stayPos.y = 35;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 0;
				stayPos.y = 25;
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 8)
		{
			if (spawnSet == 1)
			{
				//�ォ��|���^6�́A�`����2��
				startPos = { 30, 100, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 50, 100, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 70, 100, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -30, 100, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -50, 100, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -70, 100, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -165, 0, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
				startPos = { 165, 0, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 9)
		{
			if (spawnSet == 1)
			{
				//����ɃK���^4�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -135;
				stayPos.y = 60;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -105;
				stayPos.y = 60;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -120;
				stayPos.y = 50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -135;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				/*
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = -105;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
				*/
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 10)
		{
			if (spawnSet == 1)
			{
				//�E��ɃK���^4�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 135;
				stayPos.y = 60;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 105;
				stayPos.y = 60;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 120;
				stayPos.y = 50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 135;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				/*
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 150;
				stayPos.x = 105;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
				*/
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 11)
		{
			BossStageStart();
		}
	}
	else if (wave == 0)
	{
		if (spawnPattern == 1)
		{
			if (spawnSet == 1)
			{
				//�㉺�ɃK���^4�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 30;
				stayPos.y = 85;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -30;
				stayPos.y = 85;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 10;
				stayPos.y = 95;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -10;
				stayPos.y = 95;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 0;
				stayPos.y = 75;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				//�㉺�ɃK���^4�́A�K���^�^1��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 30;
				stayPos.y = -85;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -30;
				stayPos.y = -85;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 10;
				stayPos.y = -95;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -10;
				stayPos.y = -95;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 0;
				stayPos.y = -75;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 2)
		{
			if (spawnSet == 1)
			{
				//���ɃK���^5�́A�K���^�^3��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -150;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -150;
				stayPos.y = 20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -150;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -150;
				stayPos.y = -20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -150;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -130;
				stayPos.y = 30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -130;
				stayPos.y = 0;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -130;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 3)
		{
			if (spawnSet == 1)
			{
				//�E�ɃK���^5�́A�K���^�^3��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 150;
				stayPos.y = 40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 150;
				stayPos.y = 20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 150;
				stayPos.y = 0;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 150;
				stayPos.y = -20;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 150;
				stayPos.y = -40;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 130;
				stayPos.y = 30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 130;
				stayPos.y = 0;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 130;
				stayPos.y = -30;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 4)
		{
			if (spawnSet == 1)
			{
				//�ォ��|���^10�́A�`����2��
				startPos = { 90, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 110, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -90, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -110, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 30, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 50, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 70, 130, 0 }; angle = 150;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -30, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -50, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -70, 130, 0 }; angle = 210;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -70, -130, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
				startPos = { 70, -130, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 5)
		{
			if (spawnSet == 1)
			{
				//���ɃK���^6�́A�K���^�^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -140;
				stayPos.y = 50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -170;
				stayPos.y = 70;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -170;
				stayPos.y = 30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -140;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -170;
				stayPos.y = -70;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -170;
				stayPos.y = -30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -155;
				stayPos.y = 50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -155;
				stayPos.y = -50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 6)
		{
			if (spawnSet == 1)
			{
				//�E�ɃK���^6�́A�K���^�^2��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 140;
				stayPos.y = 50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 170;
				stayPos.y = 70;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 170;
				stayPos.y = 30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 140;
				stayPos.y = -50;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 170;
				stayPos.y = -70;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 170;
				stayPos.y = -30;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 155;
				stayPos.y = 50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 155;
				stayPos.y = -50;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 7)
		{
			if (spawnSet == 1)
			{
				//���ɃK���^6�́A�K���^�^4��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 50;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 70;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 90;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 60;
				stayPos.y = -60;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = 80;
				stayPos.y = -60;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -50;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -70;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -90;
				stayPos.y = -80;
				garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -60;
				stayPos.y = -60;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = -200;
				stayPos.x = -80;
				stayPos.y = -60;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 8)
		{
			if (spawnSet == 1)
			{
				//�ォ��`����2��
				startPos = { 50, 130, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
				startPos = { -50, 130, 0 };
				charoEnemys.push_back(Charo::Create(charoModel, startPos));
				//������|���^6��
				startPos = { -205, 40, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, 60, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, 80, 0 }; angle = 300;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, -40, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, -60, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { -205, -80, 0 }; angle = 240;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				//�E����|���^6��
				startPos = { 205, 40, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, 60, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, 80, 0 }; angle = 120;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, -40, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, -60, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
				startPos = { 205, -80, 0 }; angle = 60;
				portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 9)
		{
			if (spawnSet == 1)
			{
				//��ɃK���^�^6��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 80;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 100;
				stayPos.y = 90;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 120;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 70;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 50;
				stayPos.y = 90;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = 30;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 10)
		{
			if (spawnSet == 1)
			{
				//��ɃK���^�^6��
				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -80;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -100;
				stayPos.y = 90;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -120;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -70;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -50;
				stayPos.y = 90;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

				spawnPos.x = (float)(rand() % 200 - 100);
				spawnPos.y = 200;
				stayPos.x = -30;
				stayPos.y = 80;
				garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
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
			else if (spawnSet == 5)
			{

			}
		}
		else if (spawnPattern == 11)
		{
			BossStageStart();
		}
	}
}

void GameScene::SpawnGaruEnemy(int spawnPattern, int wave)
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

void GameScene::SpawnCharoPorta(int spawnPattern, int wave)
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

	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	//�����_������(�e�L�g�[�ɃK���^��2�̍~�点��)
	for (int i = 0; i < 2; i++)
	{
		spawnPos.x = (float)(rand() % 200 - 100);
		spawnPos.y = 100;
		stayPos.x = (float)(rand() % 180 - 90);
		stayPos.y = (float)(rand() % 110 - 55);
		garuEnemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}

	//�����_������(�e�L�g�[�ɃK���^�^��1�̍~�点��)
	spawnPos.x = (float)(rand() % 200 - 100);
	spawnPos.y = 100;
	stayPos.x = (float)(rand() % 180 - 90);
	stayPos.y = (float)(rand() % 110 - 55);
	garuEnemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));

	//�����_������(�e�L�g�[�Ƀ|���^��1�̍~�点��)
	//4�p�^�[���̃����_���Ń|���^�̏������W�ƈړ��������Z�b�g
	int startPosRand = rand() % 3;

	if (startPosRand == 0) { startPos = { -70, 65, 0 }; angle = 210; }
	else if (startPosRand == 1) { startPos = { -50, 65, 0 }; angle = 210; }
	else if (startPosRand == 2) { startPos = { -30, 65, 0 }; angle = 210; }
	else if (startPosRand == 3) { startPos = { -10, 65, 0 }; angle = 210; }
	portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));

	//�����_������(�e�L�g�[�Ƀ|���^��1�̍~�点��)
	//4�p�^�[���̃����_���Ń|���^�̏������W�ƈړ��������Z�b�g
	int startPosRand2 = rand() % 3;

	if (startPosRand2 == 0) { startPos = { 70, 65, 0 }; angle = 150; }
	else if (startPosRand2 == 1) { startPos = { 50, 65, 0 }; angle = 150; }
	else if (startPosRand2 == 2) { startPos = { 30, 65, 0 }; angle = 150; }
	else if (startPosRand2 == 3) { startPos = { 10, 65, 0 }; angle = 150; }
	portaEnemys.push_back(Porta::Create(portaModel, startPos, angle));

	/*
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
	*/
}

void GameScene::CreatePinStart()
{
	//�G�l���M�[�|�C���g���ő�l��20%�ȉ��Ȃ�֐��𔲂���
	if (!energy->CheckPercent(20)) { return; }

	//�v���C���[�Ɗ����s�����Փ˂��Ă�����֐��𔲂���
	for (auto itrPin = pins.begin(); itrPin != pins.end(); itrPin++)
	{
		//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
		XMFLOAT3 playerPos = player->GetPosition();
		float playerSize = player->GetScale().x;
		XMFLOAT3 pinPos = (*itrPin)->GetPosition();
		float pinSize = (*itrPin)->GetScale().x;

		//�Փ˔�����v�Z
		bool isCollision = Collision::CheckCircle2Circle(
			playerPos, playerSize, pinPos, pinSize);

		//�v���C���[�ƃs�����Փˏ�ԂȂ甲����
		if (isCollision) { return; }
	}

	//�s���쐬��Ԃɂ���
	isCreatePin = true;
	//�v���C���[�̍s�������S�Ɏ~�߂�
	player->SetIsStop(true);
	//�s���쐬�^�C�}�[��������
	createPinTimer = 0;
}

void GameScene::CreatePin()
{
	Input* input = Input::GetInstance();
	XInputManager* Xinput = XInputManager::GetInstance();

	//�{�^�����͂�r���ŏI���B�܂��͔�e������
	//����Ȃ��Ȃ�����s���쐬���I������
	if (!(input->PushKey(DIK_A) || Xinput->PushButton(XInputManager::PAD_A)) || player->GetIsDamege())
	{
		//�s���쐬��Ԃ��I��
		isCreatePin = false;
		//�v���C���[�̍s������������
		player->SetIsStop(false);
		//�s���쐬�^�C�}�[��������
		createPinTimer = 0;

		return;
	}
	else if (input->PushKey(DIK_A) || Xinput->PushButton(XInputManager::PAD_A))
	{
		//�s���쐬�^�C�}�[���X�V
		createPinTimer++;

		//�s���쐬�^�C�}�[���w��̎��Ԃ܂Ői�񂾂�
		const int createEndTime = 100;
		if (createPinTimer >= createEndTime)
		{
			//�V�����s�������
			pins.push_back(Pin::Create(happyModel, player->GetPosition()));
			//�V�����s������o��R�l�N�g�T�[�N���𐶐�
			auto itrPin = pins.end();
			itrPin--;
			connectCircles.push_back(PinCircle::Create(circleModel, *itrPin));

			//�s���쐬��Ԃ��I��
			isCreatePin = false;
			//�v���C���[�̍s������������
			player->SetIsStop(false);
			//�s���쐬�^�C�}�[��������
			createPinTimer = 0;
			//�G�l���M�[�|�C���g��20%����
			energy->UseTwentyPercent();
		}
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