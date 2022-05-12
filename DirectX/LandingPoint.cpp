#include "LandingPoint.h"
#include "SafeDelete.h"
#include "Easing.h"

DirectX::XMFLOAT2 LandingPoint::moveRangeMin = {};
DirectX::XMFLOAT2 LandingPoint::moveRangeMax = {};

LandingPoint* LandingPoint::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	LandingPoint* instance = new LandingPoint();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
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
		//�I�u�W�F�N�g����
		pointCircleObject[i] = Object3d::Create(model);
		if (pointCircleObject[i] == nullptr) {
			return false;
		}

		//�傫�����Z�b�g
		pointCircleObject[i]->SetScale({ 4, 4, 1 });
	}
	return true;
}

void LandingPoint::Update(XMFLOAT3 playerPosition, XMFLOAT3 playerRotation)
{
	//������
	if (isCreate)
	{
		Create();
	}
	//�v���C���[�ɒǏ]������
	if (isChase)
	{
		Chase(playerPosition, playerRotation);
	}

	//�I�u�W�F�N�g�X�V
	for (int i = 0; i < pointNum; i++)
	{
		pointCircleObject[i]->Update();
	}
}

void LandingPoint::Draw()
{
	//�I�u�W�F�N�g�`��
	for (int i = 0; i < pointNum; i++)
	{
		//�����Ă��Ȃ���Ε`�悵�Ȃ�
		if (!isAlive[i]) { continue; }
		pointCircleObject[i]->Draw();
	}
}

void LandingPoint::Dead(int pointNum)
{
	//���S������
	isAlive[pointNum] = false;
}

void LandingPoint::Revive()
{
	//�S�Đ����Ԃ�
	for (int i = 0; i < pointNum; i++)
	{
		isAlive[i] = true;
	}

	//�Ǐ]��Ԃ�߂��Ă���
	isChase = true;

	//�����^�C�}�[��������
	createTimer = 0;
	//������Ԃɂ���
	isCreate = true;
}

bool LandingPoint::CheckAllDead()
{
	for (int i = 0; i < pointNum; i++)
	{
		//��ł������Ă����false
		if (isAlive[i]) { return false; }
	}

	//�S�Ď���ł�����true
	return true;
}

void LandingPoint::Create()
{
	//�������s������
	const int createTime = 30;

	//�^�C�}�[�X�V
	createTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)createTimer / createTime;

	for (int i = 0; i < pointNum; i++)
	{
		//�T�C�Y��ύX
		XMFLOAT3 scale = pointCircleObject[i]->GetScale();
		const float baseScale = 4.0f;
		scale.x = Easing::OutQuad(0, baseScale, easeTimer);
		scale.y = Easing::OutQuad(0, baseScale, easeTimer);
		//�X�V�����T�C�Y���Z�b�g
		pointCircleObject[i]->SetScale(scale);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (createTimer >= createTime)
	{
		//�����I��
		isCreate = false;
	}
}

void LandingPoint::Chase(XMFLOAT3 position, XMFLOAT3 rotation)
{
	for (int i = 0; i < pointNum; i++)
	{
		//�v���C���[�p�x��Ǐ]����
		XMFLOAT3 rota = rotation;
		pointCircleObject[i]->SetRotation(rota);

		float angle = DirectX::XMConvertToRadians(rota.z + 90 + (-30 + i * 30));

		//�v���C���[���痣�ꂽ�����ɐݒu����
		XMFLOAT3 pos;
		float distance = 60.0f;

		//�i�����[�v������邽�߂ɃJ�E���^�[��p��
		int count = 0;
		//�ǂɓ������Ă�����񂵑�����
		bool isRoopEnd = false;
		while (!isRoopEnd)
		{
			//�v���C���[�����苗������
			pos = position;
			pos.x += distance * cosf(angle);
			pos.y += distance * sinf(angle);

			bool isCollisionWall = false;
			if (pos.x <= moveRangeMin.x) { isCollisionWall = true; }
			else if (pos.x >= moveRangeMax.x) { isCollisionWall = true; }
			if (pos.y <= moveRangeMin.y) { isCollisionWall = true; }
			else if (pos.y >= moveRangeMax.y) { isCollisionWall = true; }

			//�ǂɓ������Ă����狗�����l�߂�
			if (isCollisionWall)
			{
				distance--;
			}
			//�ǂɓ������Ă��Ȃ������烋�[�v�I��
			else
			{
				isRoopEnd = true;
			}

			//�i�����[�v��h�����߂�100�񃋁[�v������I��
			count++;
			if (count >= 100)
			{
				isRoopEnd = true;
			}
		}

		//�X�V�������W���Z�b�g
		pointCircleObject[i]->SetPosition(pos);
	}
}
