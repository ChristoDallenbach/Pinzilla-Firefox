#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 25.0f, 6.0f), //Position
		vector3(0.0f, 0.0f, 6.0f),	//Target
		vector3(0.0f, 10.0f, 7.0f));//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)
	
	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	//add player entity
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", 0, "Player");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0.0f,-2.0f,-1.0f)));
	m_pEntityMngr->SetAxisVisibility(true);
	m_pEntityMngr->SetDimension(0);

	//add an entity
	m_pEntityMngr->AddEntity("Sorted\\Pawn.obj", 1, "Pin");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::scale(vector3(0.3f)) * glm::translate(vector3(0.0f, -4.5f, 8.0f)));
	m_pEntityMngr->SetAxisVisibility(true);
	m_pEntityMngr->SetDimension(1);

	////add floor
	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", 5, "Floor");
	m_pEntityMngr->SetModelMatrix(glm::scale(IDENTITY_M4, vector3(20.0f,1.0f,40.0f)) * glm::translate(IDENTITY_M4, vector3(-0.5f, -10.0f, -0.1f)));
	m_pEntityMngr->SetAxisVisibility(true);
	m_pEntityMngr->SetDimension(2);

	////add left wall
	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", 3, "Left_Wall");
	m_pEntityMngr->SetModelMatrix(glm::scale(IDENTITY_M4, vector3(1.0f,20.0f,40.0f)) * glm::translate(IDENTITY_M4, vector3(-10.0f, -0.5f, -0.1f)));
	m_pEntityMngr->SetAxisVisibility(true);
	m_pEntityMngr->SetDimension(3);

	////add Right wall
	m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", 4, "Right_Wall");
	m_pEntityMngr->SetModelMatrix(glm::scale(IDENTITY_M4, vector3(1.0f,20.0f,40.0f)) * glm::translate(IDENTITY_M4, vector3(10.0f, -0.5f, -0.1f)));
	m_pEntityMngr->SetAxisVisibility(true);
	m_pEntityMngr->SetDimension(4);

	m_iPlayerHealth = 3;

}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	uint uTemp;
	matrix4 lastMatrix;
	matrix4 matrixTemp;
	vector3 v3Temp;

	// Moving the ball
	uTemp = m_pEntityMngr->GetEntityIndex("Ball");
	if (uTemp != -1)
	{
		 lastMatrix = m_pEntityMngr->GetModelMatrix(uTemp);
		 matrixTemp = lastMatrix;
		 v3Temp = m_pEntityMngr->GetVelocity(uTemp);
		 //v3Temp = glm::normalize(v3Temp);
		 lastMatrix *= glm::translate(IDENTITY_M4, v3Temp);
		 if (v3Temp.x <= m_fFriction && v3Temp.x >= -m_fFriction)
		 {
			 v3Temp.x = 0.0f;
		 }
		 else if (v3Temp.x <= m_fFriction)
		 {
			 v3Temp.x += m_fFriction;
		 }
		 else if (v3Temp.x >= m_fFriction)
		 {
			 v3Temp.x -= m_fFriction;
		 }
		 if (v3Temp.z <= m_fFriction && v3Temp.z >= -m_fFriction)
		 {
			 v3Temp.z = 0.0f;
		 }
		 else if (v3Temp.z <= m_fFriction)
		 {
			 v3Temp.z += m_fFriction;
		 }
		 else if (v3Temp.z >= m_fFriction)
		 {
			 v3Temp.z -= m_fFriction;
		 }

		m_pEntityMngr->SetModelMatrix(lastMatrix, uTemp);
		m_pEntityMngr->SetVelocity(v3Temp, uTemp);

		if (lastMatrix != matrixTemp)
		{
			m_pEntityMngr->SetDimension(uTemp);
		}
	}

	// Moving the Player
	uTemp = m_pEntityMngr->GetEntityIndex("Player");
	if (uTemp != -1)
	{
		lastMatrix = m_pEntityMngr->GetModelMatrix(uTemp);
		matrixTemp = lastMatrix;
		v3Temp = m_pEntityMngr->GetVelocity(uTemp);
		//v3Temp = glm::normalize(v3Temp);
		lastMatrix *= glm::translate(IDENTITY_M4, v3Temp);
		if(v3Temp.x <= m_fFriction && v3Temp.x >= -m_fFriction)
		{
		    v3Temp.x = 0.0f;
		}
		else if(v3Temp.x <= m_fFriction)
		{
		    v3Temp.x += m_fFriction;
		}
		else if (v3Temp.x >= m_fFriction)
		{
			v3Temp.x -= m_fFriction;
		}
		if (v3Temp.z <= m_fFriction && v3Temp.z >= -m_fFriction)
		{
			v3Temp.z = 0.0f;
		}
		else if (v3Temp.z <= m_fFriction)
		{
			v3Temp.z += m_fFriction;
		}
		else if (v3Temp.z >= m_fFriction)
		{
			v3Temp.z -= m_fFriction;
		}

		m_pEntityMngr->SetModelMatrix(lastMatrix, uTemp);
		m_pEntityMngr->SetVelocity(v3Temp, uTemp);

		if (lastMatrix != matrixTemp)
		{
			m_pEntityMngr->SetDimension(uTemp);
		}
	}

	// Moving the Pins
	for (int i = 0; i < m_pEntityMngr->GetCount(); i++)
	{	
		if (m_pEntityMngr->GetNumId(i) == 1)
		{
			lastMatrix = m_pEntityMngr->GetModelMatrix(i);
			matrixTemp = lastMatrix;
			v3Temp = m_pEntityMngr->GetVelocity(i);
			//v3Temp = glm::normalize(v3Temp);
			lastMatrix *= glm::translate(IDENTITY_M4, v3Temp);
			if (v3Temp.x <= m_fFriction && v3Temp.x >= -m_fFriction)
			{
				v3Temp.x = 0.0f;
			}
			else if (v3Temp.x <= m_fFriction)
			{
				v3Temp.x += m_fFriction;
			}
			else if (v3Temp.x >= m_fFriction)
			{
				v3Temp.x -= m_fFriction;
			}
			if (v3Temp.z <= m_fFriction && v3Temp.z >= -m_fFriction)
			{
				v3Temp.z = 0.0f;
			}
			else if (v3Temp.z <= m_fFriction)
			{
				v3Temp.z += m_fFriction;
			}
			else if (v3Temp.z >= m_fFriction)
			{
				v3Temp.z -= m_fFriction;
			}

			m_pEntityMngr->SetModelMatrix(lastMatrix, i);
			m_pEntityMngr->SetVelocity(v3Temp, i);

			if (lastMatrix != matrixTemp)
			{
				m_pEntityMngr->SetDimension(i);
			}
		}
	}

	//Update Entity Manager
	m_pEntityMngr->Update(&m_iPlayerHealth);
		
	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();
	
	//m_pMeshMngr->AddCubeToRenderList(glm::scale(vector3(2.0, 1.0, 1.0)),C_RED);
	//RigidBody* pTemp = new RigidBody();

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw the grid
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, vector3(-4.5f, 0.0f, 3.0f)) * glm::scale(vector3(9.0f, 0.0f, 10.0f)), vector3(1.0f, 1.0f, 0.0f), 1); // top left
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, vector3(4.5f, 0.0f, 3.0f)) * glm::scale(vector3(9.0f, 0.0f, 10.0f)), vector3(1.0f, 1.0f, 0.0f), 1); // top right
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, vector3(-4.5f, 0.0f, 13.0f)) * glm::scale(vector3(9.0f, 0.0f, 10.0f)), vector3(1.0f, 1.0f, 0.0f), 1); // bottom left
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, vector3(4.5f, 0.0f, 13.0f)) * glm::scale(vector3(9.0f, 0.0f, 10.0f)), vector3(1.0f, 1.0f, 0.0f), 1); // bottom right

	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the entity manager
	m_pEntityMngr->ReleaseInstance();

	//release GUI
	ShutdownGUI();
}