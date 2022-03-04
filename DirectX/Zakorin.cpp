#include "Zakorin.h"

Zakorin *Zakorin::Create(XMFLOAT2 position)
{
	//�C���X�^���X�𐶐�
	Zakorin *instance = new Zakorin();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(position)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool Zakorin::Initialize(XMFLOAT2 position)
{
	//�U�R�����X�v���C�g����
	enemySprite = Sprite::Create(1);
	if (enemySprite == nullptr) {
		return false;
	}

	//�������W�Z�b�g
	enemySprite->SetPosition(position);
	//�傫�����Z�b�g
	enemySprite->SetSize({ 20.0f, 20.0f });

	return true;
}

void Zakorin::Update()
{
	//�������̓G�̂ݍX�V
	if (isAlive) {

		XMFLOAT2 pos = enemySprite->GetPosition();
		pos.x++;
		pos.y++;
		enemySprite->SetPosition(pos);
	}
	//�X�v���C�g�X�V
	enemySprite->Update();
}
