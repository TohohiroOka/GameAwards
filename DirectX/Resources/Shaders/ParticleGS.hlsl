#include "Particle.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] = {
	float4(-0.5f,-0.5f,0,0),//左下
	float4(-0.5f,+0.5f,0,0),//左上
	float4(+0.5f,-0.5f,0,0),//右下
	float4(+0.5f,+0.5f,0,0)//右上
};

//1左が0,0　右が1,1
static const float2 uv_array[vnum] = {
	float2(0,1),//左下
	float2(0,0),//左上
	float2(1,1),//右下
	float2(1,0)//右上

};

//点の入力から、四角形を出力
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;

	matrix matrota = {
		{cos(input[0].rota.z),-sin(input[0].rota.z),0,0},
		{sin(input[0].rota.z),cos(input[0].rota.z),0,0 },
		{0,0,1,0},
		{0,0,0,0},
	};

	for (uint i = 0; i < vnum; i++)
	{
		//中心からのオフセットをスケーリング
		float4 offset = float4(offset_array[i].x * input[0].scale.x, offset_array[i].y * input[0].scale.y, 0, 0);
		//中心からオフセット
		offset = mul(mul(matrota, matBillboard), offset);
		//オフセット分ずらす
		element.svpos = input[0].pos + offset;
		//ビュープロジェクション
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		element.color = input[0].color;
		output.Append(element);
	}
}