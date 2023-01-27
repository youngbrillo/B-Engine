#include "NeoShipController.h"
#include "Game.h"
#include "App.h"


NeoShipController::NeoShipController(NeoShipControllerDefinition* controllerDefinition, b2Body* groundref)
	: mouseAiming(true)
	, projectileLocalLaunchPos(controllerDefinition->projectileLocalLaunchPos)
	, projectileLaunchVel(controllerDefinition->projectileLaunchVel)
	, target(0.0f, 0.f)
	, launchVelocity(50.0f)
{
	m_ship = new Ship(&controllerDefinition->shipDef, Game::m_World, b2Vec2_zero, groundref);
	m_launcher = new OmniLauncher(&controllerDefinition->launcherDef);
	m_camera_controller = new CameraController(Game::AppCam, m_ship->mBody);
	m_camera_controller->mode = CameraController::FollowMode::Bound;
}

NeoShipController::~NeoShipController()
{
	delete m_launcher;
	delete m_ship;
	delete m_camera_controller;

}

void NeoShipController::FixedUpdate(float deltaTime)
{
	m_ship->FixedUpdate(deltaTime);

	if (shooting) this->ShootProjectile();

}

void NeoShipController::Update(float dt)
{
	m_ship->Update(dt);
	m_launcher->Update(dt);
	m_camera_controller->Update(dt);
}

void NeoShipController::handleBeginContact(b2Contact* contact)
{
	m_ship->handleBeginContact(contact);
}

void NeoShipController::handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	m_ship->handlePreSolve(contact, oldManifold);
	m_launcher->handleCollision(contact);
}

void NeoShipController::handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	m_ship->handlePostSolve(contact, impulse);
}

void NeoShipController::handleEndContact(b2Contact* contact)
{
	m_ship->handleEndContact(contact);
}

void NeoShipController::mouseCallback(int button, int action, int mode)
{
	if (ImGui::GetIO().WantCaptureMouse) {
		shooting = false;
		return;
	}
	m_ship->mouseCallback(button, action, mode);

	if (!this->mouseAiming) return;

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		shooting = true;
	}

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
	{
		shooting = false;
	}
}

void NeoShipController::mouseCallback_Cursor(double x, double y)
{
	m_ship->mouseCallback_Cursor(x, y);

}
void NeoShipController::onKeyPress(int key)
{
//	if (ImGui::GetIO().WantCaptureMouse) return;
	//if (ImGui::GetIO().WantCaptureKeyboard) return;

	m_ship->onKeyPress(key);

	if (key == GLFW_KEY_TAB)
	{
		m_launcher->ToggleFireMode();
	}
	if (key == GLFW_KEY_Q)
	{
		m_launcher->ToggleAmmoType();
	}

	if (key == GLFW_KEY_SPACE && !mouseAiming)
	{
		shooting = true;
	}
}

void NeoShipController::onKeyRelease(int key)
{
	//if (ImGui::GetIO().WantCaptureMouse) return;
	//if (ImGui::GetIO().WantCaptureKeyboard) return;

	if (key == GLFW_KEY_SPACE && !mouseAiming)
	{
		shooting = false;
	}
	m_ship->onKeyRelease(key);
}

void NeoShipController::onKeyRepeat(int key)
{
	if (ImGui::GetIO().WantCaptureKeyboard) return;

}

void NeoShipController::ShootProjectile()
{

	b2Vec2 p = m_ship->getBody()->GetWorldPoint(projectileLocalLaunchPos); //velocity should be in the direction the player is facing
	b2Vec2 v = m_ship->getBody()->GetWorldVector(projectileLaunchVel); //with a fairly large magnitude
	//add the ship's forward velocity as well

	v += m_ship->getBody()->GetLinearVelocity();

	if (mouseAiming)
	{
		//float angle_mouse = atan2f(target.y, target.x), angle_ship = atan2f(v.y, v.x);
		//float angle_to_use = angle_mouse - angle_ship;
		//b2Mat22 mat = b2Mat22(cos(angle_to_use), sin(angle_to_use), -sin(angle_to_use), cos(angle_to_use));
		//b2Vec2 modVel = b2MulT(mat, v);

		v = target - m_ship->mBody->GetPosition();
		v.Normalize();
		v *= launchVelocity;// +(m_ship->mBody->GetLinearVelocity().Length() + 1);
		//note: in this iteration, the ship can move faster than the launch velociy so take that into account as well
		//		i think the solution should be to limit the the maximum speed of the ship...
		m_launcher->Shoot(p, v, m_ship->getBody()->GetAngle());
	}
	else
	{
		m_launcher->Shoot(p, v, m_ship->getBody()->GetAngle());
	}
}

void NeoShipController::Draw(Shader* shader, Surface* surface)
{
	//DrawString(5, 25, "Target:	 (%.3f,%.3f)", target.x, target.y);
	DrawString(5, 25, "screenSize: (%d,%d)", Game::AppCam->Width, Game::AppCam->Height);

	glm::vec2 screenExtents = Game::AppCam->GetScreenExtents();
	DrawString(5, 50, "screen extents (source): (%.3f,%.3f)", screenExtents.x, screenExtents.y);
	DrawString(5, 125,	"camcontroller boundary size: (%.3f,%.3f)", m_camera_controller->transform.size.x, m_camera_controller->transform.size.y);
	//DrawString(5, 25, "Target: (%.3f,%.3f)", 1.0f, 1.0f, 1.0f, 1.0f,  target.x, target.y);
	DrawString(200, 75, "cam Spd: (%.3f)", m_camera_controller->rec_followSpeed);
	DrawString(250, 0, "target last pos: (%.3f, %.3f)", m_camera_controller->TargetLastPosition.x, m_camera_controller->TargetLastPosition.y);


	m_ship->Draw();
	m_ship->Draw(shader, surface);

	m_camera_controller->Draw(shader, surface);
}

void NeoShipController::Debug()
{
	m_camera_controller->Debug();
	if (ImGui::TreeNode("- Neo Ship Controller -")) {
		ImGui::Checkbox("Using Mouse controls", &this->mouseAiming);
		if (ImGui::TreeNode("- launch config -"))
		{
			ImGui::SliderFloat2("Launch pos", &projectileLocalLaunchPos.x, -3, 3);
			ImGui::SliderFloat2("Launch vel", &projectileLaunchVel.x, -100, 100);
			ImGui::SliderFloat("launch vel -mouse", &launchVelocity, -50, 50);
			ImGui::SliderFloat2("target pos", &target.x, -3, 3);
			ImGui::TreePop();
		}
		m_launcher->Debug();
		m_ship->Debug();
		ImGui::TreePop();
	}
}
