#include "CameraController.h"

#include <imgui.h>
#include "ResourceManager.h"


CameraController::CameraController(Camera* targetCam, const b2Body* targetBody, glm::vec2 offset, glm::vec2 limits)
	: cam(targetCam)
	, target(targetBody)
	, cameraOffset(offset)
	, boundLimits(limits)
	, enabled(true)
	, sleep(false)
	, mode(FollowMode::Instant)
	, followSpeed(130.0f)
	, DrawDebug(false)
	, TargetLastPosition(0.0f)
{
	if (targetBody == NULL) enabled = false;
	transform.pivot = glm::vec3(0.0f);
	if (enabled)
	{
		TargetLastPosition = glm::vec2(target->GetPosition().x, target->GetPosition().x);
	}
}

CameraController::~CameraController()
{
	target = NULL;
	cam = NULL;
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
	//if (onTarget()) return;
	//get screen limits
	glm::vec2 A(0.0f), B(cam->Width, cam->Height), AA, BB, T, Direction(0.0f);
	//A	= A + boundLimits;
	//B	= B - boundLimits;
	AA	= cam->convertScreenToWorld(A);
	AA += glm::vec2(boundLimits.x, -boundLimits.y);
	BB	= cam->convertScreenToWorld(B);
	BB += glm::vec2(-boundLimits.x, boundLimits.y);
	T	= glm::vec2(target->GetPosition().x, target->GetPosition().y);
	//if above boundary
	if (T.y > AA.y)
	{
		Direction.y = 1.0f;
	}
	//if below boundary
	if (T.y < BB.y)
	{
		Direction.y = -1.0f;
	}
	//if to the right of boundary
	if (T.x > BB.x)
	{
		Direction.x = 1.0f;
	}
	//if to the left of boundary
	if (T.x < AA.x)
	{
		Direction.x = -1.0f;
	}
	if (Direction != glm::vec2(0.0f))
	{
		color = colorOutofbounds;
	}
	else
	{
		color = colorbase;
	}

	transform.size = glm::vec3(abs(BB.x - AA.x), abs(BB.y- AA.y), 1.0f);
	transform.position = cam->position;
	transform.UpdateMatrix();
	moveToTargetAtSpeed(dt, Direction);
}

void CameraController::moveToTargetAtSpeed(float deltaTime, const glm::vec2& direction)
{
	float speedModifier = followSpeed;
	bool v = target->GetLinearVelocity().Length() > followSpeed;
	//if (v)
	//{
	//	//speedModifier = target->GetLinearVelocity().Length();
	//	color = colorlinear;
	//}
	//else
	//{
	//	color = colorfollow;
	//}
	//speedModifier = v ? target->GetLinearVelocity().Length() : followSpeed;
	rec_followSpeed = speedModifier * deltaTime;
	cam->position = cam->position + glm::vec3(direction * rec_followSpeed, 0.0f);

	if (direction == glm::vec2(0.0f)) return;

	//get distance of target
	glm::vec2 targetPos = glm::vec2(target->GetPosition().x, target->GetPosition().y);
	glm::vec2 campos = glm::vec2(cam->position);
	TargetLastPosition = targetPos - campos;
	TargetLastPosition = campos - targetPos;

	glm::vec3 camTeleportPos = cam->position + glm::vec3(direction.x * transform.size.x, direction.y * transform.size.y, 0);


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


void CameraController::Draw(Shader* shader, Surface* surface)
{
	if (!DrawDebug) return;

	shader->SetMatrix4("model", transform.m_model)
		.SetVector4f("color", color);

	ResourceManager::GetTexturePtr("default")->Bind();
	surface->Bind();
}

void CameraController::Debug()
{
	if (ImGui::TreeNode("-Camera Controller-"))
	{
		ImGui::Checkbox("Enable", &enabled);
		ImGui::Checkbox("Debug View", &DrawDebug);
		ImGui::ColorEdit4("Debug color", &color.x);
		transform.Debug("Camera Transform");

		static int elem = FollowMode::Bound; //b/c its first
		const char* elem_names[FollowMode::Bound + 1] = { "Instant", "Follow (Lerp)", "Boundary Follow" };
		const char* elem_name = (elem >= 0 && elem < FollowMode::Bound + 1) ? elem_names[elem] : "Unknown";

		elem = this->mode;
		if (ImGui::SliderInt("State", &elem, 0, FollowMode::Bound, elem_name)) {
			this->mode = (FollowMode)elem;
		}

		ImGui::SliderFloat("follow speed", &followSpeed, 0, 100);


		ImGui::SliderFloat2("Offset", &cameraOffset.x, -50, 50);
		ImGui::SliderFloat2("bounds", &boundLimits.x, 0, cam->GetScreenExtents().x/2.0f);

		ImGui::TreePop();
	}
}

