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

public://�ÓI�����o�֐�

	/// <summary>
	/// �R�A���j�G�t�F�N�g�Z�b�g
	/// </summary>
	/// <param name="position">�o�����W</param>
	/// <returns>�i�݊���</returns>
	static float SetTitleCoreExplosion(const XMFLOAT3 position);

	/// <summary>
	/// �v���C���[�ړ����̃G�t�F�N�g�Z�b�g
	/// </summary>
	/// <param name="position">�v���C���[�̍��W</param>
	/// <param name="rotation">�v���C���[�̊p�x</param>
	static void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// �G���|���ꂽ�Ƃ��̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�G�̍��W</param>
	/// <returns>������</returns>
	static int SetEnemeyDead(const XMFLOAT3 position);

	/// <summary>
	/// �e���Ŏ��̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�e�̍��W</param>
	/// <param name="color">�F</param>
	static void SetPlayerBulletDelete(const XMFLOAT3 position, const XMFLOAT4 color);

	/// <summary>
	/// �����q���������̃G�t�F�N�g
	/// </summary>
	/// <param name="position_one">�n�_</param>
	/// <param name="position_two">�I�_</param>
	static void SetConnectLine(const XMFLOAT3 position_one, const XMFLOAT3 position_two);

	/// <summary>
	/// �G���͂����ꂽ�Ƃ��̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�G���W</param>
	/// <param name="radius">������΂��p�x�i���W�A���j</param>
	/// <param name="color">�F</param>
	static void SetPushEnemy(const XMFLOAT3 position, const float radius, const XMFLOAT4 color);

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

private:

	//�R�A���j�G�t�F�N�g
	static Emitter* titleCoreExplosion;
	//���j�G�t�F�N�g����
	static int explosionTime;
	//���j�G�t�F�N�g�ő厞��
	static const int explosionTimeMax = 100;
	//�v���C���[�ړ����̃G�t�F�N�g
	static Emitter* playerMove;
	//�o���Ԋu
	static int playerMoveContro;
	//�G���|���ꂽ�Ƃ��̃G�t�F�N�g
	static Emitter* enemeyDead;
	//�e���Ŏ��̃G�t�F�N�g
	static Emitter* playerBulletDelete;
	//�����q���������̃G�t�F�N�g
	static Emitter* connectLine;
	//�G��������񂾎��̃G�t�F�N�g
	static Emitter* pushEnemy;
};