#pragma once
#include "Object3d.h"
#include "GaruEnemy.h"
#include "Charo.h"
#include "Porta.h"

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
	/// �Ռ��g����
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="power">�З�</param>
	void WaveStart(XMFLOAT3 position, int power);

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// ��x����̔��������ׂɁA�����̃K������m���Ă��邩�ǂ������肷��
	/// </summary>
	bool IsKnowGaruEnemy(GaruEnemy* garuEnemy);

	/// <summary>
	/// ��x����̔��������ׂɁA�����̃`������m���Ă��邩�ǂ������肷��
	/// </summary>
	bool IsKnowCharo(Charo* charo);

	/// <summary>
	/// ��x����̔��������ׂɁA�����̃|���^��m���Ă��邩�ǂ������肷��
	/// </summary>
	bool IsKnowPorta(Porta* porta);

	//getter
	XMFLOAT3 GetPosition() { return shockWaveObject->GetPosition(); }
	float GetRadius() { return shockWaveObject->GetScale().x; }
	int GetPower() { return power; }
	bool GetIsAlive() { return isAlive; }

private:
	/// <summary>
	/// �Ռ��g���L����
	/// </summary>
	void WaveSpread();

private:
	//�Ռ��g�I�u�W�F�N�g
	Object3d* shockWaveObject = nullptr;
	//�Ռ����L���鑬�x
	float spreadSpeed = 0;
	//�U����
	int power = 0;
	//��������̎��ԃ^�C�}�[
	int aliveTimer = 0;
	//�����\����
	int aliveTime = 0;
	//�����Ă邩
	bool isAlive = false;
	//�Ռ��g���m���Ă���K����
	std::list <GaruEnemy*> alreadyGaruEnemys;
	//�Ռ��g���m���Ă���`����
	std::list <Charo*> alreadyCharoEnemys;
	//�Ռ��g���m���Ă���|���^
	std::list <Porta*> alreadyPortaEnemys;
};