#include "PostEffect.hlsli"

Texture2D<float4> tex0:register(t0);//0番スロットに設定されたテクスチャ
Texture2D<float4> tex1:register(t1);//1番スロットに設定されたテクスチャ
SamplerState smp:register(s0);//0番スロットに設定されたサンプラー

/// <summary>
/// ガウス処理
/// </summary>
float Gaussian(float2 drawUV, float2 pickUV, float sigma);

/// <summary>
/// ブルーム処理
/// </summary>
float4 SetBloom(float2 uv);

float4 main(VSOutput input) : SV_TARGET
{
	//メインカラー
	float4 mainColor = tex0.Sample(smp, input.uv);

	//bloom処理
	float4 bloom = SetBloom(input.uv);

	//ポストエフェクトの合成
	mainColor = float4(mainColor.rgb + bloom.rgb, 1.0);

	return mainColor;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-d * d) / (2 * sigma * sigma);
}

float4 SetBloom(float2 uv)
{
	float totalWeight = 0;
	float sigma = 0.002;
	float stepWidth = 0.0005;
	float4 color = { 0, 0, 0, 0 };

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
	{
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
		{
			float2 pickUV = uv + float2(px, py);
			float weight = Gaussian(uv, pickUV, sigma);
			color += tex1.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}

	color.rgb = color.rgb / totalWeight;

	return color;
}