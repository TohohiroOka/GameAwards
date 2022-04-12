#include "ShockWave.h"
#include "SafeDelete.h"

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

bool ShockWave::Update()
{
	//�Ռ��g���L����
	if (isSpreadWave >= 0)
	{
		isSpreadWave--;
		WaveSpread();
	}
	//�E�F�[�u���I�������A�b�v�f�[�g���~�߂�
	else {
		isSpreadWave = -1;
		return false;
	}

	//�I�u�W�F�N�g�X�V
	shockWaveObject->Update();

	return true;
}

void ShockWave::Draw()
{
	//�I�u�W�F�N�g�X�V
	shockWaveObject->Draw();
}

void ShockWave::Reset()
{
	//�Ռ��g�~�̔��a������
	radius = 0;
	//�Ռ����L���鑬�x������
	spreadSpeed = 0;
	//�Ռ����L���鑬�x�̑��x������
	spreadSpeedAccle = 0;
	//�Ռ��g���L���Ȃ�
	isSpreadWave = -1;
	//�I�u�W�F�N�g�X�V
	shockWaveObject->Update();
}

void ShockWave::SetWaveStart(XMFLOAT3 startPos)
{
	//���W���Z�b�g
	position = startPos;
	//���W�������̔��˒n�_�Ɏ����Ă���
	shockWaveObject->SetPosition(position);

	//���a��0�ɏ�����
	radius = 0;
	//�傫����0�ɂ���
	shockWaveObject->SetScale({ radius, radius, 1 });

	//�L���鑬�x�����������Ă���
	spreadSpeed = 1.0f;

	//�L���鑬�x�̑��x�����������Ă���
	spreadSpeedAccle = 0;

	//�Ռ��g���L���鎞�ԃZ�b�g
	isSpreadWave = 200;
}

void ShockWave::WaveSpread()
{
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
}