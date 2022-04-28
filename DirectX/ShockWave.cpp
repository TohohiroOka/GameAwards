#include "ShockWave.h"
#include "SafeDelete.h"
#include "Easing.h"

ShockWave* ShockWave::Create(Model* waveModel)
{
	//�C���X�^���X�𐶐�
	ShockWave* instance = new ShockWave();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(waveModel)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ShockWave::~ShockWave()
{
	//�Ռ��g���m���Ă���K�����̃��X�g���
	alreadyGaruEnemys.clear();
	//�Ռ��g���m���Ă���`�����̃��X�g���
	alreadyCharoEnemys.clear();
	//�Ռ��g���m���Ă���|���^�̃��X�g���
	alreadyPortaEnemys.clear();

	//�I�u�W�F�N�g���
	safe_delete(shockWaveObject);
}

bool ShockWave::Initialize(Model* waveModel)
{
	//�Ռ��g�I�u�W�F�N�g����
	shockWaveObject = Object3d::Create();
	if (shockWaveObject == nullptr) {
		return false;
	}

	//���f�����Z�b�g
	if (waveModel) {
		shockWaveObject->SetModel(waveModel);
	}

	//�F�𐅐F�ɂ��Ă���
	shockWaveObject->SetColor({ 0, 1, 1, 1 });

	//�u���[����������
	shockWaveObject->SetBloom(true);


	return true;
}

void ShockWave::Update()
{
	//���˒��̂ݍX�V
	if (!isAlive) return;

	//�Ռ��g���L����
	WaveSpread();

	//�I�u�W�F�N�g�X�V
	shockWaveObject->Update();
}

void ShockWave::Draw()
{
	//���˒��̂ݕ`��
	if (!isAlive) return;

	//�I�u�W�F�N�g�X�V
	shockWaveObject->Draw();
}

void ShockWave::WaveStart(XMFLOAT3 position, int power)
{
	//���ˈʒu��ݒ�
	shockWaveObject->SetPosition(position);
	//�傫����0�ɖ߂�
	shockWaveObject->SetScale({ 0, 0, 1 });
	//�F�̔�����߂�
	shockWaveObject->SetColor({ 1, 1, 1, 1 });

	//�L���鑬�x��������
	spreadSpeed = 1.5f;
	//�З͂�ݒ�
	this->power = power;
	//��������̎��ԃ^�C�}�[������
	aliveTimer = 0;

	//�����\���Ԃ��Z�b�g
	aliveTime = 20 + power;

	//���ˏ�Ԃɂ���
	isAlive = true;
}

void ShockWave::Dead()
{
	//�Ռ��g���m���Ă���K�����̃��X�g���
	alreadyGaruEnemys.clear();
	//�Ռ��g���m���Ă���`�����̃��X�g���
	alreadyCharoEnemys.clear();
	//�Ռ��g���m���Ă���|���^�̃��X�g���
	alreadyPortaEnemys.clear();

	//�Ռ��g�𔭎ˏ�Ԃł͂Ȃ�����
	isAlive = false;
}

bool ShockWave::IsKnowGaruEnemy(GaruEnemy* garuEnemy)
{
	//�����̃K���������ɒm���Ă��邩�m�F
	for (auto itr = alreadyGaruEnemys.begin(); itr != alreadyGaruEnemys.end(); itr++)
	{
		//���ɒm���Ă�����true��Ԃ�
		if (garuEnemy == (*itr))
		{
			return true;
		}
	}

	//�S�Ċm�F���Ă��m��Ȃ�������V���ɒǉ�����
	alreadyGaruEnemys.push_front(garuEnemy);

	//�m��Ȃ������ꍇ��false
	return false;
}

bool ShockWave::IsKnowCharo(Charo* charo)
{
	//�����̃`���������ɒm���Ă��邩�m�F
	for (auto itr = alreadyCharoEnemys.begin(); itr != alreadyCharoEnemys.end(); itr++)
	{
		//���ɒm���Ă�����true��Ԃ�
		if (charo == (*itr))
		{
			return true;
		}
	}

	//�S�Ċm�F���Ă��m��Ȃ�������V���ɒǉ�����
	alreadyCharoEnemys.push_front(charo);

	//�m��Ȃ������ꍇ��false
	return false;
}

bool ShockWave::IsKnowPorta(Porta* porta)
{
	//�����̃|���^�����ɒm���Ă��邩�m�F
	for (auto itr = alreadyPortaEnemys.begin(); itr != alreadyPortaEnemys.end(); itr++)
	{
		//���ɒm���Ă�����true��Ԃ�
		if (porta == (*itr))
		{
			return true;
		}
	}

	//�S�Ċm�F���Ă��m��Ȃ�������V���ɒǉ�����
	alreadyPortaEnemys.push_front(porta);

	//�m��Ȃ������ꍇ��false
	return false;
}

void ShockWave::WaveSpread()
{
	//��������̎��Ԃ��X�V
	aliveTimer++;

	//�L���鑬�x�����񂾂�x�����Ă���
	spreadSpeed -= 0.001f * aliveTimer;
	const float spreadSpeedMin = 0.01f;
	if (spreadSpeed <= spreadSpeedMin)
	{
		spreadSpeed = spreadSpeedMin;
	}

	//�ǂ�ǂ񔼌a��傫�����Ă���
	XMFLOAT3 scale = shockWaveObject->GetScale();
	scale.x += spreadSpeed;
	scale.y += spreadSpeed;
	shockWaveObject->SetScale(scale);

	//��]������
	XMFLOAT3 rota = shockWaveObject->GetRotation();
	rota.z++;
	shockWaveObject->SetRotation(rota);


	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)aliveTimer / aliveTime;
	//�F�𔖂����Ă���
	float alpha = Easing::InCubic(1.0f, 0.1f, easeTimer);
	//�X�V�����F�̔������Z�b�g
	shockWaveObject->SetColor({ 1, 1, 1, alpha });


	//�������Ԃ��w��̎��ԂɂȂ�����폜����
	if (aliveTimer >= aliveTime)
	{
		Dead();
	}
}