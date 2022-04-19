#pragma once
#include "Object3d.h"
#include "Pin.h"
#include "FixedEnemy.h"

class ConnectCircle
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
	/// �f�X�g���N�^
	/// </summary>
	~ConnectCircle();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ێ����Ă���s���������̃s�����g�p���Ă��邩�m�F
	/// </summary>
	/// <param name="pin">�s��</param>
	/// <returns>�ێ����Ă���s���������̃s�����g�p���Ă��邩</returns>
	bool CheckUsePin(Pin* pin);

	/// <summary>
	/// �폜
	/// </summary>
	void SetDelete();

	//getter
	XMFLOAT3 GetPosition() { return circleObject->GetPosition(); }
	float GetRadius() { return radius; }
	bool GetIsChangeRadius() { return isChangeRadius; }
	bool GetIsDelete() { return isDelete; }

protected:
	/// <summary>
	/// ���a�̑傫���ύX���Z�b�g
	/// </summary>
	void SetChangeRadius();

	/// <summary>
	/// ���a�̑傫����ύX
	/// </summary>
	void ChangeRadius();

protected:
	//�~�I�u�W�F�N�g
	Object3d* circleObject = nullptr;
	//�~�̒��S�ƂȂ�s��
	Pin* pin = nullptr;
	//�~�̒��S�ƂȂ�Œ�G
	FixedEnemy* fixedEnemy = nullptr;
	//��̔��a
	float baseRadius;
	//�~�̔��a
	float radius = 0;
	//�ύX�O�̉~�̃T�C�Y
	float changeRadiusStart = 0;
	//�ύX��̉~�̃T�C�Y
	float changeRadiusEnd = 0;
	//�~�̃T�C�Y�ύX�p�̃^�C�}�[
	int changeRadiusTimer = 0;
	//��������������{�o�Ă��邩�J�E���g
	int countLine = 0;
	//�T�C�Y��ύX����
	bool isChangeRadius = false;
	//�폜���邩
	bool isDelete = false;
};