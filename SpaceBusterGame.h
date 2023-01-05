#pragma once
#ifndef SPACEBUSTERGAME_H
#define SPACEBUSTERGAME_H

#include "Game.h"
#include "AstroidManager.h"
#include "ShipController.h"
#include "CameraController.h"
#include "imgui.h"

class SpaceBusterGame :  public Game
{
public:
	SpaceBusterGame();
	virtual ~SpaceBusterGame();


	virtual void FixedUpdate(float deltaTime) override;
	virtual void Update(float deltaTime) override;

	virtual void DrawScene() override;
	virtual void DrawForeGround() override;
	virtual void DrawBackGround() override;

	virtual void onBeginContact(b2Contact* contact) override;
	virtual void onEndContact(b2Contact* contact) override;
	virtual void onPreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
	virtual void onPostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;

	virtual void KeyboardUp(int key) override;
	virtual void KeyboardDown(int key) override;

	virtual void DrawDebug() override;
protected:

	ShipController* m_player;
	SpaceBuster::AstroidFactory* m_astroidFactory;
	CameraController* m_cameraController;
};
#endif // !SPACEBUSTERGAME_H

