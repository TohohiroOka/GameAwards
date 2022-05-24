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

private://�\����

	struct SMASH {
		XMFLOAT3 position = {};//�o���ʒu
		XMFLOAT3 velocity = {};//���x
		unsigned int power = 0;
		int maxTime = 0;
		int time = 0;//�G�t�F�N�g�̏o�鎞��
	};

public://�ÓI�����o�֐�

	/// <summary>
	/// �v���C���[�ړ����̃G�t�F�N�g�Z�b�g
	/// </summary>
	/// <param name="position">�v���C���[�̍��W</param>
	/// <param name="rotation">�v���C���[�̊p�x</param>
	static void SetPlayerMove(const XMFLOAT3 position, const XMFLOAT3 rotation);

	/// <summary>
	/// �G���͂����ꂽ�Ƃ��̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�G���W</param>
	/// <param name="power">�G���W</param>
	static void SetPushEnemy(const XMFLOAT3 position, const unsigned char power);

	/// <summary>
	/// �ǂ��j�󂳂ꂽ�Ƃ��̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�o�����W</param>
	static void SetWallBreak(const XMFLOAT3 position);

	/// <summary>
	/// �ǂɓ����������̃G�t�F�N�g
	/// </summary>
	/// <param name="position">�o�����W</param>
	/// <param name="power">�З�</param>
	static void SetSmash(const XMFLOAT3 position, const  unsigned int power);

public://�����o�֐�

	StageEffect() {};
	~StageEffect();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �ǂɓ����������̏����X�V
	/// </summary>
	void smashUpdate();

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
	static int playerMoveContro;
	//�ǃI�u�W�F�N�g�n�̃p�[�e�B�N���e�N�X�`����
	static const int wallTexNum = 3;
	//�G��������񂾎��̃G�t�F�N�g
	static std::array<Emitter*, wallTexNum> wallEffect;
	//�X�}�b�V�����̃G�t�F�N�g
	static Emitter* smash;
	//�X�}�b�V���G�t�F�N�g�̏��
	static std::forward_list<SMASH> smashInfo;
};