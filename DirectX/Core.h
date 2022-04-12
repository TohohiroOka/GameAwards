#pragma once
#include "Object3d.h"

class Core
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
	/// �R�A����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�R�A</returns>
	static Core* Create(Model* model);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Core();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damagePower">�U���̋���</param>
	void Damage(int damagePower);

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// �X�|�[���J�n���Z�b�g
	/// </summary>
	/// <param name="spawnPosition">�X�|�[�����W</param>
	/// <param name="stayPosition">��~���W</param>
	void SetSpawn(XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

	/// <summary>
	/// �R�A�����񂾂Ƃ��ɏ������������Ȃ鏈��
	/// </summary>
	/// <param name="time">�o�ߎ���</param>
	void ScaleDown(float time);

	//getter
	XMFLOAT3 GetPosition() { return coreObject->GetPosition(); }
	XMFLOAT3 GetScale() { return coreObject->GetScale(); }
	bool GetIsAlive() { return isAlive; }
	bool GetIsSpawn() { return isDuringSpawn; }

private:
	/// <summary>
	/// �����ʒu�ɖ߂�����
	/// </summary>
	void Spawn();

private:
	//�R�A�I�u�W�F�N�g
	Object3d* coreObject = nullptr;
	//�����Ă��邩
	bool isAlive = true;
	//�X�|�[������
	bool isDuringSpawn = false;
	//�X�|�[���^�C�}�[
	int spawnTimer = 0;
	//�X�|�[�����W
	XMFLOAT3 spawnPosition = {};
	//��~���W
	XMFLOAT3 stayPosition = {};
	//�T�C�Y�ύX�p�X�P�[��
	XMFLOAT3 scale = { 5,5,1 };
};

