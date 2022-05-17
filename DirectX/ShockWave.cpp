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
	//�Ռ��g���m���Ă���G�̃��X�g���
	alreadyEnemys.clear();

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

void ShockWave::Reset()
{
	//�Ռ����L���鑬�x
	spreadSpeed = 0;
	//�З͒i�K
	powerLevel = 0;
	//��������̎��ԃ^�C�}�[
	aliveTimer = 0;
	//�����\����
	aliveTime = 0;
	//�����Ă邩
	isAlive = false;
	//�Ռ��g���m���Ă���G�̃��X�g���
	alreadyEnemys.clear();

	//�傫����0�ɖ߂�
	shockWaveObject->SetScale({ 0, 0, 1 });
}

void ShockWave::PlayerWaveStart(XMFLOAT3 position)
{
	//�����O���[�v�������Ռ��g�ɂ���
	group = ShockWaveGroup::PlayerWave;

	//�F�̃Z�b�g
	shockWaveObject->SetColor({ 0, 1, 1, 1 });

	//�L���鑬�x���Z�b�g
	spreadSpeed = 2.0f;

	//�З͂�ݒ�
	powerLevel = 1;

	//�����\���Ԃ��Z�b�g
	aliveTime = 20;

	//�Ռ��g���ˋ��ʏ���
	WaveStartCommon(position);
}

void ShockWave::LitteringWaveStart(XMFLOAT3 position)
{
	//�����O���[�v���|�C�̂ďՌ��g�ɂ���
	group = ShockWaveGroup::LitteringWave;

	//�F�̃Z�b�g
	shockWaveObject->SetColor({ 1, 1, 1, 1 });

	//�L���鑬�x���Z�b�g
	spreadSpeed = 1.5f;

	//�З͂�ݒ�
	powerLevel = 1;

	//�����\���Ԃ��Z�b�g
	aliveTime = 20;

	//�Ռ��g���ˋ��ʏ���
	WaveStartCommon(position);
}

void ShockWave::BigWaveStart(XMFLOAT3 position, int powerLevel)
{
	//�����O���[�v������Ռ��g�ɂ���
	group = ShockWaveGroup::BigWave;

	//�F�̃Z�b�g
	if (powerLevel == 1) { shockWaveObject->SetColor({ 0, 0, 1, 1 }); }
	else if (powerLevel == 2) { shockWaveObject->SetColor({ 1, 1, 0, 1 }); }
	else if (powerLevel == 3) { shockWaveObject->SetColor({ 1, 0, 0, 1 }); }
	else { return; }

	//�L���鑬�x���Z�b�g
	spreadSpeed = 3.0f;

	//�З͂�ݒ�(�ʏ킪1�Ȃ̂�2����n�߂�)
	this->powerLevel = powerLevel + 1;

	//�����\���Ԃ��Z�b�g
	aliveTime = 40;

	//�Ռ��g���ˋ��ʏ���
	WaveStartCommon(position);
}

void ShockWave::Dead()
{
	//�Ռ��g���m���Ă���G�̃��X�g���
	alreadyEnemys.clear();

	//�Ռ��g�𔭎ˏ�Ԃł͂Ȃ�����
	isAlive = false;
}

bool ShockWave::IsKnowEnemy(BaseEnemy* enemy)
{
	//�����̓G�����ɒm���Ă��邩�m�F
	for (auto itr = alreadyEnemys.begin(); itr != alreadyEnemys.end(); itr++)
	{
		//���ɒm���Ă�����true��Ԃ�
		if (enemy == (*itr))
		{
			return true;
		}
	}

	//�S�Ċm�F���Ă��m��Ȃ�������V���ɒǉ�����
	alreadyEnemys.push_front(enemy);

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
	//�З͔{����Ⴍ���Ă���
	powerMagnification = Easing::Lerp(1.0f, 0.2f, easeTimer);

	if (aliveTimer >= aliveTime / 2)
	{
		//�C�[�W���O�v�Z�p�̎���
		float colorTimer = (float)(aliveTimer - aliveTime / 2) / (aliveTime / 2);
		//�X�V�����F�̔������Z�b�g
		XMFLOAT4 color = shockWaveObject->GetColor();
		//�F�𔖂����Ă���
		color.w = Easing::OutQuint(1.0f, 0.1f, colorTimer);
		shockWaveObject->SetColor(color);
	}

	//�������Ԃ��w��̎��ԂɂȂ�����폜����
	if (aliveTimer >= aliveTime)
	{
		Dead();
	}
}

void ShockWave::WaveStartCommon(XMFLOAT3 position)
{
	//���ˈʒu��ݒ�
	shockWaveObject->SetPosition(position);
	//�傫����0�ɖ߂�
	shockWaveObject->SetScale({ 0, 0, 1 });
	//�З͔{����1���߂�
	powerMagnification = 1.0f;
	//��������̎��ԃ^�C�}�[������
	aliveTimer = 0;
	//���ˏ�Ԃɂ���
	isAlive = true;
}