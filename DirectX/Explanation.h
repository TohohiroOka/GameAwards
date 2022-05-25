#pragma once
#include"Sprite.h"

class Explanation
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
	/// �^�C�g����ʗpUI����
	/// </summary>
	/// <returns>�^�C�g����ʗpUI</returns>
	static Explanation* Create(int explanationTexNum, int XBunttonTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Explanation();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(int explanationTexNum, int XBunttonTexNum);

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
	/// ��ʓ��Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveInScreen();

	/// <summary>
	/// ��ʊO�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveOutScreen();


private:
	/// <summary>
	/// ��ʓ��Ɉړ�
	/// </summary>
	void MoveInScreen();

	/// <summary>
	/// ��ʊO�Ɉړ�
	/// </summary>
	void MoveOutScreen();

private:
	//�Q�[�������X�v���C�g
	Sprite* explanationSprite = nullptr;
	//X�{�^���X�v���C�g
	Sprite* XButtonSprite = nullptr;
	//��ʓ��Ɉړ�����
	bool isMoveInScreen = false;
	//��ʓ��Ɉړ����鎞�ԃ^�C�}�[
	int moveInScreenTimer = 0;
	//��ʊO�Ɉړ�����
	bool isMoveOutScreen = false;
	//��ʊO�Ɉړ����鎞�ԃ^�C�}�[
	int moveOutScreenTimer = 0;
};