#include "ShipController.h"

#include "Game.h"

ShipController* ShipController::g_shipController = nullptr;


ShipController::ShipController(b2World* worldreference, const b2Vec2& initalShipPos, b2Body* anchor, int levelDifficulty)
	: shipCount(3)
	, initialSpawnPoint(initalShipPos)
	, anchorRef(anchor)
	, m_ship(NULL)
	, m_launcher(NULL)
	, autoRespawn(true)
	, respawnTimerCurrent(0.0f), respawnTimerTarget(3.0f)
{
	LaunchShip(worldreference, initalShipPos, anchor);
	m_launcher = new SpaceBuster::AutoLauncher(50);
	shipCount = (levelDifficulty);
	m_inventory = new SpaceBuster::Inventory();


	//set up ui element
	int m_int_min = 0;
	UIElementDefinition UIdef;
	{ //energy meter
		UIdef.position = glm::vec3(-40.0f, -12.0f, 0.0f);
		UIdef.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		UIdef.elementName = "condition bar";
		conditionMeter = UIMeter(&UIdef);
	}
	glm::vec3 offset = glm::vec3(0, -2.0f, 0.0f);

	{ //health meter
		UIdef.position = UIdef.position + offset;
		UIdef.color = glm::vec4(0.353f, 1.0f, 0.0f, 1.0f);
		UIdef.elementName = "Energy bar";
		energyMeter = UIMeter(&UIdef);
	}

	{ //ammo meter
		UIdef.position = UIdef.position + offset;
		UIdef.color = glm::vec4(0.0f, .0f, 1.0f, 1.0f);
		UIdef.elementName = "ammo bar";
		ammoMeter = UIMeter(&UIdef, &m_launcher->magCurrent, &m_int_min, &m_launcher->magCapacity);
	}


}

ShipController::~ShipController()
{
	anchorRef = nullptr;
	delete m_ship; m_ship = nullptr;
	delete m_launcher; m_launcher = nullptr;
}

void ShipController::LaunchShip(b2World* worldreference, const b2Vec2& initalShipPos, b2Body* anchor)
{
	if (shipCount <= 0) return;

	if (m_ship != nullptr)
	{
		m_ship->mainBody->SetUserData(nullptr);
		delete m_ship;
	}

	m_ship = new Spaceship(worldreference, initalShipPos, anchor);
	m_ship->mainBody->SetUserData(this);
	--shipCount;
}

void ShipController::FixedUpdate(float deltaTime)
{
	if (m_ship == nullptr) return;
	m_ship->FixedUpdate(deltaTime);
	if (m_ship->broken || !m_ship->operational) return;
	if (glfwGetKey(Game::mainWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		b2Vec2 p = m_ship->getBody()->GetWorldPoint(m_launcher->projectileStartPos); //velocity should be in the direction the player is facing
		b2Vec2 v = m_ship->getBody()->GetWorldVector(m_launcher->projectileVelocity); //with a fairly large magnitude
		//add the ship's forward velocity as well

		v += m_ship->getBody()->GetLinearVelocity();

		if (glfwGetKey(Game::mainWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			m_launcher->ShootBounce(p, v);
		else
			m_launcher->Shoot(p, v);
	}
}

void ShipController::Update(float deltaTime)
{


	if (m_ship == nullptr) {
		if (autoRespawn)
		{
			respawnTimerCurrent += deltaTime;
			if (respawnTimerCurrent >= respawnTimerTarget)
			{
				LaunchShip(Game::m_World, initialSpawnPoint, anchorRef);
				respawnTimerCurrent = 0.0f;
			}
		}
		return;
	}
	m_ship->Update(deltaTime);
	m_launcher->Update(deltaTime);
	if (m_ship->broken)
	{
		//for future iterations i want to be able to collect destroyed inoperable ships,
		//and i think i will just have a "scrapped ship" type of item appear at the ships current location
		//and i think i will do it HERE, just fyi, remember to remember
		
		//count down to destroy, maybe play a little animation here
		
		respawnTimerCurrent += deltaTime; //the first time the ship breaks, the respawntimer should be 0.0, but somehow this could be circumvented, make this the case in the contact methods mkay :)
		if (respawnTimerCurrent >= respawnTimerTarget)
		{
			delete m_ship;
			m_ship = nullptr;
			respawnTimerCurrent = 0.0f;
		}
	}
}

void ShipController::handleBeginContact(b2Contact* contact)
{
	if (m_ship == nullptr) return;
	m_ship->handleBeginContact(contact);
}

void ShipController::handlePreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
	if (m_ship == nullptr) return;
	m_ship->handlePreSolve(contact, oldManifold);
	m_launcher->handleCollision(contact);
}

void ShipController::handlePostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	if (m_ship == nullptr) return;
	m_ship->handlePostSolve(contact, impulse);
}

void ShipController::handleEndContact(b2Contact* contact)
{
	if (m_ship == nullptr) return;
	m_ship->handleEndContact(contact);
}

void ShipController::onKeyPress(int key)
{
	if (m_ship == nullptr) return;
	m_ship->onKeyPress(key);

	switch (key)
	{
	case GLFW_KEY_R:
		m_launcher->Reload();
		break;
	default:
		break;
	}

}

void ShipController::onKeyRelease(int key)
{
	if (m_ship == nullptr) return;
	m_ship->onKeyRelease(key);
}
void ShipController::onKeyRepeat(int key)
{
	if (m_ship == nullptr) return;
	m_ship->onKeyRepeat(key);

}
#include "imgui.h"

void ShipController::Debug()
{
	if (ImGui::TreeNode("- Ship Controller -"))
	{

		ImGui::SliderInt("Ships Left", &shipCount, 0, 3);
		if (ImGui::Button("Respawn"))
		{
			LaunchShip(Game::m_World, initialSpawnPoint, anchorRef);
		}

		m_inventory->Debug();

		if (m_ship)
			m_ship->Debug();
		m_launcher->Debug();

		ImGui::TreePop();
	}
}

bool ShipController::AddToInventory(SpaceBuster::Item* item)
{
	m_inventory->AddItem(item);

	bool canAutoUse = false;

	switch (item->GetType())
	{
		case SpaceBuster::ItemType::SparePart:
			canAutoUse = m_ship->condition < m_ship->maxCondition;
			printf("auto use spare parts (%.2f,%.2f)? %s/%s\n", m_ship->condition, m_ship->maxCondition, canAutoUse ? "Yes": "No", m_ship->condition < m_ship->maxCondition ? "Yes" : "No" );
			break;
		case SpaceBuster::ItemType::EnergyCell:
			canAutoUse = m_ship->energy < m_ship->maxEnergy;
			printf("auto use energy cell (%.2f,%.2f)? %s/%s\n", m_ship->energy, m_ship->maxEnergy, canAutoUse ? "Yes" : "No", m_ship->energy < m_ship->maxEnergy ? "Yes" : "No");
			break;
		default:
			break;
	}
	//if (item->GetType() == SpaceBuster::ItemType::SparePart && m_ship->condition < m_ship->maxCondition)
	//{
	//	canAutoUse == m_ship->condition < m_ship->maxCondition;
	//	printf("auto use spare parts (%.2f,%.2f)? %s/%s\n", m_ship->condition, m_ship->maxCondition, canAutoUse ? "Yes": "No", m_ship->condition < m_ship->maxCondition ? "Yes" : "No" );

	//}
	//if (item->GetType() == SpaceBuster::ItemType::EnergyCell)
	//{
	//	canAutoUse == m_ship->energy < m_ship->maxEnergy;
	//	printf("auto use energy cell (%.2f,%.2f)? %s/%s\n", m_ship->energy, m_ship->maxEnergy, canAutoUse ? "Yes" : "No", m_ship->energy < m_ship->maxEnergy ? "Yes" : "No");
	//}

	if(item->autoUse && canAutoUse)
	{
		m_inventory->useItem(item);
	}
	return true;
	//return false;
}

bool ShipController::removeFromInventory(SpaceBuster::Item* item)
{
	return false;
}

b2Body* ShipController::getBody()
{
	if (m_ship == nullptr) 
		return nullptr;
	else
		return m_ship->mainBody;
}


void ShipController::Draw()
{
}
void ShipController::DrawUI(Shader* customShader, Surface* customSurface)
{
	if(m_ship) conditionMeter.DrawValue(customShader, customSurface, m_ship->condition / m_ship->maxCondition);
	if (m_ship) energyMeter.DrawValue(customShader, customSurface, m_ship->energy / m_ship->maxEnergy );
	ammoMeter.Draw(customShader, customSurface);
}