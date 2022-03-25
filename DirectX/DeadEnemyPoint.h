#pragma once
#include "Object3d.h"
#include "BaseEnemy.h"

class DeadEnemyPoint
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
	/// �G�̎��S�ʒu����
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>�G�̎��S�ʒu</returns>
	static DeadEnemyPoint *Create(Model *model, BaseEnemy *enemy, float radius);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DeadEnemyPoint();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <returns>����</returns>
	bool Initialize(Model *model, BaseEnemy *enemy, float radius);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���a�̑傫���ύX
	/// </summary>
	void ChangeRadius();

	/// <summary>
	/// �ێ����Ă���G�ƈ����̓G���g�p���Ă��邩�m�F
	/// </summary>
	/// <param name="enemy">�G</param>
	void CheckUseEnemy(BaseEnemy *enemy);

	//getter
	XMFLOAT3 GetPosition() { return deadPointObject->GetPosition(); }
	float GetRadius() { return radius; }
	bool GetIsChangeRadius() { return isChangeRadius; }
	bool GetIsDelete() { return isDelete; }

private:
	//���񂾈ʒu�I�u�W�F�N�g
	Object3d *deadPointObject = nullptr;
	//�~�̒��S�ƂȂ�G(���S)
	BaseEnemy *enemy = nullptr;
	//��̔��a
	float BaseRadius;
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