#pragma once
#include "Object3d.h"
#include "GaruEnemy.h"

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
	/// �T�C�Y��傫������
	/// </summary>
	void BigRadius();

	/// <summary>
	/// �T�C�Y������������
	/// </summary>
	void SmallRadius();

	/// <summary>
	/// �ێ����Ă���K�����̓G�ƈ����̃K�����̓G���g�p���Ă��邩�m�F
	/// </summary>
	/// <param name="enemy">�G</param>
	/// <returns>�ێ����Ă���G�ƈ����̓G���g�p���Ă��邩</returns>
	bool CheckUseEnemy(GaruEnemy *enemy);

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
	/// ���a�̑傫���ύX
	/// </summary>
	void ChangeRadius();

protected:
	//�~�I�u�W�F�N�g
	Object3d *circleObject = nullptr;
	//�~�̒��S�ƂȂ�G(���S)
	GaruEnemy *enemy = nullptr;
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