#pragma once
#include "Object3d.h"

class Wall
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
	/// �ǐ���
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[</returns>
	static Wall* Create(Model* model = nullptr);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Wall();

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
	XMFLOAT2 GetWallLine() { return wallLine; }
	int GetHP() { return HP; }
	bool GetIsCreate() { return isCreate; }
	bool GetIsAlive() { return isAlive; }

private:
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
	//�ǃI�u�W�F�N�g
	Object3d* wallObject = nullptr;
	//��ʏ�Ō����Ƃ��̕ǂ̃��C���̈ʒu
	XMFLOAT2 wallLine = { 196, 108 };
	//��̍ő�HP
	const int baseMaxHP = 10;
	//�ǂ̍ő�HP
	int maxHP = 0;
	//�ǂ�HP
	int HP = 0;
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
	//�����Ă��邩
	bool isAlive = true;
};