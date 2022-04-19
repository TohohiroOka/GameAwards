#pragma once
#include"Sprite.h"

class Energy
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
	/// �G�l���M�[����
	/// </summary>
	/// <returns>�G�l���M�[</returns>
	static Energy* Create(int frameTexNum, int barTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Energy();

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
	/// �G�l���M�[�|�C���g�𑝂₷
	/// </summary>
	void AddEnergyPoint(const int addPoint);

	/// <summary>
	/// �G�l���M�[�|�C���g������
	/// </summary>
	void LoseEnergyPoint();

	/// <summary>
	/// �ő�l��20%�̃|�C���g���g�p����
	/// </summary>
	void UseTwentyPercent();

	/// <summary>
	/// �ő�l�́���%�̃|�C���g�������Ă��邩�m�F
	/// </summary>
	/// <returns>�ő�l�́���%�̃|�C���g�������Ă��邩</returns>
	bool CheckPercent(int percent);

	//getter
	const int GetEnergyPoint() {}

private:
	/// <summary>
	/// �o�[�̒�����ύX
	/// </summary>
	void ChangeLengthBar();

	/// <summary>
	/// �o�[�̒����ύX���Z�b�g
	/// </summary>
	void SetChangeLength();

private:
	//�|�C���g�\��(�g)�X�v���C�g
	Sprite* frameSprite = nullptr;
	//�|�C���g�\��(�o�[)�X�v���C�g
	Sprite* barSprite = nullptr;
	//�G�l���M�[�|�C���g����l
	const int energyPointMax = 100;
	//�G�l���M�[�|�C���g
	int energyPoint = 0;
	//�o�[�̒�����ύX���邩
	bool isChangeLengthBar = false;
	//�o�[�̒����ő�l
	const float lengthMax = 556;
	//�o�[�̒����ύX�^�C�}�[
	int changeLengthTimer = 0;
	//�o�[�̒����ύX�O�̒���
	float changeLengthBefore = 0;
	//�o�[�̒����ύX��̒���
	float changeLengthAftar = 0;
};