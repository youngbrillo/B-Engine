#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//#include <glad/glad.h>

Camera::Camera(glm::vec4 viewPort) :
	  Width((int)viewPort.z), Height((int)viewPort.w)
	, screenCenter(viewPort.z/2, viewPort.w/2)
	, position(0,0, 0)
	, zoom(1.0f), movementSpeed(5.0f), mouseSensitivity(0.1f)
	, pitch(0.0f), yaw(-90.0f)
	, nearPlane(-1.0f), farPlane(1.0f)
	, nP_perspective(0.01f), fP_perspective(200.0f)
	, nP_Ortho(-1000.0f), fp_Ortho(10000.0f)
	, isOrtho(true), readingInput(false), firstMouse(true), enabled(true), rightMouseDown(false)
	, isStatic(false)
	, isNDCOnly(false)
	, mousePoint(0.0f)
	, right(1.0f, 0, 0)
	, up(0, 1, 0)
	, front(0, 0, -1)
{
	//printf("Camera.cpp::Camera():: initializing Camera (%d x %d)\n", Width, Height);
	CalculateProjection();
	CalculateView();
	updateCamVectors();
}

void Camera::Reset()
{
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->position = glm::vec3(0, 0, 0);
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	this->zoom = 1.0f;
	this->isOrtho = true;
	this->isStatic = false;
	this->isNDCOnly = false;
	this->mousePoint = glm::vec2(0.0f);

	this->nearPlane = -1.0f;
	this->farPlane = 1.0f;
	this->updateCamVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	if (isNDCOnly) return glm::mat4(1);

	if (isStatic) {
		return view;
	}
	return CalculateView();
}

glm::mat4 Camera::getProjectionMatrix()
{
	if (isNDCOnly) return glm::mat4(1);
	if (isStatic) {
		return projection;
	}
	return CalculateProjection();
}

void Camera::mouseCallback(GLFWwindow* window, int button, int action, int mode)
{
	if (ImGui::GetIO().WantCaptureMouse) return;


	double xd, yd;
	glfwGetCursorPos(window, &xd, &yd);
	glm::vec2 ps((float)xd, (float)yd);
	glm::vec2 rcmp = ps;

	if (button == GLFW_MOUSE_BUTTON_2)
	{
		if (action == GLFW_PRESS)
		{
			rcmp = this->convertScreenToWorld(ps);
			rightMouseDown = true;
			mousePoint = rcmp;

		}
		if (action == GLFW_RELEASE)
		{
			rightMouseDown = false;
		}
	}
}

void Camera::mouseCallback_Cursor(GLFWwindow* w, double x, double y)
{


	if (rightMouseDown)
	{

		glm::vec2 ps((float)x, (float)y);
		glm::vec2 pw = convertScreenToWorld(ps);

		glm::vec2 diff = pw - mousePoint;
		position.x -= diff.x;
		position.y -= diff.y;
		mousePoint = convertScreenToWorld(ps);
	}
}

void Camera::mouseScroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (ImGui::GetIO().WantCaptureMouse) return;


	if (yoffset > 0) {
		zoom /= 1.1f;
	}
	else {
		zoom *= 1.1f;
	}

	return;
	float clamMin = 0.25, clampMax = 2.25;
	if (zoom > clampMax)
		zoom = clampMax;

	if (zoom < clamMin)
		zoom = clamMin;
}


glm::mat4 Camera::CalculateView()
{

	glm::mat4 _view = glm::mat4(1.0f);
	_view = glm::translate(_view, position);
	_view = glm::lookAt(position, position + front, up);

	view = _view;
	return _view;
}

glm::mat4 Camera::CalculateProjection()
{
	float ratio = Width / Height;

	glm::mat4 _projection = glm::ortho(
		-Width * ratio *	glm::radians(zoom), //left
		Width * ratio *	glm::radians(zoom), //right
		-Height * ratio * glm::radians(zoom), //bottom
		Height * ratio * glm::radians(zoom), //top
		nearPlane,
		farPlane
	);

	//what if perspective??

	projection = _projection;
	return _projection;
}

glm::vec2 Camera::convertScreenToWorld(glm::vec2& mousePos)
{
	//required variables
	glm::mat4 projection = getProjectionMatrix();
	glm::mat4 view = GetViewMatrix();
	glm::vec4 output;

	glm::vec4 NDC;

	{ //get normalized device coords
		float w, h, x, y, ndc_x, ndc_y;
		w = Width * 1.0f;
		h = Height * 1.0f;
		x = mousePos.x;
		y = mousePos.y;



		ndc_x = 2.0 * x / w - 1.0;
		ndc_y = 1.0 - 2.0 * y / h; //invert y axis

		NDC = glm::vec4(ndc_x, ndc_y, 0, 1);
	}

	{
		output = NDC * glm::inverse(projection) * glm::inverse(view);
		{
			output.x += position.x;
			output.y += position.y;
		}
	}

	return glm::vec2(output.x, output.y);
}

void Camera::debug()
{
	//if (ImGui::TreeNode("Camera"))
	if (ImGui::BeginTabItem("Camera"))
	{
		ImGui::Checkbox("static Cam", &isStatic);
		ImGui::Checkbox("NDC cam", &isNDCOnly);
		ImGui::Checkbox("right mouse btn down", &rightMouseDown);
		ImGui::Checkbox("reading input", &readingInput);


		ImGui::SliderFloat3("cam Pos", &position.x, -25, 25);
		ImGui::SliderFloat3("front", &front.x, -1, 1);
		ImGui::SliderFloat3("up", &up.x, -1, 1);
		ImGui::SliderFloat3("right", &right.x, -1, 1);


		ImGui::SliderFloat("mouse Sensitivty", &mouseSensitivity, 0.01, 3);
		ImGui::SliderFloat("pitch", &pitch, -360, 360);
		ImGui::SliderFloat("yaw", &yaw, -360, 360);
		ImGui::SliderFloat("zoom", &zoom, 0, 25);


		ImGui::SliderFloat("Near plane", &nearPlane, 0.01, 100);
		ImGui::SliderFloat("Far  Plane", &farPlane, 0.01, 100);
		ImGui::SliderFloat("cam speed", &movementSpeed, 1, 100);


		ImGui::EndTabItem();
		//ImGui::TreePop();
	}
}

void Camera::updateCamVectors()
{
	glm::vec3 _front;

	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(_front);
	// also re-calculate the Right and Up vector

	glm::vec3 WORLD_UP = glm::vec3(0, 1, 0);
	right = glm::normalize(glm::cross(front, WORLD_UP));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));

	////to-do, check that this is consistent w/ a moving cam
	screenCenter += right.x;
	screenCenter.y += up.y;
}
