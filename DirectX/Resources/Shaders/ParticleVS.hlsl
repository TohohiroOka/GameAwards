#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : SCALE, float4 color : COLOR, float3 rota : ROTATE)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.pos = pos;
	output.scale = scale;
	output.color = color;
	output.rota = rota;
	return output;
}