#pragma once
#include "Object3d.h"

class Frame
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
	/// �v���C���[����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�v���C���[</returns>
	static Frame* Create(Model* model = nullptr);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Frame();

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
	/// �g�̃��C���̈ʒu�Ƙg�I�u�W�F�N�g�̑傫���ύX���Z�b�g
	/// </summary>
	/// <param name="nextFrameNum">���̉�ʃT�C�Y�ԍ�</param>
	void SetChangeFrameLine(char nextFrameNum);

	/// <summary>
	/// �g�̃��C���̈ʒu�ύX
	/// </summary>
	void ChangeFrameLine();

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
	/// �����ɉ����ĐF��ω�������
	/// </summary>
	/// <param name="alpha">�ω�����</param>
	void ChangeColor(float ratio);

	//getter
	XMFLOAT2 GetFrameLine() { return frameLine; }
	bool GetIsChangeFrameLine() { return isChangeFrameLine; }
	bool GetIsBreak() { return isBreak; }
	int GetHP() { return HP; }

private:
	//�g�I�u�W�F�N�g
	Object3d* frameObject = nullptr;
	//��ʏ�Ō����Ƃ��̘g�̃��C���̈ʒu
	XMFLOAT2 frameLine = { 196, 108 };
	//�g�I�u�W�F�N�g�̑傫���C�[�W���O�J�n
	XMFLOAT3 frameScaleEaseStart = {};
	//�g�I�u�W�F�N�g�̑傫���C�[�W���O�I��
	XMFLOAT3 frameScaleEaseEnd = {};
	//�g�̃��C���̈ʒu�C�[�W���O�J�n
	XMFLOAT2 frameLineEaseStart = {};
	//�g�̃��C���̈ʒu�C�[�W���O�I��
	XMFLOAT2 frameLineEaseEnd = {};
	//�g�̃��C���̈ʒu��ύX����
	bool isChangeFrameLine = false;
	//�g�̃��C���̈ʒu�C�[�W���O�^�C�}�[
	int frameLineEaseTimer = 0;
	//�g��HP
	int HP = 30;
	//�j�󂷂邩
	bool isBreak = false;
	//�F
	const XMFLOAT4 resetColor = { 1.0f, 0.1f, 0.1f, 1.0f };
};