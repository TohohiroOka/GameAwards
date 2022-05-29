#pragma once
#include "Emitter.h"
#include <array>

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
	/// �v���C���[�ړ����̃G�t�F�N�g�Z�b�g
	/// </summary>
	/// <param name="position">�v���C���[�̍��W</param>
	/// <param name="rotation">�v���C���[�̊p�x</param>
	static void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// �G���͂��������̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�G���W</param>
	/// <param name="power">�G���W</param>
	static void SetPushEnemyPower(const XMFLOAT3 position, const unsigned char power);

	/// <summary>
	/// �G���͂����ꂽ�Ƃ��̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�G���W</param>
	static void SetPushEnemy(const XMFLOAT3 position);

	/// <summary>
	/// �ǂ��j�󂳂ꂽ�Ƃ��̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�o�����W</param>
	static void SetWallBreak(const XMFLOAT3 position);

	/// <summary>
	/// ���ǂɓ����������̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�G�̍��W</param>
	/// <param name="direction">�ǂ̕��� ��->1/��->2/��->3/�E->4</param>
	static void SetWallHitEnemey(const XMFLOAT3 position, const unsigned char direction);

	/// <summary>
	/// �G�������Ƃ��̉��o
	/// </summary>
	/// <param name="position">�G�̍��W</param>
	static void SetDeleteEnemey(const XMFLOAT3 position);

	/// <summary>
	/// �񕜃t�B�[���h�̃G�t�F�N�g
	/// </summary>
	/// <param name="position">���W</param>
	static void SetHealField(const XMFLOAT3 position);

	/// <summary>
	/// �񕜃G�t�F�N�g
	/// </summary>
	/// <param name="position">�v���C���[���W</param>
	static void SetHeal(const XMFLOAT3 position);

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

	//�v���C���[�ړ����̃G�t�F�N�g
	static Emitter* generalEffect;
	//�o���Ԋu
	static int playerMoveControl;
	//�ǃI�u�W�F�N�g�n�̃p�[�e�B�N���e�N�X�`����
	static const int wallTexNum = 3;
	//�G��������񂾎��̃G�t�F�N�g
	static std::array<Emitter*, wallTexNum> wallEffect;
	//�G��e�������̃G�t�F�N�g
	static Emitter* pushEnemy;
	//�񕜃G�t�F�N�g
	static Emitter* heal;
	//�񕜃G�t�F�N�g�o������
	static int healControl;
	//�e����G�t�F�N�g
	static Emitter* pop;
	//�񕜃t�B�[���h�̃G�t�F�N�g�o���Ԋu
	static int healFieldControl;
};