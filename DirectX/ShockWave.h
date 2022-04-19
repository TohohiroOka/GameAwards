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
	static ShockWave* Create(Model* waveModel, XMFLOAT3 position, int power);

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
	bool Initialize(Model* waveModel, XMFLOAT3 position, int power);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �폜
	/// </summary>
	void SetDelete();

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
	XMFLOAT3 GetPosition() { return position; }
	float GetRadius() { return radius; }
	int GetPower() { return power; }
	bool GetIsDelete() { return isDelete; }

private:
	/// <summary>
	/// �Ռ��g���L����
	/// </summary>
	void WaveSpread();

private:
	//�Ռ��g�I�u�W�F�N�g
	Object3d* shockWaveObject = nullptr;
	//���W
	XMFLOAT3 position = {};
	//�Ռ��g�~�̔��a
	float radius = 0;
	//�Ռ����L���鑬�x
	float spreadSpeed = 1;
	//�Ռ����L���鑬�x�̑��x
	float spreadSpeedAccle = 0;
	//�U����
	int power = 0;
	//��������̎��ԃ^�C�}�[
	int aliveTimer = 0;
	//�폜���邩
	bool isDelete = false;
	//�Ռ��g���m���Ă���K����
	std::list <GaruEnemy*> alreadyGaruEnemys;
	//�Ռ��g���m���Ă���`����
	std::list <Charo*> alreadyCharoEnemys;
	//�Ռ��g���m���Ă���|���^
	std::list <Porta*> alreadyPortaEnemys;
};