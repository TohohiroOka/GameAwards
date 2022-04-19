#include "ShockWave.h"
#include "SafeDelete.h"
#include "Easing.h"

ShockWave* ShockWave::Create(Model* waveModel, XMFLOAT3 position, int power)
{
	//�C���X�^���X�𐶐�
	ShockWave* instance = new ShockWave();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(waveModel, position, power)) {
		delete instance;
		assert(0);
	}

	return instance;
}

ShockWave::~ShockWave()
{
	//�I�u�W�F�N�g���
	safe_delete(shockWaveObject);

	//�Ռ��g���m���Ă���K�����̃��X�g���
	alreadyGaruEnemys.clear();
	//�Ռ��g���m���Ă���`�����̃��X�g���
	alreadyCharoEnemys.clear();
	//�Ռ��g���m���Ă���|���^�̃��X�g���
	alreadyPortaEnemys.clear();
}

bool ShockWave::Initialize(Model* waveModel, XMFLOAT3 position, int power)
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

	//���W���Z�b�g
	this->position = position;
	shockWaveObject->SetPosition(position);

	//�F�𐅐F�ɂ��Ă���
	shockWaveObject->SetColor({ 0, 1, 1, 1 });

	//�u���[����������
	shockWaveObject->SetBloom(true);

	//�U���͂�ݒ�
	this->power = power;

	return true;
}

void ShockWave::Update()
{
	//�Ռ��g���L����
	WaveSpread();

	//�I�u�W�F�N�g�X�V
	shockWaveObject->Update();
}

void ShockWave::Draw()
{
	//�I�u�W�F�N�g�X�V
	shockWaveObject->Draw();
}

void ShockWave::SetDelete()
{
	//�폜����
	isDelete = true;
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
	//�����ł��鎞��
	const int aliveTime = 20 + power;

	//��������̎��Ԃ��X�V
	aliveTimer++;

	//�L���鑬�x�̑��x�����񂾂񑬂����Ă���
	spreadSpeedAccle += 0.001f;
	//�L���鑬�x�����񂾂񑬂����Ă���
	spreadSpeed += spreadSpeedAccle;
	//�ǂ�ǂ񔼌a��傫�����Ă���
	radius += spreadSpeed;
	shockWaveObject->SetScale({ radius + 0.1f, radius + 0.1f, 1 });


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
		SetDelete();
	}
}