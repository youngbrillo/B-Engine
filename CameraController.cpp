#include "CameraController.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

CameraController::CameraController(Camera* targetCam, const b2Body* targetBody, glm::vec2 offset, glm::vec2 limits)
	: cam(targetCam)
	, target(targetBody)
	, m_body(NULL)
	, m_screenLeft(NULL), m_screenRight(NULL), m_screen_top(NULL), m_screenBottom(NULL)
	, cameraOffset(offset)
	, boundLimits(limits)
	, enabled(true)
	, sleep(false)
	, mode(FollowMode::Lerp)
	, followSpeed(15.0f)
{
	if (targetBody == NULL) enabled = false;
}

CameraController::~CameraController()
{
	
}

void CameraController::Update(float dt)
{
	if (!enabled) return;
	
	switch (mode)
	{
		case CameraController::Instant:
			Follow();
			break;
		case CameraController::Lerp:
			FollowLerp(dt);
			break;
		case CameraController::Bound:
			FollowBound(dt);
			break;
		default:
			break;
	}
}

bool CameraController::onTarget()
{
	if (!!target == false) return true; //always on target if there is no target
	return cam->position.x == target->GetPosition().x && cam->position.y == target->GetPosition().y;
}

void CameraController::Follow()
{
	if (onTarget()) return;
	cam->position = glm::vec3(target->GetPosition().x + cameraOffset.x, target->GetPosition().y + cameraOffset.y, cam->position.z);
}

void CameraController::FollowLerp(float dt)
{
	if (onTarget()) return;
	glm::vec2 bpos(target->GetPosition().x, target->GetPosition().y);
	glm::vec2 apos = glm::vec2(cam->position) + cameraOffset;

	glm::vec2 dir(0.0f);

	float val = 1.0f;

	float leftBoundary = apos.x - boundLimits.x;
	float rightBoundary = apos.x + boundLimits.x;
	float topBoundary = apos.y + boundLimits.y;
	float bottomboundary = apos.y - boundLimits.y;


	if (bpos.x < leftBoundary) //drift left
	{
		dir.x = -followSpeed * val;
	}
	if (bpos.x > rightBoundary) //drift right
	{
		dir.x = followSpeed * val;
	}
	if (bpos.y > topBoundary) //drift up
	{
		dir.y = followSpeed * val;
	}

	if (bpos.y < bottomboundary) //drift down
	{
		dir.y = -followSpeed * val;
	}

	if (dir == glm::vec2(0.0f)) return;

	dir *= dt;

	cam->position = cam->position + glm::vec3(dir, 0);


	//see why this is getting wonky as heck!
}

void CameraController::FollowBound(float dt)
{
	if (onTarget()) return;

}

void CameraController::handleBeginContact(b2Contact* contact)
{
	if (!enabled) return;
}

void CameraController::handleEndContact(b2Contact* contact)
{
	if (!enabled) return;
}

void CameraController::handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	if (!enabled) return;
}

void CameraController::Debug()
{
	if (ImGui::TreeNode("-Camera Controller-"))
	{
		ImGui::Checkbox("Enable", &enabled);

		static int elem = FollowMode::Bound; //b/c its first
		const char* elem_names[FollowMode::Bound + 1] = { "Instant", "Follow (Lerp)", "Boundary Follow" };
		const char* elem_name = (elem >= 0 && elem < FollowMode::Bound + 1) ? elem_names[elem] : "Unknown";

		elem = this->mode;
		if (ImGui::SliderInt("State", &elem, 0, FollowMode::Bound, elem_name)) {
			this->mode = (FollowMode)elem;
		}

		ImGui::SliderFloat("follow speed", &followSpeed, 0, 5);


		ImGui::SliderFloat2("Offset", &cameraOffset.x, -50, 50);
		ImGui::SliderFloat2("bounds", &boundLimits.x, 0, 50);

		ImGui::TreePop();
	}
}
