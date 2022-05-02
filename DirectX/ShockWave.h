#pragma once
#include "Object3d.h"
#include "BaseEnemy.h"

class ShockWave
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// �Ռ��g����
	/// </summary>
	/// <param name="waveModel">���f��</param>
	/// <returns>�Ռ��g</returns>
	static ShockWave* Create(Model* waveModel);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ShockWave();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="waveModel">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* waveModel);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[������Ԋu�ŏo��Ռ��g����
	/// </summary>
	void PlayerWaveStart(XMFLOAT3 position);

	/// <summary>
	/// �|�C�̂ďՌ��g����
	/// </summary>
	void LitteringWaveStart(XMFLOAT3 position);

	/// <summary>
	/// ����Ռ��g����
	/// </summary>
	/// <param name="position"></param>
	void BigWaveStart(XMFLOAT3 position, int powerLevel);

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// ��x����̔��������ׂɁA�����̓G��m���Ă��邩�ǂ������肷��
	/// </summary>
	bool IsKnowEnemy(BaseEnemy* enemy);


	//getter
	XMFLOAT3 GetPosition() { return shockWaveObject->GetPosition(); }
	float GetRadius() { return shockWaveObject->GetScale().x; }
	int GetPowerLevel() { return powerLevel; }
	bool GetIsAlive() { return isAlive; }

private:
	/// <summary>
	/// �Ռ��g���L����
	/// </summary>
	void WaveSpread();

	/// <summary>
	/// �Ռ��g���ˋ��ʏ���
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="power">�З͒i�K</param>
	void WaveStartCommon(XMFLOAT3 position, int powerLevel);

private:
	//�Ռ��g�I�u�W�F�N�g
	Object3d* shockWaveObject = nullptr;
	//�Ռ����L���鑬�x
	float spreadSpeed = 0;
	//�З͒i�K
	int powerLevel = 0;
	//��������̎��ԃ^�C�}�[
	int aliveTimer = 0;
	//�����\����
	int aliveTime = 0;
	//�����Ă邩
	bool isAlive = false;
	//�Ռ��g���m���Ă���G
	std::list <BaseEnemy*> alreadyEnemys;
};