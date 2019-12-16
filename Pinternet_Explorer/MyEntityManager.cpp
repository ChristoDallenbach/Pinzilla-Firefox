#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_entityList.clear();
	
}
void MyEntityManager::Release(void)
{
	for (uint uEntity = 0; uEntity < m_uEntityCount; ++uEntity)
	{
		MyEntity* pEntity = m_entityList[uEntity];
		SafeDelete(pEntity);
	}
	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if(m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if(m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//look one by one for the specified unique id
	for (uint uIndex = 0; uIndex < m_uEntityCount; ++uIndex)
	{
		if (a_sUniqueID == m_entityList[uIndex]->GetUniqueID())
			return uIndex;
	}
	//if not found return -1
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	//if the list is empty return blank
	if (m_entityList.size() == 0)
		return nullptr;

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetModel();
	}
	return nullptr;
}
MyRigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	//if the list is empty return blank
	if (m_entityList.size() == 0)
		return nullptr;

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetRigidBody();
}
MyRigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetRigidBody();
	}
	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	//if the list is empty return blank
	if (m_entityList.size() == 0)
		return IDENTITY_M4;

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		return pTemp->GetModelMatrix();
	}
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->SetModelMatrix(a_m4ToWorld);
	}
}
void Simplex::MyEntityManager::SetAxisVisibility(bool a_bVisibility, uint a_uIndex)
{
	//if the list is empty return blank
	if (m_entityList.size() == 0)
		return;

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->SetAxisVisible(a_bVisibility);
}
void Simplex::MyEntityManager::SetAxisVisibility(bool a_bVisibility, String a_sUniqueID)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->SetAxisVisible(a_bVisibility);
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	//if the list is empty return blank
	if (m_entityList.size() == 0)
		return;

	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount -1;

	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
vector3 Simplex::MyEntityManager::GetForward(Simplex::String a_sUniqueID)
{
	return m_entityList[GetEntityIndex(a_sUniqueID)]->GetForward();
}

void Simplex::MyEntityManager::Update(void)
{
	//check collisions
	for (uint i = 0; i < m_uEntityCount - 1; i++)
	{
		for (uint j = i + 1; j < m_uEntityCount; j++)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}

void Simplex::MyEntityManager::Update(int *health) {
	//check collisions

	// The Items m_uNumId
	// Player = 0, Pin = 1, Ball = 2, Left_Wall = 3, Right_Wall = 4, Floor = 5
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		//checks if the object is out of bounds on the x axis. If it is, it reverses its x direction so it bounces off the wall.
		if (m_entityList[i]->GetRigidBody()->GetMaxGlobal().x >= 9.0f || m_entityList[i]->GetRigidBody()->GetMinGlobal().x <= -9.0f) {
			vector4 tempVert = vector4(m_entityList[i]->GetVelocity(), 0);
			matrix4 modelMatrix = m_entityList[i]->GetModelMatrix();

			tempVert = tempVert * glm::inverse(modelMatrix);
			vector3 tempVert2 = glm::reflect(vector3(tempVert), vector3(1, 0, 0));
			tempVert2 = vector3(vector4(tempVert2, 0) * modelMatrix);
			m_entityList[i]->SetVelocity(tempVert2);
		}
		//checks if the object is out of bounds on the z axis. If it is, it reverses its x direction so it bounces off the wall.
		if (m_entityList[i]->GetRigidBody()->GetMaxGlobal().z >= 17.5f || m_entityList[i]->GetRigidBody()->GetMinGlobal().z <= -1.5f) {
			vector4 tempVert = vector4(m_entityList[i]->GetVelocity(), 0);
			matrix4 modelMatrix = m_entityList[i]->GetModelMatrix();

			tempVert = tempVert * glm::inverse(modelMatrix);
			vector3 tempVert2 = glm::reflect(vector3(tempVert), vector3(0, 0, 1));
			tempVert2 = vector3(vector4(tempVert2, 0) * modelMatrix);
			m_entityList[i]->SetVelocity(tempVert2);
		}

		for (uint j = i + 1; j < m_uEntityCount; j++)
		{
			// spatial optimization
			if (m_entityList[i]->CheckDimension(m_entityList[j]))
			{
				bool tempbool = m_entityList[i]->IsColliding(m_entityList[j]);

				// player and pin
				//if (m_entityList[i]->GetNumId() == 0 && m_entityList[j]->GetNumId() == 1 && tempbool) {
				//	m_entityList[i]->GetRigidBody()->RemoveCollisionWith(m_entityList[j]->GetRigidBody());
				//	health -= 1;
				//	RemoveEntity(j);
				//	tempbool = false;
				//}
				// if the ball and the pin are colliding
				if (((m_entityList[i]->GetNumId() == 1 && m_entityList[j]->GetNumId() == 2) || (m_entityList[i]->GetNumId() == 2 && m_entityList[j]->GetNumId() == 1)) && tempbool) {
					
					MyEntity* tempBall;
					MyEntity* tempPin;
					
					if (m_entityList[i]->GetNumId() == 2)
					{
						tempBall = m_entityList[i];
						tempPin = m_entityList[j];
					}
					else
					{
						tempBall = m_entityList[j];
						tempPin = m_entityList[i];
					}				

					vector3 collideDirection = glm::normalize(tempBall->GetRigidBody()->GetCenterGlobal());
					collideDirection.y = 0;

					vector3 newDirection1 = glm::rotateY(collideDirection, glm::radians(45.0f));

					vector3 newDirection2 = glm::rotateY(collideDirection, glm::radians(-45.0f));

					vector3 minPosition = tempPin->GetRigidBody()->GetMinGlobal();
					minPosition.y = 0;
					minPosition = tempPin->GetRigidBody()->GetCenterGlobal() - minPosition;
					matrix4 firstPin = tempPin->GetModelMatrix();
					firstPin = glm::translate(firstPin, newDirection1 * 5.0f);


					vector3 maxPosition = tempPin->GetRigidBody()->GetMaxGlobal();
					maxPosition.y = 0;
					maxPosition = tempPin->GetRigidBody()->GetCenterGlobal() - maxPosition;
					matrix4 secondPin = tempPin->GetModelMatrix();
					secondPin = glm::translate(secondPin, newDirection2 * 5.0f);

					AddTypeEntity("Sorted\\Pawn.obj", 1, "Pin");
					m_entityList[m_uEntityCount - 1]->SetModelMatrix(firstPin);
					//SetVelocity to newDirection1 * speed
					m_entityList[m_uEntityCount - 1]->SetVelocity(newDirection1 * .3f);
					SetDimension(m_uEntityCount - 1);

					AddTypeEntity("Sorted\\Pawn.obj", 1, "Pin");
					m_entityList[m_uEntityCount - 1]->SetModelMatrix(secondPin);
					//SetVelocity to newDirection2 * speed
					m_entityList[m_uEntityCount - 1]->SetVelocity(newDirection2 * .3f);
					SetDimension(m_uEntityCount - 1);

					m_entityList[i]->GetRigidBody()->RemoveCollisionWith(m_entityList[j]->GetRigidBody());
					RemoveEntity(i);
					RemoveEntity(j);
					tempbool = false;
				}

				// pin and pin collision
			    //if (m_entityList[i]->GetNumId() == 1 && m_entityList[j]->GetNumId() == 1 && tempbool)
				//{		 		    	
			    //	vector3 newVeli = m_entityList[i]->GetVelocity();
			    //	vector3 newVelj = m_entityList[j]->GetVelocity();
			    //
			    //	if ((newVeli.x < 0 && newVelj.x > 0) || (newVeli.x > 0 && newVelj.x < 0)) 
				//	{
			    //		newVeli.x = newVeli.x - m_entityList[j]->GetVelocity().x;
			    //		newVelj.x = newVelj.x - m_entityList[i]->GetVelocity().x;
			    //	}
			    //
			    //	if ((newVeli.z < 0 && newVelj.z > 0) || (newVeli.z > 0 && newVelj.z < 0)) 
				//	{
			    //		newVeli.z = newVeli.z - m_entityList[j]->GetVelocity().z;
			    //		newVelj.z = newVelj.z - m_entityList[i]->GetVelocity().z;
			    //	}
			    //
			    //	newVeli.y = 0;
			    //	newVelj.y = 0;
			    //			  
			    //	m_entityList[i]->SetVelocity(newVeli);
			    //	m_entityList[j]->SetVelocity(newVelj);
			    //	tempbool = false;
			    //}
			}
		}
	}


	//seperate algo for pin to pin collision
	for (uint e = 0; e < m_uEntityCount; e++) {
		if (m_entityList[e]->GetNumId() == 1) {
			for (uint r = e + 1; r < m_uEntityCount; r++)
			{
				// spatial optimization
				if (m_entityList[e]->CheckDimension(m_entityList[r])) 
				{
					if (m_entityList[r]->GetNumId() == 1 && m_entityList[e]->IsColliding(m_entityList[r])) {

						MyEntity* PinE = m_entityList[e];
						MyEntity* PinR = m_entityList[r];

						vector3 VelocityE = PinE->GetVelocity();
						vector3 VelocityR = PinR->GetVelocity();

						vector3 normalvector = PinE->GetRigidBody()->GetCenterGlobal() - PinR->GetRigidBody()->GetCenterGlobal();
						normalvector.y = 0;						
						
						//VelocityE = glm::reflect(VelocityE, normalvector);
						//VelocityR = glm::reflect(VelocityR, normalvector);

						PinE->SetVelocity(VelocityR);
						PinR->SetVelocity(VelocityE);

						m_entityList[e]->GetRigidBody()->RemoveCollisionWith(m_entityList[r]->GetRigidBody());

						//uint angle = (glm::dot(VelocityE, VelocityR) / (glm::dot(glm::length(VelocityE), glm::length(VelocityR))));
						//angle = glm::acos(angle);
					}
				}
			}
		}
	}
}

void Simplex::MyEntityManager::AddEntity(String a_sFileName, uint a_uNumId, String a_sUniqueID)
{
	//Create a temporal entity to store the object
	MyEntity* pTemp = new MyEntity(a_sFileName, a_sUniqueID);
	//if I was able to generate it add it to the list
	if (pTemp->IsInitialized())
	{
		pTemp->SetNumId(a_uNumId);
		m_entityList.push_back(pTemp);
		m_uEntityCount = m_entityList.size();
	}
}

void Simplex::MyEntityManager::AddTypeEntity(String a_sFileName, uint a_uNumId, String a_sID) {
	MyEntity* pTemp = new MyEntity(a_sFileName);
	pTemp->GenUniqueID(a_sID);

	if (pTemp->IsInitialized())
	{
		pTemp->SetNumId(a_uNumId);
		m_entityList.push_back(pTemp);
		m_uEntityCount = m_entityList.size();
	}
}


void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//if the list is empty return
	if (m_entityList.size() == 0)
		return;

	// if out of bounds choose the last one
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	// if the entity is not the very last we swap it for the last one
	if (a_uIndex != m_uEntityCount - 1)
	{
		std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);
	}
	
	//and then pop the last one
	MyEntity* pTemp = m_entityList[m_uEntityCount - 1];
	SafeDelete(pTemp);
	m_entityList.pop_back();
	--m_uEntityCount;
	return;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	int nIndex = GetEntityIndex(a_sUniqueID);
	RemoveEntity((uint)nIndex);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	//if the list is empty return blank
	if (m_entityList.size() == 0)
		return "";
	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_entityList.size())
		a_uIndex = m_entityList.size() - 1;
	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	//if the list is empty return blank
	if (m_entityList.size() == 0)
		return nullptr;

	//if the index is larger than the number of entries we are asking for the last one
	if (a_uIndex >= m_entityList.size())
		a_uIndex = m_entityList.size() - 1;

	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//if out of bounds will do it for all
	if (a_uIndex >= m_uEntityCount)
	{
		//add for each one in the entity list
		for (a_uIndex = 0; a_uIndex < m_uEntityCount; ++a_uIndex)
		{
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
	else //do it for the specified one
	{
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	//Get the entity
	MyEntity* pTemp = MyEntity::GetEntity(a_sUniqueID);
	//if the entity exists
	if (pTemp)
	{
		pTemp->AddToRenderList(a_bRigidBody);
	}
}

vector3 MyEntityManager::GetVelocity(uint a_uIndex) 
{ 
	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetVelocity(); 
}
void MyEntityManager::SetVelocity(vector3 a_v3Velocity, uint a_uIndex) 
{ 
	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	m_entityList[a_uIndex]->SetVelocity(a_v3Velocity); 
}
float MyEntityManager::GetMass(uint a_uIndex) 
{ 
	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	return m_entityList[a_uIndex]->GetMass(); 
}
void MyEntityManager::SetMass(float a_fMass, uint a_uIndex) 
{ 
	// if out of bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	m_entityList[a_uIndex]->SetMass(a_fMass); 
}
uint MyEntityManager::GetCount() { return m_uEntityCount; }

uint MyEntityManager::GetNumId(uint a_uIndex) { return m_entityList[a_uIndex]->GetNumId(); }

void MyEntityManager::SetDimension(uint a_uIndex) 
{
	vector3 v3TempMin = vector3(0.0f);
	vector3 v3TempMax = vector3(0.0f);
	MyEntity* entityTemp = m_entityList[a_uIndex];
	std::vector<uint> listTemp;

	// 18 wide 20 tall

	// top left - 0
	v3TempMin.x = -9.0f;
	v3TempMax.x = 0.0f;
	v3TempMin.z = 8.0f;
	v3TempMax.z = 18.0f;
	
	if (entityTemp->GetRigidBody()->GetMaxGlobal().x >= v3TempMin.x && entityTemp->GetRigidBody()->GetMinGlobal().x <= v3TempMax.x
		&& entityTemp->GetRigidBody()->GetMaxGlobal().z >= v3TempMin.z && entityTemp->GetRigidBody()->GetMinGlobal().z <= v3TempMax.z)
	{
		listTemp.push_back(0);
	}
		
	// top right - 1
	v3TempMin.x = 0.0f;
	v3TempMax.x = 9.0f;
	v3TempMin.z = 8.0f;
	v3TempMax.z = 18.0f;
	
	if (entityTemp->GetRigidBody()->GetMaxGlobal().x >= v3TempMin.x && entityTemp->GetRigidBody()->GetMinGlobal().x <= v3TempMax.x
		&& entityTemp->GetRigidBody()->GetMaxGlobal().z >= v3TempMin.z && entityTemp->GetRigidBody()->GetMinGlobal().z <= v3TempMax.z)
	{
		listTemp.push_back(1);
	}

	// bottom left - 2
	v3TempMin.x = -9.0f;
	v3TempMax.x = 0.0f;
	v3TempMin.z = -2.0f;
	v3TempMax.z = 8.0f;
	
	if (entityTemp->GetRigidBody()->GetMaxGlobal().x >= v3TempMin.x && entityTemp->GetRigidBody()->GetMinGlobal().x <= v3TempMax.x
		&& entityTemp->GetRigidBody()->GetMaxGlobal().z >= v3TempMin.z && entityTemp->GetRigidBody()->GetMinGlobal().z <= v3TempMax.z)
	{
		listTemp.push_back(2);
	}

	// bottom right - 3
	v3TempMin.x = 0.0f;
	v3TempMax.x = 9.0f;
	v3TempMin.z = -2.0f;
	v3TempMax.z = 8.0f;

	if (entityTemp->GetRigidBody()->GetMaxGlobal().x >= v3TempMin.x && entityTemp->GetRigidBody()->GetMinGlobal().x <= v3TempMax.x
		&& entityTemp->GetRigidBody()->GetMaxGlobal().z >= v3TempMin.z && entityTemp->GetRigidBody()->GetMinGlobal().z <= v3TempMax.z)
	{
		listTemp.push_back(3);
	}

	entityTemp->SetDimension(listTemp);
}