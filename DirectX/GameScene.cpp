#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"
#include "Tuff.h"

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
	safe_delete(RBbuttonModel);

	//�v���C���[���
	safe_delete(player);
	//�Ռ��g���
	for (int i = 0; i < shockWaveNum; i++)
	{
		safe_delete(shockWave[i]);
	}

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

	//�Œ�G���
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		safe_delete(*itrFixedEnemy);
	}
	//�Œ�G�̃��X�g���
	fixedEnemys.clear();


	//��ʘg���
	safe_delete(frame);

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

	//�Ռ��g����
	for (int i = 0; i < shockWaveNum; i++)
	{
		shockWave[i] = ShockWave::Create(waveModel);
	}

	//�{�X����
	bossEnemy[0] = Tuff::Create(tuffModel);

	//�G�̒e����
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//��ʘg����
	frame = Frame::Create(frameModel);
	XMFLOAT2 frameline = frame->GetFrameLine();
	Player::SetMoveRange(frameline);


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

	//��ʘg�X�V
	frame->Update();

	//�v���C���[�X�V
	player->Update();

	//�Ռ��g����
	if (player->GetIsShockWaveStart())
	{
		ShockWaveStart(10);
	}
	//�Ռ��g�X�V
	for (int i = 0; i < shockWaveNum; i++)
	{
		//�X�V����
		shockWave[i]->Update();

		//�Ռ��g�����ˏ�ԂłȂ���Δ�΂�
		if (!shockWave[i]->GetIsAlive()) { continue; }

		//�����蔻��p�ϐ�
		XMFLOAT3 wavePos = shockWave[i]->GetPosition();
		float waveSize = shockWave[i]->GetRadius();

		//�Ռ��g�ƃK�����̓����蔻��
		for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
		{
			//�K�����������Ă��Ȃ���Δ�΂� �K�������X�|�[�������������΂� �K���������������������΂�
			if (!(*itrGaruEnemy)->GetIsAlive() || (*itrGaruEnemy)->GetIsDuringSpawn() || (*itrGaruEnemy)->GetIsEscape()) { continue; }

			XMFLOAT3 enemyPos = (*itrGaruEnemy)->GetPosition();
			float enemySize = (*itrGaruEnemy)->GetScale().x - 0.2f;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				wavePos, waveSize, enemyPos, enemySize);

			//�Ռ��g�ƃK�������Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//���ɏՓ˂������Ƃ����邩�m�F(�Փ˒��_���[�W��H�炢�����Ă��܂�����)
			if (shockWave[i]->IsKnowGaruEnemy((*itrGaruEnemy))) { continue; }

			//�K�������m�b�N�o�b�N�Ŕ�΂�
			int power = shockWave[i]->GetPower();
			(*itrGaruEnemy)->SetKnockBack(wavePos, power);
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
			if (shockWave[i]->IsKnowCharo((*itrCharo))) { continue; }

			//�`�����Ƀ_���[�W�����킹��
			int damagePower = shockWave[i]->GetPower();
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
			if (shockWave[i]->IsKnowPorta((*itrPorta))) { continue; }

			//�|���^�Ƀ_���[�W�����킹��
			int damagePower = shockWave[i]->GetPower();
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

		//�g�ɓ�����������������
		if ((*itrGaruEnemy)->IsCollisionFrame(frame->GetFrameLine()))
		{
			//���S
			(*itrGaruEnemy)->Dead();

			//�g�ɂ��_���[�W��^����
			frame->Damage(10);
		}

		//�v���C���[�ƃK�����̓����蔻��
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
			if (player->GetIsTackle())
			{
				player->SetKnockback();

				//�K�������m�b�N�o�b�N�Ŕ�΂�
				XMFLOAT3 playerPos = player->GetPosition();
				int power = 10;
				(*itrGaruEnemy)->SetKnockBack(playerPos, power);
			}
			else
			{
				//�v���C���[���_���[�W��ԂłȂ��Ȃ�
				if (!player->GetIsDamege())
				{
					//�v���C���[�̓_���[�W����炤
					player->Damage();
					player->SetKnockback();

					isShake = true;
				}
			}
		}
	}

	//�`�����|���^����
	//if (input->TriggerKey(DIK_LSHIFT) || Xinput->TriggerButton(XInputManager::PAD_LT))
	/*{
		SpawnCharoPorta(0, wave);
	}*/

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
			}
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

				//��ʂ��V�F�C�N������
				isShake = true;
			}
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

					//��ʂ��V�F�C�N������
					isShake = true;
				}
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
			}
		}
	}

	//�Œ�G�X�V
	for (auto itrFixedEnemy = fixedEnemys.begin(); itrFixedEnemy != fixedEnemys.end(); itrFixedEnemy++)
	{
		(*itrFixedEnemy)->Update();
	}

	//�K�����폜
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrGaruEnemy)->GetIsDelete())
		{
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
			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrPorta);
			itrPorta = portaEnemys.erase(itrPorta);
			continue;
		}
		//for������
		itrPorta++;
	}


	if (frame->GetHP() == 30) { DebugText::GetInstance()->Print("WALL HP:30", 100, 500); }
	else if (frame->GetHP() == 20) { DebugText::GetInstance()->Print("WALL HP:20", 100, 500); }
	else if (frame->GetHP() == 10) { DebugText::GetInstance()->Print("WALL HP:10", 100, 500); }
	else if (frame->GetHP() <= 0) { DebugText::GetInstance()->Print("WALL BREAK", 100, 500); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("LB:Tackle", 1000, 150);
	DebugText::GetInstance()->Print("RB:ShockWave", 1000, 200);
	DebugText::GetInstance()->Print("RT:SpawnEnemy", 1000, 250);

	//�G�t�F�N�g�̍X�V
	effects->Update(camera);

	//�w�i�X�V
	buckGround->Update();

	//�J�����X�V
	CameraUpdate(camera);

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

		//�Ռ��g�`��
		for (int i = 0; i < shockWaveNum; i++)
		{
			shockWave[i]->Draw();
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

		//�w�i
		buckGround->Draw();

		Object3d::PostDraw();


		//�X�v���C�g�O�ʕ`��
		Sprite::PreDraw(cmdList);

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
	Player::SetMoveRange(frameLine);
	EnemyBullet::SetDeadPos({ frameLine.x + 10, frameLine.y + 5 });
	Porta::SetReflectionLine({ frameLine.x - 3, frameLine.y - 2 });
	BossEnemy::SetFrameLine({ frameLine.x - 3, frameLine.y - 2 });


	//�X�|�[���p�^�[��
	spawnTimer = 0;//�X�|�[���^�C�}�[������
}

void GameScene::ShockWaveStart(const int shockWavePower)
{
	//�v���C���[�̍��W���甭�˂���
	XMFLOAT3 pos = player->GetPosition();

	//���˂���Ռ��g��ݒ�
	for (int i = 0; i < shockWaveNum; i++)
	{
		//���˂���Ă������΂�
		if (shockWave[i]->GetIsAlive()) { continue; }

		//�Ռ��g����
		shockWave[i]->WaveStart(pos, shockWavePower);

		//��ʃV�F�C�N
		isShake = true;

		//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̏Ռ��g�𔭎˂��Ă��܂�����)
		break;
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
		camera->StartCameraShake(3);
		ShakeTime++;

		//�V�F�C�N�J�n�����莞�Ԍo������
		if (ShakeTime > 20) {
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