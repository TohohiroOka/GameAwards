#include "GameScene.h"
#include "MainEngine.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "Collision.h"

#include "Garuta.h"
#include "Garutata.h"
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
	safe_delete(pBullModel);
	safe_delete(enemy01Model);
	safe_delete(enemyPoint01Model);
	safe_delete(enemy02Model);
	safe_delete(enemyPoint02Model);
	safe_delete(eBullModel);
	safe_delete(deadEnemyModel);
	safe_delete(hexagonModel);
	safe_delete(happyModel);

	//�v���C���[���
	safe_delete(player);

	//�v���C���[�e���
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}

	//���[�U�[�T�C�g���
	safe_delete(laserSite);

	//�G���
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		safe_delete(*itrEnemy);
	}
	//�G�̃��X�g���
	enemys.clear();

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

	//�G�t�F�N�g�̉��
	safe_delete(effects);
}

void GameScene::Initialize(Camera *camera)
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
	pBullModel = Model::CreateFromOBJ("playerbullet");//�v���C���[�̒e�̃��f��
	enemy01Model = Model::CreateFromOBJ("enemy");//�G01(�K���^)�̃��f��
	enemyPoint01Model = Model::CreateFromOBJ("garutaspown");//�G01(�K���^)�̏o���ʒu�̃��f��
	enemy02Model = Model::CreateFromOBJ("garutata");//�G02(�K���^�^)�̃��f��
	enemyPoint02Model = Model::CreateFromOBJ("garutataspown");//�G02(�K���^�^)�̏o���ʒu�̃��f��
	eBullModel = Model::CreateFromOBJ("enemybullet");//�G�̒e�̃��f��
	deadEnemyModel = Model::CreateFromOBJ("desenemy");//���񂾓G�̃��f��
	hexagonModel = Model::CreateFromOBJ("hexagon");//�Z�p�`�̃��f��
	happyModel = Model::CreateFromOBJ("happy");//�^�o�R���f��

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
	laserSite = LaserSite::Create();

	//�G�̒e����
	for (int i = 0; i < enemyBulletNum; i++)
	{
		enemyBullet[i] = EnemyBullet::Create(eBullModel);
	}

	//�Œ�I�u�W�F�N�g���Z�b�g
	SetFixedObject();

	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(1, L"Resources/kari.png");

	//�X�v���C�g����
	sprite = Sprite::Create(1);
	sprite->SetPosition({ 100, 100 });
	sprite->SetSize({ 100, 100 });
	sprite->SetTexSize({ 64, 64 });

	//�f�o�b�O�e�L�X�g����
	DebugText::GetInstance()->Initialize(0);

	//�T�E���h�p
	audio = new Audio();

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();
}

void GameScene::Update(Camera *camera)
{
	Input *input = Input::GetInstance();
	XInputManager *Xinput = XInputManager::GetInstance();

	//�v���C���[�X�V
	player->Update(effects);

	//���[�U�[�T�C�g�X�V
	laserSite->SetPosition(player->GetWeaponPosition(), player->GetWeaponRotation());
	laserSite->Update(camera);

	//�f�o�b�O�p���̐F
	for (auto itrLine = powerUpLines.begin(); itrLine != powerUpLines.end(); itrLine++)
	{
		(*itrLine)->SetColor({ 0.4f, 1, 0.2f, 1 });
	}

	//�v���C���[�e����
	bulletShotTimer--;
	if (bulletShotTimer <= 0 && (input->PushKey(DIK_SPACE) || Xinput->PushButton(XInputManager::PAD_RB)))
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

		//���̒e���˂܂ł̃^�C�}�[������������
		bulletShotTimer = 10;
	}

	//�v���C���[�e�X�V
	for (int i = 0; i < playerBulletNum; i++)
	{
		//�e�������Ă��Ȃ���Δ�΂�
		if (!playerBullet[i]->GetIsAlive()) { continue; }

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

	//�G����
	if (input->TriggerKey(DIK_RETURN) || Xinput->TriggerButton(XInputManager::PAD_RT))
	{
		//�G���X�|�[��
		SpawnEnemy();
	}

	//�G�X�V
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		//���݂��Ȃ��������΂�
		if (!(*itrEnemy)->GetIsExistence()) { continue; }

		//�X�V����
		(*itrEnemy)->Update(effects);

		//�m�b�N�o�b�N���I���A���݂��Ȃ��Ȃ�����
		if (!(*itrEnemy)->GetIsExistence())
		{
			//�m�b�N�o�b�N�I�����̍��W�ŁA���̎��S��Ԃ̓G�Ƃ̓����蔻������
			for (auto itrEnemy2 = enemys.begin(); itrEnemy2 != enemys.end(); itrEnemy2++)
			{
				//�Փˑ��肪���݂������Ă������΂�
				if ((*itrEnemy2)->GetIsExistence()) { continue; }

				//�������g�Ƃ̓����蔻��͍s��Ȃ�
				if (itrEnemy == itrEnemy2) { continue; }

				//�Փ˗p�ɓG1�ƓG2�̍��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 enemyPos1 = (*itrEnemy)->GetPosition();
				float enemyRadius1 = (*itrEnemy)->GetScale().x;
				XMFLOAT3 enemyPos2 = (*itrEnemy2)->GetPosition();
				float enemyRadius2 = (*itrEnemy2)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos1, enemyRadius1, enemyPos2, enemyRadius2);

				//�G1�ƓG2���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�G1�ƓG2�����폜��Ԃɂ���
				(*itrEnemy)->SetDelete();
				(*itrEnemy2)->SetDelete();
			}

			//�폜��ԂȂ炱�̐�̏������s��Ȃ�
			if ((*itrEnemy)->GetIsDelete()) { continue; }

			//�m�b�N�o�b�N�I�����̍��W�ŁA�Œ�I�u�W�F�N�g�Ɠ����蔻������
			for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
			{
				//�Փ˗p�ɓG�ƌŒ�I�u�W�F�N�g�̍��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
				float enemyRadius = (*itrEnemy)->GetScale().x;
				XMFLOAT3 fixedObjectPos = (*itrFixedObject)->GetPosition();
				float fixedObjectRadius = (*itrFixedObject)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemyRadius, fixedObjectPos, fixedObjectRadius);

				//�G�ƌŒ�I�u�W�F�N�g���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�폜��Ԃɂ���
				(*itrEnemy)->SetDelete();
			}

			//�폜��ԂȂ炱�̐�̏������s��Ȃ�
			if ((*itrEnemy)->GetIsDelete()) { continue; }

			//�I�u�W�F�N�g�̃��f����ύX����
			(*itrEnemy)->SetModel(deadEnemyModel);

			//�R�l�N�g�T�[�N���̔��a���Z�b�g����i �G�̑傫���~�i �|���ꂽ���̒e�̋��� / 4 �j�j
			float radius = (*itrEnemy)->GetScale().x * ((float)(*itrEnemy)->GetKillBulletPower() / 4);
			connectCircles.push_back(
				EnemyCircle::Create(circleModel, *itrEnemy, radius));

			//�G�̑��݂��Ȃ��Ȃ����̂Ŕ�΂�
			continue;
		}

		//�G�������Ă��Ȃ���Δ�΂� �G���X�|�[�������������΂� �G�����������������΂�
		if (!(*itrEnemy)->GetIsAlive() || (*itrEnemy)->GetIsDuringSpawn() || (*itrEnemy)->GetIsEscape()) { continue; }

		//�G�e���˃t���O��true�Ȃ�
		if ((*itrEnemy)->GetIsBulletShot())
		{
			for (int i = 0; i < enemyBulletNum; i++)
			{
				//�G�e�����˂���Ă������΂�
				if (enemyBullet[i]->GetIsAlive()) { continue; }

				//�G�̍��W�ƕW�I�̍��W�i�v���C���[�̍��W�j��e������
				XMFLOAT3 startPos = (*itrEnemy)->GetPosition();
				XMFLOAT3 targetPos = player->GetPosition();

				//�G�e����
				enemyBullet[i]->BulletStart(startPos, targetPos);

				//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
				break;
			}
		}

		//�v���C���[�ƓG�̓����蔻��
		//�v���C���[���_���[�W��ԂłȂ��Ȃ画�������
		if (!player->GetIsDamege())
		{
			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
			float enemySize = (*itrEnemy)->GetScale().x;
			XMFLOAT3 playerPos = player->GetPosition();
			float playerSize = player->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				enemyPos, enemySize, playerPos, playerSize);

			//�G�ƃv���C���[�e���Փˏ��
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

		//�e�ƓG�̓����蔻��
		for (int i = 0; i < playerBulletNum; i++)
		{
			//�e�����ˏ�ԂłȂ���Δ�΂�
			if (!playerBullet[i]->GetIsAlive()) { continue; }

			//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
			XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
			float bulletSize = playerBullet[i]->GetScale().x;
			XMFLOAT3 enemyPos = (*itrEnemy)->GetPosition();
			float enemySize = (*itrEnemy)->GetScale().x;

			//�Փ˔�����v�Z
			bool isCollision = Collision::CheckCircle2Circle(
				bulletPos, bulletSize, enemyPos, enemySize);

			//�G�ƒe���Փˏ�ԂłȂ���Δ�΂�
			if (!isCollision) { continue; }

			//�e�͎��S
			playerBullet[i]->Dead();

			//�G�̓_���[�W����炤
			int bulletPower = playerBullet[i]->GetPower();
			(*itrEnemy)->Damage(bulletPower);

			//�_���[�W�������Ă�HP���c���Ă������΂�
			if ((*itrEnemy)->GetHP() > 0) { continue; }

			//�G��HP��0�ȉ��Ȃ̂Ńm�b�N�o�b�N���J�n����
			float bulletAngle = playerBullet[i]->GetAngle();
			(*itrEnemy)->SetKnockBack(bulletAngle, bulletPower);
		}
	}

	//�G�폜
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end();)
	{
		//�폜�t���O��true�Ȃ�폜
		if ((*itrEnemy)->GetIsDelete())
		{
			//�R�l�N�g�T�[�N�����폜����G���g�p���Ă��邩�m�F
			for (auto itrConnectCircle = connectCircles.begin(); itrConnectCircle != connectCircles.end(); itrConnectCircle++)
			{
				//�g�p���Ă�����R�l�N�g�T�[�N�����폜��ԂɃZ�b�g
				if ((*itrConnectCircle)->CheckUseEnemy(*itrEnemy))
				{
					(*itrConnectCircle)->SetDelete();
				}
			}

			//�v�f���폜�A���X�g���珜�O����
			safe_delete(*itrEnemy);
			itrEnemy = enemys.erase(itrEnemy);
			continue;
		}
		//for������
		itrEnemy++;
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

		//�G�ƃv���C���[�e���Փˏ�ԂłȂ���Δ�΂�
		if (!isCollision) { continue; }

		//�G�e�͎��S
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
		//�T�C�Y�ύX��ԂłȂ��ꍇ�͔�΂�
		if (!(*itrConnectCircle)->GetIsChangeRadius()) { continue; }

		//�X�V
		(*itrConnectCircle)->Update();

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

	//�J�����Z�b�g
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

	//�X�v���C�g�X�V
	sprite->Update();


	if (player->GetHP() == 3) { DebugText::GetInstance()->Print("HP : 3", 100, 500); }
	else if (player->GetHP() == 2) { DebugText::GetInstance()->Print("HP : 2", 100, 500); }
	else if (player->GetHP() == 1) { DebugText::GetInstance()->Print("HP : 1", 100, 500); }
	else if (player->GetHP() == 0) { DebugText::GetInstance()->Print("HP : 0", 100, 500); }

	if (player->GetIsAlive()) { DebugText::GetInstance()->Print("PLAYER ALIVE", 100, 550); }
	else { DebugText::GetInstance()->Print("PLAYER DEAD", 100, 550); }

	if (playerBullet[0]->GetPower() == 10) { DebugText::GetInstance()->Print("POWER : 10", 100, 600); }
	else if (playerBullet[0]->GetPower() == 12) { DebugText::GetInstance()->Print("POWER : 12", 100, 600); }
	else if (playerBullet[0]->GetPower() == 14) { DebugText::GetInstance()->Print("POWER : 14", 100, 600); }
	else if (playerBullet[0]->GetPower() == 16) { DebugText::GetInstance()->Print("POWER : 16", 100, 600); }
	else if (playerBullet[0]->GetPower() == 18) { DebugText::GetInstance()->Print("POWER : 18", 100, 600); }
	else if (playerBullet[0]->GetPower() == 20) { DebugText::GetInstance()->Print("POWER : 20", 100, 600); }
	else if (playerBullet[0]->GetPower() == 22) { DebugText::GetInstance()->Print("POWER : 22", 100, 600); }
	else if (playerBullet[0]->GetPower() == 24) { DebugText::GetInstance()->Print("POWER : 24", 100, 600); }
	else if (playerBullet[0]->GetPower() == 26) { DebugText::GetInstance()->Print("POWER : 26", 100, 600); }
	else if (playerBullet[0]->GetPower() == 28) { DebugText::GetInstance()->Print("POWER : 28", 100, 600); }

	DebugText::GetInstance()->Print("LSTICK:PlayerMove", 1000, 100);
	DebugText::GetInstance()->Print("LB:Sneak", 1000, 150);
	DebugText::GetInstance()->Print("RB:BulletShot", 1000, 200);
	DebugText::GetInstance()->Print("ENTERKEY:CreateEnemy", 1000, 250);

	//�G�t�F�N�g�̍X�V
	effects->Update(camera);
}

void GameScene::Draw(ID3D12GraphicsCommandList *cmdList)
{
	//�X�v���C�g�w�ʕ`��
	//Sprite::PreDraw(cmdList);

	//sprite->Draw();
	//Sprite::PostDraw();

	//�G�t�F�N�g�̕`��
	effects->Draw(cmdList);

	//�I�u�W�F�N�g�`��
	Object3d::PreDraw(cmdList);

	//�I�u�W�F�N�g�`��
	player->Draw();

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
	//�G�`��
	for (auto itrEnemy = enemys.begin(); itrEnemy != enemys.end(); itrEnemy++)
	{
		(*itrEnemy)->Draw();
	}
	//�Œ�I�u�W�F�N�g�`��
	for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
	{
		(*itrFixedObject)->Draw();
	}

	Object3d::PostDraw();

	//��3d
	DrawLine3D::PreDraw(cmdList);
	
	//���[�U�[�T�C�g�`��
	laserSite->Draw();

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

	Object3d::PostDraw();


	//�X�v���C�g�O�ʕ`��
	Sprite::PreDraw(cmdList);

	//�f�o�b�O�e�L�X�g�`��
	DebugText::GetInstance()->DrawAll(cmdList);

	Sprite::PostDraw();
}

void GameScene::SpawnEnemy()
{
	//�������ɃX�|�[�����W�ƈړ���̍��W�����߂�
	XMFLOAT3 spawnPos = {};
	XMFLOAT3 stayPos = {};

	spawnPos.x = (float)(rand() % 200 - 100);
	spawnPos.y = 100;

	stayPos.x = (float)(rand() % 200 - 100);
	stayPos.y = (float)(rand() % 120 - 60);


	//20%�̊m���ŃK���^�^�@80%�̊m���ŃK���^�𐶐�
	int enemyKindRand = rand() % 5;
	if (enemyKindRand == 0)
	{
		enemys.push_back(Garutata::Create(enemy02Model, enemyPoint02Model, spawnPos, stayPos));
	}
	else
	{
		enemys.push_back(Garuta::Create(enemy01Model, enemyPoint01Model, spawnPos, stayPos));
	}
}

void GameScene::SetFixedObject()
{
	//�Œ�I�u�W�F�N�g����
	fixedObjects.push_back(FixedObject::Create(happyModel, { 20, 0, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 20, 19, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 39, 0, 0 }));
	fixedObjects.push_back(FixedObject::Create(happyModel, { 39, 19, 0 }));

	//�Œ�R�l�N�g�T�[�N���̔��a
	float circleRadius = 10.0f;

	//�Œ�I�u�W�F�N�g�𐶐��������R�l�N�g�T�[�N���𐶐�
	for (auto itrFixedObject = fixedObjects.begin(); itrFixedObject != fixedObjects.end(); itrFixedObject++)
	{
		connectCircles.push_back(StartSetCircle::Create(circleModel, (*itrFixedObject)->GetPosition(), circleRadius));
	}
}

void GameScene::CreatePowerUpLine(ConnectCircle *startPoint, ConnectCircle *endPoint)
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

	//�q���ꂽ���̎n�_�ƏI�_�̉~��傫������
	startPoint->BigRadius();
	endPoint->BigRadius();
}
