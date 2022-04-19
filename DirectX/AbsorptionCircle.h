#pragma once
#include "Object3d.h"

class AbsorptionCircle 
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	enum ActionNumber 
	{
		Spread,	//�~���L����
		Stay,	//�ꎞ��~
		Shrink,	//�~���k�߂�
	};

public:
	/// <summary>
	/// �z���~����
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�z���~</returns>
	static AbsorptionCircle* Create(Model* model, XMFLOAT3 position);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~AbsorptionCircle();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="waveModel">���f��</param>
	/// <returns>����</returns>
	bool Initialize(Model* model, XMFLOAT3 position);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �폜
	/// </summary>
	void SetDelete();

	//getter
	XMFLOAT3 GetPosition() { return circleObject->GetPosition(); }
	XMFLOAT3 GetScale() { return circleObject->GetScale(); }
	bool GetIsDelete() { return isDelete; }

private:
	/// <summary>
	/// �z���~���L����
	/// </summary>
	void SpreadCircle();

	/// <summary>
	/// �ꎞ��~
	/// </summary>
	void StayCircle();

	/// <summary>
	/// �z���~���k��������
	/// </summary>
	void ShrinkCircle();

private:
	//�z���~�I�u�W�F�N�g
	Object3d* circleObject = nullptr;
	//�~�̃T�C�Y���L����^�C�}�[
	int spreadTimer = 0;
	//�ꎞ��~�^�C�}�[
	int stayTimer = 0;
	//�~�̃T�C�Y���k�߂�^�C�}�[
	int shrinkTimer = 0;
	//�~�̍s��
	int action = ActionNumber::Spread;
	//�폜���邩
	bool isDelete = false;
};