#include "Emitter.h"
#include "Camera.h"

Emitter::~Emitter()
{
	delete particleManager;
}

void Emitter::Create(int texNumber)
{
	particleManager = new ParticleManager();
	particleManager->Create(texNumber);
}

void Emitter::InEmitter(int maxFrame, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel,
	XMFLOAT2 startScale, XMFLOAT2 endScale, XMFLOAT4 startColor, XMFLOAT4 endColor)
{
	particleManager->Add(maxFrame, position, velocity,
		accel, startScale, endScale, startColor, endColor);
}

void Emitter::Update()
{
	count = particleManager->Update();
}

void Emitter::Draw()
{
	particleManager->Draw();
}

void Emitter::AllDelete()
{
	particleManager->AllDelete();
}