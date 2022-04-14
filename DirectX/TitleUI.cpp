#include "TitleUI.h"
#include "SafeDelete.h"

TitleUI* TitleUI::Create(Model* RBModel)
{
	//�C���X�^���X�𐶐�
	TitleUI* instance = new TitleUI();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(RBModel)) {
		delete instance;
		assert(0);
	}

	return instance;
}

TitleUI::~TitleUI()
{
	safe_delete(RBObject);
	//safe_delete(arrowObject);
}

bool TitleUI::Initialize(Model* RBModel)
{
	//RB�I�u�W�F�N�g����
	RBObject = Object3d::Create(RBModel);
	if (RBObject == nullptr) {
		return false;
	}
	//�������W���Z�b�g
	RBObject->SetPosition({ 0, 0, 0 });
	RBObject->SetScale({ 2, 2, 1 });

	/*
	//���I�u�W�F�N�g����
	arrowObject = Object3d::Create(arrowModel);
	if (arrowObject == nullptr) {
		return false;
	}

	//�������W���Z�b�g
	arrowObject->SetPosition({ -10, -10, 0 });
	arrowObject->SetScale({ 2, 2, 1 });
	*/

	return true;
}

void TitleUI::Update(XMFLOAT3 playerPos)
{
	//RB�I�u�W�F�N�g���v���C���[�ɒǏ]������
	XMFLOAT3 RBpos = playerPos;
	RBpos.y -= 10;
	RBObject->SetPosition(RBpos);


	//�I�u�W�F�N�g�X�V
	RBObject->Update();
	//arrowObject->Update();
}

void TitleUI::Draw()
{
	//�I�u�W�F�N�g�`��
	RBObject->Draw();
	//arrowObject->Draw();
}
