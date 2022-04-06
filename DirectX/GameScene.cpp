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
	safe_delete(porutaModel);
	safe_delete(frameModel);

	//�v���C���[���
	safe_delete(player);

	//�v���C���[�e���
	for (int i = 0; i < playerBulletNum; i++)
	{
		safe_delete(playerBullet[i]);
	}

	//���[�U�[�T�C�g���
	safe_delete(laserSite);

	//�K�������
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		safe_delete(*itrGaruEnemy);
	}
	//�K�����̃��X�g���
	garuEnemys.clear();

	//�n�Q�����
	for (auto itrHageEnemy = hageEnemys.begin(); itrHageEnemy != hageEnemys.end(); itrHageEnemy++)
	{
		safe_delete(*itrHageEnemy);
	}
	//�n�Q���̃��X�g���
	hageEnemys.clear();

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

	//�X�R�A���
	safe_delete(score);

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
	porutaModel = Model::CreateFromOBJ("poruta");//�|���^�̃��f��
	frameModel = Model::CreateFromOBJ("frame");//�t���[���̃��f��

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
	Sprite::LoadTexture(2, L"Resources/number.png");

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

	//�X�R�A������
	score = Score::Create(2);

	//�G�t�F�N�g������
	effects = new StageEffect();
	effects->Initialize();
}

void GameScene::Update(Camera *camera)
{
	Input *input = Input::GetInstance();
	XInputManager *Xinput = XInputManager::GetInstance();


	//�Q�[���v���C�V�[��
	if (scene == SceneName::Game)
	{
		//�v���C���[�X�V
		player->Update();

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
				for (int i = 0; i < enemyBulletNum; i++)
				{
					//�e�����˂���Ă������΂�
					if (enemyBullet[i]->GetIsAlive()) { continue; }

					//�K�����̍��W�ƕW�I�̍��W�i�v���C���[�̍��W�j��e������
					XMFLOAT3 startPos = (*itrGaruEnemy)->GetPosition();
					XMFLOAT3 targetPos = player->GetPosition();

					//�e����
					enemyBullet[i]->BulletStart(startPos, targetPos);

					//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
					break;
				}
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

		//�n�Q������
		if (input->TriggerKey(DIK_LSHIFT) || Xinput->TriggerButton(XInputManager::PAD_LT))
		{
			//�n�Q�����X�|�[��
			SpawnHageEnemy(0);
		}

		//�n�Q���X�V
		for (auto itrHageEnemy = hageEnemys.begin(); itrHageEnemy != hageEnemys.end(); itrHageEnemy++)
		{
			//�X�V����
			(*itrHageEnemy)->Update();

			//�e���˃t���O��true�Ȃ�
			if ((*itrHageEnemy)->GetIsBulletShot())
			{
				for (int i = 0; i < enemyBulletNum; i++)
				{
					//�e�����˂���Ă������΂�
					if (enemyBullet[i]->GetIsAlive()) { continue; }

					//�n�Q���̍��W�ƕW�I�̍��W�i�v���C���[�̍��W�j��e������
					XMFLOAT3 startPos = (*itrHageEnemy)->GetPosition();
					XMFLOAT3 targetPos = player->GetPosition();

					//�e����
					enemyBullet[i]->BulletStart(startPos, targetPos);

					//1���˂����烋�[�v�𔲂���(��C�ɑS�Ă̒e�������Ă��܂�����)
					break;
				}
			}

			//�v���C���[�ƃn�Q���̓����蔻��
			//�v���C���[���_���[�W��ԂłȂ��Ȃ画�������
			if (!player->GetIsDamege())
			{
				//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 enemyPos = (*itrHageEnemy)->GetPosition();
				float enemySize = (*itrHageEnemy)->GetScale().x;
				XMFLOAT3 playerPos = player->GetPosition();
				float playerSize = player->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					enemyPos, enemySize, playerPos, playerSize);

				//�v���C���[�ƒe���Փˏ��
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

			//�e�ƃn�Q���̓����蔻��
			for (int i = 0; i < playerBulletNum; i++)
			{
				//�e�����ˏ�ԂłȂ���Δ�΂�
				if (!playerBullet[i]->GetIsAlive()) { continue; }

				//�Փ˗p�ɍ��W�Ɣ��a�̑傫�����؂��
				XMFLOAT3 bulletPos = playerBullet[i]->GetPosition();
				float bulletSize = playerBullet[i]->GetScale().x;
				XMFLOAT3 enemyPos = (*itrHageEnemy)->GetPosition();
				float enemySize = (*itrHageEnemy)->GetScale().x;

				//�Փ˔�����v�Z
				bool isCollision = Collision::CheckCircle2Circle(
					bulletPos, bulletSize, enemyPos, enemySize);

				//�n�Q���ƒe���Փˏ�ԂłȂ���Δ�΂�
				if (!isCollision) { continue; }

				//�e�͎��S
				playerBullet[i]->Dead();

				//�n�Q���̓_���[�W����炤
				int bulletPower = playerBullet[i]->GetPower();
				(*itrHageEnemy)->Damage(bulletPower);

				//�_���[�W�������Ă�HP���c���Ă������΂�
				if ((*itrHageEnemy)->GetHP() > 0) { continue; }

				//�n�Q����HP��0�ȉ��Ȃ̂Ŏ��S
				(*itrHageEnemy)->Dead();
			}
		}

		//�n�Q���폜
		for (auto itrHageEnemy = hageEnemys.begin(); itrHageEnemy != hageEnemys.end();)
		{
			//�폜�t���O��true�Ȃ�폜
			if ((*itrHageEnemy)->GetIsDelete())
			{
				//�v�f���폜�A���X�g���珜�O����
				safe_delete(*itrHageEnemy);
				itrHageEnemy = hageEnemys.erase(itrHageEnemy);
				continue;
			}
			//for������
			itrHageEnemy++;
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

		//�X�R�A�X�V
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
		score->Update();

		//	���̃E�F�[�u�Ƀ`�F���W
		if (input->TriggerKey(DIK_R) || Xinput->TriggerButton(XInputManager::PAD_START))
		{
			scene = SceneName::ChangeWave;
		}

		//�X�v���C�g�X�V
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
				HageEnemy::SetBulletShotRange({ frameLine.x + 10, frameLine.y + 5 });

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
					HageEnemy::SetBulletShotRange({ frameLine.x + 9, frameLine.y + 7 });
				}
				else if (wave % 3 == 0)
				{
					Player::SetMoveRange({ frameLine.x - 5, frameLine.y - 5 });
					PlayerBullet::SetDeadPos({ frameLine.x + 8, frameLine.y + 8 });
					EnemyBullet::SetDeadPos({ frameLine.x + 8, frameLine.y + 8 });
					HageEnemy::SetBulletShotRange({ frameLine.x + 8, frameLine.y + 8 });
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
		laserSite->Update(camera);
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
		//�X�R�A�X�V
		score->Update();
	}

	//�G�t�F�N�g�̍X�V
	effects->Update(camera);

	//�J�����X�V
	CameraUpdate(camera);

	//��ʘg�X�V
	frame->Update();
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

	//��ʘg�`��
	frame->Draw();

	//�v���C���[�`��
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
	//�K�����`��
	for (auto itrGaruEnemy = garuEnemys.begin(); itrGaruEnemy != garuEnemys.end(); itrGaruEnemy++)
	{
		(*itrGaruEnemy)->Draw();
	}
	//�n�Q���`��
	for (auto itrHageEnemy = hageEnemys.begin(); itrHageEnemy != hageEnemys.end(); itrHageEnemy++)
	{
		(*itrHageEnemy)->Draw();
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

	//�X�R�A�`��
	score->Draw();

	//�f�o�b�O�e�L�X�g�`��
	DebugText::GetInstance()->DrawAll(cmdList);

	Sprite::PostDraw();
}

void GameScene::SpawnEnemyManager()
{
	//�X�|�[���^�C�}�[�X�V
	spawnTimer++;

	if (spawnTimer == 50)
	{
		SpawnHageEnemy(1);
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
		SpawnHageEnemy(2);
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
		SpawnHageEnemy(3);
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

void GameScene::SpawnHageEnemy(int spawnPattern)
{
	//�������ɏ������W�ƈړ����������߂�
	XMFLOAT3 startPos = {};
	float angle = 0;

	if (spawnPattern == 0)
	{
		//4�p�^�[���̃����_���ŏ������W�ƈړ��������Z�b�g
		int posAngleRand = rand() % 4;
		if (posAngleRand == 0) { startPos = { 0, -65, 0 }; angle = 0; }
		else if (posAngleRand == 1) { startPos = { 115, 0, 0 }; angle = 90; }
		else if (posAngleRand == 2) { startPos = { 0, 65, 0 }; angle = 180; }
		else if (posAngleRand == 3) { startPos = { -115, 0, 0 }; angle = 270; }

		//20%�̊m���Ńn�Q�^�^�@80%�̊m���Ńn�Q�^�𐶐�
		int enemyKindRand = rand() % 5;
		if (enemyKindRand == 0)
		{
			hageEnemys.push_back(Hagetata::Create(happyModel, startPos, player->GetPosition()));
		}
		else
		{
			hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));
		}
	}
	else if (spawnPattern == 1)
	{
		//�ォ��5��
		startPos = { 0, 65, 0 }; angle = 180;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 10, 70, 0 }; angle = 180;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -10, 70, 0 }; angle = 180;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 20, 75, 0 }; angle = 180;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -20, 75, 0 }; angle = 180;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));
	}
	else if (spawnPattern == 2)
	{
		//�E����5��
		startPos = { 120, -10, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 115, -20, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 110, -30, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 115, -40, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 120, -50, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 120, 10, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 115, 20, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 110, 30, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 115, 40, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { 120, 50, 0 }; angle = 90;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));
	}
	else if (spawnPattern == 3)
	{
		//������5��
		startPos = { -120, -10, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -115, -20, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -110, -30, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -115, -40, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -120, -50, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -120, 10, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -115, 20, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -110, 30, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -115, 40, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));

		startPos = { -120, 50, 0 }; angle = 270;
		hageEnemys.push_back(Hageta::Create(porutaModel, startPos, angle));
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

void GameScene::CameraUpdate(Camera *camera)
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