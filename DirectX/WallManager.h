#pragma once
#include "WallObject.h"

class WallManager
{
private:// �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:

	//�estep���Ƃ̃I�u�W�F�N�g��
	enum WALL_STEP {
		step1 = 100,
		step2 = 50,
		step3 = 30,
	};

	//�ǂ̏��
	struct STATUS
	{
		bool isAlive = false;//�����Ă��邩
		int hp = 10;//�q�b�g�|�C���g
		WALL_STEP wallNum = WALL_STEP::step1;//�ǃI�u�W�F�N�g�̌�
	};

public:

	/// <summary>
	/// �ǐ���
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static WallManager* Create();

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~WallManager();

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
	/// �_���[�W��^����
	/// </summary>
	/// <param name="damagePower">�_���[�W��^����З�</param>
	void Damage(int damagePower);

	/// <summary>
	/// �j��
	/// </summary>
	void Break();

	/// <summary>
	/// �x�e��Ԃɂ���
	/// </summary>
	void SetBreakTime();

	/// <summary>
	/// �V���ȕǐ�����Ԃɂ���
	/// </summary>
	void SetCreateWall();

	/// <summary>
	/// �j�󂳂ꂽ�u�Ԃ�
	/// </summary>
	/// <returns></returns>
	bool GetTriggerBreak();

	/// <summary>
	/// ���U���g�V�[���p�ɓ�������Ԃɂ���
	/// </summary>
	void SetChangeResult();

	//getter
	int GetHP() { return status.hp; }
	bool GetIsCreate() { return isCreate; }
	bool GetIsAlive() { return status.isAlive; }
	void SetEffect() { isSetEffect = 1; }

private:

	/// <summary>
	/// ���f���̓ǂݍ���
	/// </summary>
	void LoadModel();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// ���o�̃Z�b�g
	/// </summary>
	void SetUpEffect();

	/// <summary>
	/// �ǔj��ォ�琶���܂ł̋x�e����
	/// </summary>
	void BreakTime();

	/// <summary>
	/// �V���ȕǐ���
	/// </summary>
	void CreateWall();

	/// <summary>
	/// HP�����ɉ����ĐF��ω�������
	/// </summary>
	void ChangeColor();

	/// <summary>
	/// ���U���g�V�[���p�ɓ�����
	/// </summary>
	void ChangeResult();

private:

	//���f��
	Model* model[10] = { nullptr };
	//�ǂ̏��
	STATUS status;
	//��̍ő�HP
	const int baseMaxHP = 10;
	//�ǂ̍ő�HP
	int maxHP = 0;
	//�x�e���ԃ^�C�}�[
	int breakTimer = 0;
	//�x�e����
	bool isBreakTime = false;
	//�ǐ��������T�C�Y
	XMFLOAT3 createStartScale = {};
	//�ǐ����ŏI�T�C�Y
	XMFLOAT3 createEndScale = {};
	//�ǐ����^�C�}�[
	int createTimer = 0;
	//�ǐ�����
	int createCount = 0;
	//�ǐ�������
	bool isCreate = false;
	//���U���g�V�[���p�ɓ����������ԃ^�C�}�[
	int changeResultTimer = 0;
	//�ǂ����U���g�V�[���p�ɓ������Ă��邩
	bool isChangeResult = false;
	//�󂳂ꂽ��
	bool isBreak = false;
	//�I�u�W�F�N�g�̃C���X�^���X
	std::list<std::unique_ptr<WallObject>> object;
	//���o�Z�b�g���̃C�e���[�^
	std::list<std::unique_ptr<WallObject>>::iterator nowItr;
	//�I�u�W�F�N�g�ւ̉��o�t���O�Z�b�g�p
	unsigned char isSetEffect = 0;
	//���o�J�n����̕b��
	int effectTime = 0;
};