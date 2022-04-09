#include "Frame.h"
#include "SafeDelete.h"
#include "Easing.h"

Frame *Frame::Create(Model *model)
{
	//�C���X�^���X�𐶐�
	Frame *instance = new Frame();
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

Frame::~Frame()
{
	safe_delete(frameObject);
}

bool Frame::Initialize(Model *model)
{
	//�g�I�u�W�F�N�g����
	frameObject = Object3d::Create();
	if (frameObject == nullptr) {
		return false;
	}

	//�����n�_�Ƒ傫���ƐF���Z�b�g
	frameObject->SetPosition({ 0, 0, 0 });
	frameObject->SetScale({ 5.9f, 6.2f, 1 });
	frameObject->SetColor({ 1, 0.1f, 0.1f, 1 });

	//���f�����Z�b�g
	if (model) {
		frameObject->SetModel(model);
	}

	return true;
}

void Frame::Update()
{
	//�g�̃��C���ύX��Ԃ̏ꍇ�̂�
	if (isChangeFrameLine)
	{
		//�g�̃��C���Ƙg�I�u�W�F�N�g�̑傫���ύX
		ChangeFrameLine();
	}

	//�I�u�W�F�N�g�X�V
	frameObject->Update();
}

void Frame::Draw()
{
	//�I�u�W�F�N�g�`��
	frameObject->Draw();
}

void Frame::SetChangeFrameLine(char nextFrameNum)
{
	//�����̔ԍ��ɍ��킹�ă��C���ƃI�u�W�F�N�g�̑傫����ύX����
	XMFLOAT2 line = {};
	XMFLOAT3 objectScale = {};
	if (nextFrameNum == 1)
	{
		line = { 100, 55 };
		objectScale = { 5.9f, 6.2f, 1 };
	}
	else if (nextFrameNum == 2)
	{
		line = { 151, 83 };
		objectScale = { 8.85f, 9.3f, 1 };
	}
	else if (nextFrameNum == 3)
	{
		line = { 202, 112 };
		objectScale = { 11.8f, 12.4f, 1 };
	}
	else
	{
		return;
	}

	//�ύX�O�̘g�I�u�W�F�N�g�̑傫�����Z�b�g
	frameScaleEaseStart = frameObject->GetScale();
	//�ύX��̘g�I�u�W�F�N�g�̑傫�����Z�b�g
	frameScaleEaseEnd = objectScale;
	//�ύX�O�̘g�̃��C�����Z�b�g
	frameLineEaseStart = frameLine;
	//�ύX��̘g�̃��C�����Z�b�g
	frameLineEaseEnd = line;
	//�g�̃��C���ύX�^�C�}�[��������
	frameLineEaseTimer = 0;
	//�g�̃��C����ύX���ɂ���
	isChangeFrameLine = true;
}

void Frame::ChangeFrameLine()
{
	//�g�̃��C���ύX
	if (isChangeFrameLine)
	{
		//�g�̃��C���ύX���s������
		const int changeTime = 100;

		//�g�̃��C���ύX�^�C�}�[�X�V
		frameLineEaseTimer++;

		//�C�[�W���O�v�Z�p�̎���
		float easeTimer = (float)frameLineEaseTimer / changeTime;

		//�C�[�W���O�Řg�̃��C���ύX
		XMFLOAT2 frameLine = {};
		frameLine.x = Easing::InQuint(frameLineEaseStart.x, frameLineEaseEnd.x, easeTimer);
		frameLine.y = Easing::InQuint(frameLineEaseStart.y, frameLineEaseEnd.y, easeTimer);
		//�g�̃��C�����X�V
		this->frameLine = frameLine;

		//�C�[�W���O�Řg�I�u�W�F�N�g�̑傫��
		XMFLOAT3 scale = {};
		scale.x = Easing::InQuint(frameScaleEaseStart.x, frameScaleEaseEnd.x, easeTimer);
		scale.y = Easing::InQuint(frameScaleEaseStart.y, frameScaleEaseEnd.y, easeTimer);
		//�g�I�u�W�F�N�g�̑傫�����X�V
		frameObject->SetScale(scale);

		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (frameLineEaseTimer >= changeTime)
		{
			//�g�̃��C���ύX��ԏI��
			isChangeFrameLine = false;
		}
	}
}