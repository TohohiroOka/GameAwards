#include "LandingPoint.h"
#include "SafeDelete.h"

DirectX::XMFLOAT2 LandingPoint::moveRange = {};

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
	//�I�u�W�F�N�g�X�V
	for (int i = 0; i < pointNum; i++)
	{
		SetPosition(playerPosition, playerRotation);
		pointCircleObject[i]->Update();
	}
}

void LandingPoint::Draw()
{
	//�I�u�W�F�N�g�`��
	for (int i = 0; i < pointNum; i++)
	{
		pointCircleObject[i]->Draw();
	}
}

void LandingPoint::SetPosition(XMFLOAT3 position, XMFLOAT3 rotation)
{
	for (int i = 0; i < pointNum; i++)
	{
		//�v���C���[�p�x��Ǐ]����
		XMFLOAT3 rota = rotation;
		pointCircleObject[i]->SetRotation(rota);

		float angle = DirectX::XMConvertToRadians(rota.z + 90 + (-30 + i * 30));

		//�v���C���[���痣�ꂽ�����ɐݒu����
		XMFLOAT3 pos;
		float distance = 50.0f;

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
			if (pos.x <= -moveRange.x) { isCollisionWall = true; }
			else if (pos.x >= moveRange.x) { isCollisionWall = true; }
			if (pos.y <= -moveRange.y) { isCollisionWall = true; }
			else if (pos.y >= moveRange.y) { isCollisionWall = true; }

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
