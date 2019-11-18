#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(0.0f, 7.0f, -5.0f), //Position
		vector3(0.0f, 0.0f, 0.0f),	//Target
		vector3(0.0f, 8.0f, -4.0f));//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)
	
	//Entity Manager
	m_pEntityMngr = MyEntityManager::GetInstance();

	//add player entity
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Player");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::translate(vector3(0.0f,-2.0f,-2.0f)));
	m_pEntityMngr->SetAxisVisibility(true);

	//add an entity
	m_pEntityMngr->AddEntity("Sorted\\Pawn.obj", "Pin");
	//set the model matrix and visibility of the last entity added
	m_pEntityMngr->SetModelMatrix(glm::scale(vector3(0.3f)) * glm::translate(vector3(0.0f, -4.5f, 8.0f)));
	m_pEntityMngr->SetAxisVisibility(true);


	//add floor
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj","Floor");
	m_pEntityMngr->SetModelMatrix(glm::scale(vector3(0.0f,0.1f,10.0f)) * glm::translate(vector3(0.0f, -2.0f, 0.0f)));
	m_pEntityMngr->SetAxisVisibility(true);

	//add left wall
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Left_Wall");
	m_pEntityMngr->SetModelMatrix(glm::scale(vector3(1.0f,0.5f,10.0f)) * glm::translate(vector3(-2.0f, 0.0f, 0.0f)));
	m_pEntityMngr->SetAxisVisibility(true);

	//add Right wall
	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Right_Wall");
	m_pEntityMngr->SetModelMatrix(glm::scale(vector3(1.0f,0.5f,10.0f)) * glm::translate(vector3(2.0f, 0.0f, 0.0f)));
	m_pEntityMngr->SetAxisVisibility(true);


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
	
	////Set model matrix to the creeper
	//matrix4 mCreeper = glm::translate(m_v3Creeper) * ToMatrix4(m_qCreeper) * ToMatrix4(m_qArcBall);
	//m_pEntityMngr->SetModelMatrix(mCreeper, "Creeper");
	//
	//
	////Set model matrix to Steve
	//matrix4 mSteve = glm::translate(vector3(2.5f, 0.0f, 0.0f)) * glm::rotate(IDENTITY_M4, glm::radians(-55.0f), AXIS_Z);
	//m_pEntityMngr->SetModelMatrix(mSteve, "Steve");
	//
	//
	////Move the last entity added slowly to the right
	//matrix4 lastMatrix = m_pEntityMngr->GetModelMatrix();// get the model matrix of the last added
	//lastMatrix *= glm::translate(IDENTITY_M4, vector3(0.01f, 0.0f, 0.0f)); //translate it
	//m_pEntityMngr->SetModelMatrix(lastMatrix); //return it to its owner

	uint uTemp = m_pEntityMngr->GetEntityIndex("Ball");
	if (uTemp != -1)
	{
		matrix4 lastMatrix = m_pEntityMngr->GetModelMatrix(uTemp);
		lastMatrix *= glm::translate(IDENTITY_M4, vector3(0.0f, 0.0f, 0.15f));
		m_pEntityMngr->SetModelMatrix(lastMatrix, uTemp);
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
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
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