#pragma once
#include "Object3d.h"

class FixedEnemy
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
	/// �Œ�G����
	/// </summary>
	/// <param name="coreModel">�R�A�̃��f��</param>
	/// <param name="squareModel">�O�g�̃��f��</param>
	/// <param name="spawnPosition">�X�|�[�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>�K���^</returns>
	static FixedEnemy* Create(Model* coreModel, Model* squareModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~FixedEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="coreModel">�R�A�̃��f��</param>
	/// <param name="squareModel">�O�g�̃��f��</param>
	/// <param name="spawnPosition">�X�|�[�������W</param>
	/// <param name="stayPosition">��~���W</param>
	/// <returns>����</returns>
	bool Initialize(Model* coreModel, Model* squareModel, XMFLOAT3 spawnPosition, XMFLOAT3 stayPosition);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	/// <summary>
	/// �폜
	/// </summary>
	void SetDelete();

	/// <summary>
	/// �m�b�N�o�b�N�̏����Z�b�g
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <param name="power">�m�b�N�o�b�N�̋���</param>
	void SetKnockBack(float angle, int power);

	/// <summary>
	/// ���f�����Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { coreObject->SetModel(model); }

	/// <summary>
	/// �m�b�N�o�b�N���I������u�Ԃ�����
	/// </summary>
	/// <returns></returns>
	bool TriggerEndKnockBack();

	//getter
	XMFLOAT3 GetPosition() { return coreObject->GetPosition(); }
	XMFLOAT3 GetScale() { return coreObject->GetScale(); }
	bool GetIsAlive() { return isAlive; }
	bool GetIsDelete() { return isDelete; }
	bool GetIsExistence() { return isExistence; }
	bool GetIsDuringSpawn() { return isDuringSpawn; }
	int GetKillBulletPower() { return killBulletPower; }

protected:
	/// <summary>
	/// �X�|�[��
	/// </summary>
	void Spawn();

	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	void KnockBack();

	/// <summary>
	/// �O�g�̑傫����ύX
	/// </summary>
	void ChangeSquareSize();

protected:
	//�R�A�I�u�W�F�N�g
	Object3d* coreObject = nullptr;
	//�O�g�I�u�W�F�N�g
	Object3d* squareObject = nullptr;
	//�X�|�[�������W
	XMFLOAT3 spawnPosition = { 0, 0, 0 };
	//�ړ���̍��W
	XMFLOAT3 stayPosition = { 0, 0, 0 };
	//�����Ă��邩
	bool isAlive = true;
	//���o���s���Ă��邩
	bool isEffect = false;
	//�m�b�N�o�b�N����
	int effectCount = 0;
	//���݂��Ă��邩(�m�b�N�o�b�N���I��)
	bool isExistence = true;
	//�X�|�[������
	bool isDuringSpawn = true;
	//�X�|�[���^�C�}�[
	int spawnTimer = 0;
	//�m�b�N�o�b�N�^�C�}�[
	int knockBackTimer = 0;
	//�m�b�N�o�b�N�̊p�x
	float knockBackAngle = 0.0f;
	//�m�b�N�o�b�N�̋���
	int killBulletPower = 0;
	//�m�b�N�o�b�N���I������u�Ԃ�
	bool triggerEndKnockBack = false;
	//�O�g�̑傫���ύX�^�C�}�[
	int changeSquareSizeTimer = 0;
	//�O�g�̑傫����ύX���邩
	bool isChangeSquareSize = false;
	//�폜���邩
	bool isDelete = false;
};