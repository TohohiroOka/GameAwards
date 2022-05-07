#pragma once
#include "Sprite.h"

class Combo
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
	/// �R���{����
	/// </summary>
	/// <param name="numberTexNum">�����e�N�X�`���ԍ�</param>
	/// <param name="comboTexNum">Combo�e�N�X�`���ԍ�</param>
	/// <returns>�R���{</returns>
	static Combo* Create(int numberTexNum, int comboTexNum);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Combo();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="numberTexNum">�����e�N�X�`���ԍ�</param>
	/// <param name="comboTexNum">Combo�e�N�X�`���ԍ�</param>
	/// <returns>����</returns>
	bool Initialize(int numberTexNum, int comboTexNum);

	/// <summary>
	/// �X�V
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
	/// �R���{���𑝂₷
	/// </summary>
	void AddCombo();

	/// <summary>
	/// �R���{�I��
	/// </summary>
	void LostCombo();

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ���ԂɃZ�b�g
	/// </summary>
	void SetMoveResultPos();

	//getter
	int GetCombo() { return combo; }
	int GetMaxCombo() { return maxCombo; }
	bool GetIsMoveGamePosEnd() { return isMoveGamePosEnd; }

protected:
	/// <summary>
	/// �R���{�I���^�C�}�[�X�V
	/// </summary>
	void LostTimerUpdate();

	/// <summary>
	/// �\���p�R���{�X�v���C�g�ύX
	/// </summary>
	void ChangeComboSprite();

	/// <summary>
	/// �Q�[���V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveGamePos();

	/// <summary>
	/// ���U���g�V�[���̍��W�Ɉړ�
	/// </summary>
	void MoveResultPos();

private:
	//�R���{���X�v���C�g
	static const int comboDigits = 3;
	Sprite* numberSprite[comboDigits] = { nullptr };
	//COMBO�X�v���C�g
	Sprite* comboSprite = nullptr;
	//�R���{��
	int combo = 0;
	//�ő�R���{��
	int maxCombo = 0;
	//�R���{�I���^�C�}�[
	int lostComboTimer = 0;
	//�Q�[���V�[���̍��W�Ɉړ�����
	bool isMoveGamePos = false;
	//�Q�[���V�[���̍��W�Ɉړ��I��������
	bool isMoveGamePosEnd = false;
	//�Q�[���V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[
	int moveGamePosTimer = 0;
	//���U���g�V�[���̍��W�Ɉړ�����
	bool isMoveResultPos = false;
	//���U���g�V�[���̍��W�Ɉړ��I��������
	bool isMoveResultPosEnd = false;
	//���U���g�V�[���̍��W�Ɉړ����鎞�ԃ^�C�}�[
	int moveResultPosTimer = 0;
};