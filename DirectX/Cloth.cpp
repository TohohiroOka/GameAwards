#include "Cloth.h"
#include "SafeDelete.h"
#include "Camera.h"

using namespace std;

Cloth* Cloth::Create(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width)
{
	//�C���X�^���X�𐶐�
	Cloth* instance = new Cloth();
	if (instance == nullptr)
	{
		return nullptr;
	}

	//������
	if (!instance->Init(startPoint, endPoint, color, width))
	{
		delete instance;
		assert(0);
	}

	return instance;
}

Cloth::Cloth() : counter(0)
{
	gravity = { 4.9f ,4.9f ,0.0f };// �d��
	windforce = { 3.0f ,3.0f ,0.0f };// ���̋���
	damping = { 0.05f ,0.05f ,0.0f };// �e����
	k = { 5.0f ,5.0f ,0.0f };// �悭�킩���
	kd = { 2.0f ,2.0f ,0.0f };// �悭�킩���
	spring_length = { 2.0f ,2.0f ,0.0f };// �悭�킩���
	d_spring_length = { spring_length.x * sqrtf(2.0f),spring_length.y * sqrtf(2.0f),0.0f };// �悭�킩���
	dt = { 0.01f ,0.01f ,0.0f };// �悭�킩���

	//��
	line = nullptr;

	//���̑���
	weight = 0.5f;
	//���̐F
	color = { 0.4f, 1, 0.2f, 1 };

	counter = 0;

	grids.resize(NumGrid);
	for (int i = 0; i < NumGrid; i++)
	{
		grids[i].resize(NumGrid);
	}
}

Cloth::~Cloth()
{
	safe_delete(line);
}

bool Cloth::Init(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width)
{
	//������
	line = DrawLine3D::Create(LineNum);
	if (line == nullptr)
	{
		return false;
	}

	for (int j = 0; j < NumGrid; j++)
	{
		for (int i = 0; i < NumGrid; i++)
		{
			// ���̎n�_
			if (i == 0 && j == 0)
			{
				// �����ɓG�̍��W����1
				grids[j][i].pos.x = startPoint.x;
				grids[j][i].pos.y = -startPoint.y;
				grids[j][i].mass = gridmass;
				continue;
			}

			// ���̏I�_
			if (i == 0 && j == (NumGrid - 1))
			{
				// �����ɓG�̍��W����2
				grids[j][i].pos.x = endPoint.x;
				grids[j][i].pos.y = -endPoint.y;
				grids[j][i].mass = gridmass;
				continue;
			}

			grids[j][i].mass = gridmass;
			grids[j][i].pos.x = ((float)i - (float)(NumGrid - 1.0f) / 2.0f) * spring_length.x;
			grids[j][i].pos.y = ((float)j * spring_length.y);
			grids[j][i].pos.z = 0.0f;
		}
	}

	return true;
}

void Cloth::Update(Camera* camera)
{
	EularMethod();
	counter++;

	//���̍ő�l
	XMFLOAT3 vec1[LineNum];
	XMFLOAT3 vec2[LineNum];

	//���݂̐�
	int nowLine = 0;

	// �c
	for (int j = 0; j < NumGrid; j++)
	{
		// ��
		for (int i = 0; i < NumGrid - sub; i++)
		{
			// �o�l
			if (j >= 1)
			{
				vec1[nowLine] = {
					grids[j][i].pos.x,
					-grids[j][i].pos.y,
					0.0f
				};

				//1�O�̍��W�̔z��ԍ�
				int beforePosArray = j - 1;
				vec2[nowLine] = {
					grids[beforePosArray][i].pos.x,
					-grids[beforePosArray][i].pos.y,
					0.0f
				};

				nowLine++;
			}
		}
	}

	line->SetLine(vec1, vec2, weight);
	line->SetColor(color);
	line->Update(camera);
}

void Cloth::Draw()
{
	line->Draw();
}

void Cloth::SetColor(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width)
{
	//line->SetColor(color);
}

void Cloth::UpdateForce()
{
	for (int j = 0; j < NumGrid; j++)
	{
		for (int i = 0; i < NumGrid; i++)
		{
			if ((i == 0 && j == (NumGrid - 1)) || (i == 0 && j == 0))
			{
				continue;
			}

			Grid& g = grids[j][i];

			// 1�̃O���b�h�ɓ������͂����߂�

			// �͂��[���ɂ���
			g.force = { 0.0f,0.0f,0.0f };

			// �d��
			g.force.y += g.mass * gravity.y;

			// ��
			float r1 = counter / 0.70f;
			float r2 = counter / 0.25f;
			g.force.x += ((cosf(r1) * cosf(r2) * 0.5f + 0.5f + 0.5f) * windforce.x);
			g.force.y += ((sinf(r1) * sinf(r2) * 0.5f + 0.5f + 0.5f) * windforce.y);

			// �_���s���O
			g.force.x -= g.vel.x * damping.x;
			g.force.y -= g.vel.y * damping.y;

			// ��̃o�l
			if (j <= NumGrid - 2)
			{
				//1��̍��W�̔z��ԍ�
				int afterPosArray_J = j + 1;

				XMFLOAT3 d =
				{
					g.pos.x - grids[afterPosArray_J][i].pos.x,
					g.pos.y - grids[afterPosArray_J][i].pos.y,
					0.0f
				};
				XMFLOAT3 n =
				{
					d.x / sqrtf(d.x * d.x + d.y * d.y),
					d.y / sqrtf(d.x * d.x + d.y * d.y),
					0.0f
				};

				g.force.x += n.x * ((spring_length.x - sqrtf(d.x * d.x + d.y * d.y)) * k.x);
				g.force.y += n.y * ((spring_length.y - sqrtf(d.x * d.x + d.y * d.y)) * k.y);
			}

			// ���̃o�l
			if (j >= 1)
			{
				//1�O�̍��W�̔z��ԍ�
				int beforePosArray_J = j - 1;

				XMFLOAT3 d =
				{
					g.pos.x - grids[beforePosArray_J][i].pos.x,
					g.pos.y - grids[beforePosArray_J][i].pos.y,
					0.0f
				};
				XMFLOAT3 n =
				{
					d.x / sqrtf(d.x * d.x + d.y * d.y),
					d.y / sqrtf(d.x * d.x + d.y * d.y),
					0.0f
				};

				g.force.x += n.x * ((spring_length.x - sqrtf(d.x * d.x + d.y * d.y)) * k.x);
				g.force.y += n.y * ((spring_length.y - sqrtf(d.x * d.x + d.y * d.y)) * k.y);
			}

			// ���̃o�l
			if (i >= 1)
			{
				//1�O�̍��W�̔z��ԍ�
				int beforePosArray_I = i - 1;

				XMFLOAT3 d =
				{
					g.pos.x - grids[j][beforePosArray_I].pos.x,
					g.pos.y - grids[j][beforePosArray_I].pos.y,
					0.0f
				};
				XMFLOAT3 n =
				{
					d.x / sqrtf(d.x * d.x + d.y * d.y),
					d.y / sqrtf(d.x * d.x + d.y * d.y),
					0.0f
				};

				g.force.x += n.x * ((spring_length.x - sqrtf(d.x * d.x + d.y * d.y)) * k.x);
				g.force.y += n.y * ((spring_length.y - sqrtf(d.x * d.x + d.y * d.y)) * k.y);
			}

			// �E�̃o�l
			if (i <= NumGrid - 2)
			{
				//1��̍��W�̔z��ԍ�
				int afterPosArray_I = i + 1;

				XMFLOAT3 d =
				{
					g.pos.x - grids[j][afterPosArray_I].pos.x,
					g.pos.y - grids[j][afterPosArray_I].pos.y,
					0.0f
				};
				XMFLOAT3 n =
				{
					d.x / sqrtf(d.x * d.x + d.y * d.y),
					d.y / sqrtf(d.x * d.x + d.y * d.y),
					0.0f
				};

				g.force.x += n.x * ((spring_length.x - sqrtf(d.x * d.x + d.y * d.y)) * k.x);
				g.force.y += n.y * ((spring_length.y - sqrtf(d.x * d.x + d.y * d.y)) * k.y);
			}

			// ����̃o�l
			if (i >= 1 && j <= NumGrid - 2)
			{
				//1��̍��W�̔z��ԍ�
				int afterPosArray_J = j + 1;

				//1�O�̍��W�̔z��ԍ�
				int beforePosArray_I = i - 1;

				XMFLOAT3 d =
				{
					g.pos.x - grids[afterPosArray_J][beforePosArray_I].pos.x,
					g.pos.y - grids[afterPosArray_J][beforePosArray_I].pos.y,
					0.0f
				};
				XMFLOAT3 n =
				{
					d.x / sqrtf(d.x * d.x + d.y * d.y),
					d.y / sqrtf(d.x * d.x + d.y * d.y),
					0.0f
				};

				g.force.x += n.x * ((spring_length.x - sqrtf(d.x * d.x + d.y * d.y)) * kd.x);
				g.force.y += n.y * ((spring_length.y - sqrtf(d.x * d.x + d.y * d.y)) * kd.y);
			}

			// �E��̃o�l
			if (i <= NumGrid - 2 && j <= NumGrid - 2)
			{
				//1��̍��W�̔z��ԍ�
				int afterPosArray_J = j + 1;

				//1��̍��W�̔z��ԍ�
				int afterPosArray_I = i + 1;

				XMFLOAT3 d =
				{
					g.pos.x - grids[afterPosArray_J][afterPosArray_I].pos.x,
					g.pos.y - grids[afterPosArray_J][afterPosArray_I].pos.y,
					0.0f
				};
				XMFLOAT3 n =
				{
					d.x / sqrtf(d.x * d.x + d.y * d.y),
					d.y / sqrtf(d.x * d.x + d.y * d.y),
					0.0f
				};

				g.force.x += n.x * ((spring_length.x - sqrtf(d.x * d.x + d.y * d.y)) * kd.x);
				g.force.y += n.y * ((spring_length.y - sqrtf(d.x * d.x + d.y * d.y)) * kd.y);
			}

			// �����̃o�l
			if (i >= 1 && j >= 1)
			{
				//1�O�̍��W�̔z��ԍ�
				int beforePosArray_J = j - 1;

				//1�O�̍��W�̔z��ԍ�
				int beforePosArray_I = j - 1;

				XMFLOAT3 d =
				{
					g.pos.x - grids[beforePosArray_J][beforePosArray_I].pos.x,
					g.pos.y - grids[beforePosArray_J][beforePosArray_I].pos.y,
					0.0f
				};
				XMFLOAT3 n =
				{
					d.x / sqrtf(d.x * d.x + d.y * d.y),
					d.y / sqrtf(d.x * d.x + d.y * d.y),
					0.0f
				};

				g.force.x += n.x * ((spring_length.x - sqrtf(d.x * d.x + d.y * d.y)) * kd.x);
				g.force.y += n.y * ((spring_length.y - sqrtf(d.x * d.x + d.y * d.y)) * kd.y);
			}

			// �����̃o�l
			if (i <= NumGrid - 2 && j >= 1)
			{
				//1�O�̍��W�̔z��ԍ�
				int beforePosArray_J = j - 1;

				//1��̍��W�̔z��ԍ�
				int afterPosArray_I = i + 1;

				XMFLOAT3 d =
				{
					g.pos.x - grids[beforePosArray_J][afterPosArray_I].pos.x,
					g.pos.y - grids[beforePosArray_J][afterPosArray_I].pos.y,
					0.0f
				};
				XMFLOAT3 n =
				{
					d.x / sqrtf(d.x * d.x + d.y * d.y),
					d.y / sqrtf(d.x * d.x + d.y * d.y),
					0.0f
				};

				g.force.x += n.x * ((spring_length.x - sqrtf(d.x * d.x + d.y * d.y)) * kd.x);
				g.force.y += n.y * ((spring_length.y - sqrtf(d.x * d.x + d.y * d.y)) * kd.y);
			}
		}
	}
}

void Cloth::EularMethod()
{
	//�@�͂̍X�V
	UpdateForce();

	for (int j = 0; j < NumGrid; j++)
	{
		for (int i = 0; i < NumGrid; i++)
		{
			grids[j][i].vel.x += (grids[j][i].force.x / grids[j][i].mass) * dt.x;
			grids[j][i].vel.y += (grids[j][i].force.y / grids[j][i].mass) * dt.y;
			grids[j][i].pos.x += grids[j][i].vel.x * dt.x;
			grids[j][i].pos.y += grids[j][i].vel.y * dt.y;
		}
	}
}