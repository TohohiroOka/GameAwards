#pragma once
#include "Emitter.h"

class Camera;

class StageEffect
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public://�����o�֐�

	StageEffect() {};
	~StageEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="camera">�J�����N���X�̃C���X�^���X</param>
	void Update(Camera* camera);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �v���C���[�ړ����̃G�t�F�N�g�Z�b�g
	/// </summary>
	/// <param name="position">�v���C���[�̍��W</param>
	/// <param name="rotation">�v���C���[�̊p�x</param>
	void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// �G���|���ꂽ�Ƃ��̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�G�̍��W</param>
	/// <returns>������</returns>
	int SetEnemeyDead1(const XMFLOAT3 position);

private:

	//�v���C���[�ړ����̃G�t�F�N�g
	Emitter* playerMove = nullptr;
	//�o���Ԋu
	int playerMoveContro = 0;
	//�G���|���ꂽ�Ƃ��̃G�t�F�N�g
	Emitter* enemeyDead1 = nullptr;

};

