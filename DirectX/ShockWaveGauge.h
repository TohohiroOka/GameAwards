#pragma once
#include"Sprite.h"

class ShockWaveGauge
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
	/// ����Ռ��g�p�Q�[�W����
	/// </summary>
	/// <returns>����Ռ��g�p�Q�[�W</returns>
	static ShockWaveGauge* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ShockWaveGauge();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int frameTexNum, int barTexNum);

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
	/// �Q�[�W�����Z�b�g
	/// </summary>
	void GaugeReset();

	/// <summary>
	/// �Q�[�W�|�C���g����
	/// </summary>
	void IncreasePoint();

	/// <summary>
	/// �Q�[�W�|�C���g����
	/// </summary>
	void DecreasePoint();

	//setter
	void SetIsUpdate(bool isUpdate) { this->isUpdate = isUpdate; }

	//getter
	int GetGaugeLevel() { return gaugeLevel; }

private:
	/// <summary>
	/// �o�[�̒�����ύX
	/// </summary>
	void ChangeLengthBar();

	/// <summary>
	/// �Q�[�W���x����ύX
	/// </summary>
	void ChangeGaugeLevel();

	/// <summary>
	/// �`����J�n(���X�^�[�g)����
	/// </summary>
	void DrawStart();

	/// <summary>
	/// �`�掞�Ԃ��J�E���g
	/// </summary>
	void CountDrawTimer();

	/// <summary>
	/// ���߂�����
	/// </summary>
	void Transparent();

private:
	//�Q�[�W(�g)�X�v���C�g
	Sprite* frameSprite = nullptr;
	//�Q�[�W(�o�[)�X�v���C�g
	Sprite* barSprite = nullptr;
	//�ő�Q�[�W�|�C���g
	const int gaugePointMax = 1000;
	//�Q�[�W�|�C���g
	int gaugePoint = 0;
	//�Q�[�W���x��
	int gaugeLevel = 0;
	//�O�̃t���[���̃Q�[�W���x��
	int oldGaugeLevel = 0;
	//�O�̃t���[���ƃQ�[�W���x�����Ⴄ��
	bool isChangeGaugeLevel = false;
	//�o�[�X�v���C�g�̒����ő�l
	const float lengthMax = 229;
	//�X�V���邩
	bool isUpdate = false;
	//�`�悷�邩
	bool isDraw = false;
	//�`�掞�ԃJ�E���g���邩
	bool isDrawTimeCount = false;
	//�`�掞�ԃJ�E���g�^�C�}�[
	int drawTimer = 0;
	//���߂����邩
	bool isTransparent = false;
	//���߂����鎞��
	int transparentTimer = 0;
};