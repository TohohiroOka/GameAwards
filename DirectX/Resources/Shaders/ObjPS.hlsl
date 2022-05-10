#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

/// <summary>
/// �u���[���̃Z�b�g
/// </summary>
float4 SetBloom(float4 shadecolor, float4 texcolor, float4 color);

/// <summary>
/// �g�D�[���̃Z�b�g
/// </summary>
float4 SetToon(float4 shadecolor);

PSOutput main(VSOutput input)
{
	PSOutput output;

	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);

	// ����x
	const float shininess = 4.0f;

	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
	float3 ambient = m_ambient;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	//���s����
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (dirLights[i].active)
		{
			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);

			// ���ˌ��x�N�g��
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			// ���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// �S�ĉ��Z����
			shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
		}
	}

	//�u���[������
	float4 bloom = float4(0, 0, 0, 0);
	if (isBloom)
	{
		bloom = SetBloom(shadecolor, texcolor, color);
	}

	//�g�D�[���V�F�[�h
	if (isToon)
	{
		shadecolor = SetToon(shadecolor);
	}

	// �V�F�[�f�B���O�ɂ��F�ŕ`��
	float4 mainColor = shadecolor * texcolor * color;
	output.target0 = float4(mainColor.rgb, color.w);
	output.target1 = bloom;

	return output;
}

float4 SetBloom(float4 shadecolor, float4 texcolor, float4 color)
{
	//���x�l�̒��o
	float LuminousIntensity = dot(shadecolor.rgb * texcolor.rgb, float3(0.2125, 0.7154, 0.0712));

	//�u���[����������ꏊ
	float4 bloomColor = step(1.0, LuminousIntensity) * texcolor * color;

	return bloomColor;
}

float4 SetToon(float4 shadecolor)
{
	//�g�D�[���̐F�͈�
	float toonLighrRange = 0.525;
	//���Â̒��ԕ�
	float contourWidth = 0.1;

	//���邢
	float3 bright;
	bright.r = step(toonLighrRange, shadecolor.r);
	bright.g = step(toonLighrRange, shadecolor.g);
	bright.b = step(toonLighrRange, shadecolor.b);
	//�Â�
	float3 dark;
	dark.r = (1 - step(toonLighrRange, shadecolor.r));
	dark.g = (1 - step(toonLighrRange, shadecolor.g));
	dark.b = (1 - step(toonLighrRange, shadecolor.b));

	//����
	float3 intermediate;
	intermediate.r = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.r);
	intermediate.g = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.g);
	intermediate.b = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.b);

	//���݂̐F
	return float4(bright + dark + intermediate, 1);
}