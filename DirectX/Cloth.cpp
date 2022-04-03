#include "Cloth.h"
#include "SafeDelete.h"

Cloth* Cloth::Create(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width)
{
	//インスタンスを生成
	Cloth* instance = new Cloth();
	if (instance == nullptr)
	{
		return nullptr;
	}

	//初期化
	if (!instance->Init(startPoint, endPoint, color, width))
	{
		delete instance;
		assert(0);
	}

	return instance;
}

Cloth::Cloth() : counter(0)
{
	gravity = { 4.9f ,4.9f ,0.0f };// 重力
	windforce = { 3.0f ,3.0f ,0.0f };// 風の強さ
	damping = { 0.05f ,0.05f ,0.0f };// 弾性力
	k = { 5.0f ,5.0f ,0.0f };// よくわからん
	kd = { 2.0f ,2.0f ,0.0f };// よくわからん
	spring_length = { 2.0f ,2.0f ,0.0f };// よくわからん
	d_spring_length = { spring_length.x * sqrtf(2.0f),spring_length.y * sqrtf(2.0f),0.0f };// よくわからん
	dt = { 0.01f ,0.01f ,0.0f };// よくわからん

	for (int j = 0; j < NumGrid; j++)
	{
		// 横
		for (int i = 0; i < NumGrid; i++)
		{
			line[j][i] = nullptr;
		}
	}

	//線の太さ
	weight = 0.5f;
	//線の色
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
	for (int j = 0; j < NumGrid; j++)
	{
		// 横
		for (int i = 0; i < NumGrid; i++)
		{
			safe_delete(line[j][i]);
		}
	}
}

bool Cloth::Init(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width)
{
	//線生成
	for (int j = 0; j < NumGrid; j++)
	{
		// 横
		for (int i = 0; i < NumGrid; i++)
		{
			line[j][i] = DrawLine3D::Create();
			if (line[j][i] == nullptr)
			{
				return false;
			}
		}
	}

	for (int j = 0; j < NumGrid; j++)
	{
		for (int i = 0; i < NumGrid; i++)
		{
			// 線の始点
			if (i == 0 && j == 0)
			{
				// ここに敵の座標を代入1
				grids[j][i].pos.x = startPoint.x;
				grids[j][i].pos.y = -startPoint.y;
				grids[j][i].mass = gridmass;
				continue;
			}

			// 線の終点
			if (i == 0 && j == (NumGrid - 1))
			{
				// ここに敵の座標を代入2
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

	// 縦
	for (int j = 0; j < NumGrid; j++)
	{
		// 横
		for (int i = 0; i < NumGrid - sub; i++)
		{
			XMFLOAT3 vec1 = {
				grids[j][i].pos.x,
				-grids[j][i].pos.y,
				0.0f
			};

			// バネ
			if (j >= 1)
			{
				XMFLOAT3 vec2 = {
					grids[j - 1][i].pos.x,
					-grids[j - 1][i].pos.y,
					0.0f
				};

				line[j][i]->SetLine(vec1, vec2, color, weight);
			}

			line[j][i]->Update(camera);
		}
	}
}

void Cloth::Draw()
{
	for (int j = 0; j < NumGrid; j++)
	{
		// 横
		for (int i = 0; i < NumGrid; i++)
		{
			line[j][i]->Draw();
		}
	}
}

void Cloth::SetColor(XMFLOAT3 startPoint, XMFLOAT3 endPoint, XMFLOAT4 color, float width)
{
	for (int j = 0; j < NumGrid; j++)
	{
		// 横
		for (int i = 0; i < NumGrid; i++)
		{
			//line[j][i]->SetLine(startPoint, endPoint, color, width);
		}
	}
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

			// 1つのグリッドに働く合力を求める

			// 力をゼロにする
			g.force = { 0.0f,0.0f,0.0f };

			// 重力
			g.force.y += g.mass * gravity.y;

			// 風
			float r1 = counter / 0.70f;
			float r2 = counter / 0.25f;
			g.force.x += ((cosf(r1) * cosf(r2) * 0.5f + 0.5f + 0.5f) * windforce.x);
			g.force.y += ((sinf(r1) * sinf(r2) * 0.5f + 0.5f + 0.5f) * windforce.y);

			// ダンピング
			g.force.x -= g.vel.x * damping.x;
			g.force.y -= g.vel.y * damping.y;

			// 上のバネ
			if (j <= NumGrid - 2)
			{
				XMFLOAT3 d =
				{
					g.pos.x - grids[j + 1][i].pos.x,
					g.pos.y - grids[j + 1][i].pos.y,
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

			// 下のバネ
			if (j >= 1)
			{
				XMFLOAT3 d =
				{
					g.pos.x - grids[j - 1][i].pos.x,
					g.pos.y - grids[j - 1][i].pos.y,
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

			/*// 左のバネ
			if (i >= 1)
			{
				XMFLOAT3 d =
				{
					g.pos.x - grids[j][i - 1].pos.x,
					g.pos.y - grids[j][i - 1].pos.y,
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

			// 右のバネ
			if (i <= NumGrid - 2)
			{
				XMFLOAT3 d =
				{
					g.pos.x - grids[j][i + 1].pos.x,
					g.pos.y - grids[j][i + 1].pos.y,
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

			// 左上のバネ
			if (i >= 1 && j <= NumGrid - 2)
			{
				XMFLOAT3 d =
				{
					g.pos.x - grids[j + 1][i - 1].pos.x,
					g.pos.y - grids[j + 1][i - 1].pos.y,
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

			// 右上のバネ
			if (i <= NumGrid - 2 && j <= NumGrid - 2)
			{
				XMFLOAT3 d =
				{
					g.pos.x - grids[j + 1][i + 1].pos.x,
					g.pos.y - grids[j + 1][i + 1].pos.y,
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

			// 左下のバネ
			if (i >= 1 && j >= 1)
			{
				XMFLOAT3 d =
				{
					g.pos.x - grids[j - 1][i - 1].pos.x,
					g.pos.y - grids[j - 1][i - 1].pos.y,
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

			// 左下のバネ
			if (i <= NumGrid - 2 && j >= 1)
			{
				XMFLOAT3 d =
				{
					g.pos.x - grids[j - 1][i + 1].pos.x,
					g.pos.y - grids[j - 1][i + 1].pos.y,
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
			}*/
		}
	}
}

void Cloth::EularMethod()
{
	//　力の更新
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