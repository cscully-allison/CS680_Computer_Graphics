#include "tankAI.h"
#include <time.h>
#define MAXLIVES 3;
//#define COMPASS [NORTH, WEST, SOUTH, EAST];

TankAI::TankAI(){
	srand (time (0));
	AI.base = new Object("tankbase.obj", 50, btVector3(0, 0, 0), btVector3(0, -2.5, 0), 0, 0, 0, 1);
	//adjust so that it lied on top of the base
	AI.head = new Object("turret.obj", 50, btVector3(0, 0, 0), btVector3(0, 4.0995, 0), 0, 0, 0, 1);
	SetOrientation();

	AI.lives = MAXLIVES;
	AI.initialTime = 0;
	AI.compassPosition = 4; 
	// left, right, forwards, backward, stop

	//set restrictions for body movement
	AI.base->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	AI.base->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	//set restrictions for head movement
	AI.head->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	AI.head->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	
	// in seconds
	AI.timeLeft = 0;

}

TankAI::~TankAI(){

}

void TankAI::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height){

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(AI.base->GetModel()));
  AI.base->Render(scalar, spec, spot, height);

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(AI.head->GetModel()));
  AI.head->Render(scalar, spec, spot, height);
}

void TankAI::Update(unsigned int dt){
	btTransform lower;
	btTransform upper;
	btVector3 upperPos;
	btQuaternion rotato;

	//no action given yet
	if (dt-AI.initialTime >= AI.timeLeft){
		AI.direction = (1+ rand() % 4);

		//2-4 seconds for movement
		if (AI.direction == 3 || AI.direction == 4)
		{
			AI.timeLeft = (rand() % 2000)+2000;
		}
		//4 seconds for turns
		else
		{
			AI.timeLeft = 4000;
		}

		switch (AI.direction){
			case 1:
				AI.compassPosition --;
				if (AI.compassPosition < 1)
					AI.compassPosition = 4;
			break;
			case 2:
				AI.compassPosition ++;
				if (AI.compassPosition > 4)
					AI.compassPosition = 1;
			break;
		}
		AI.initialTime = dt;
		AI.base->GetRigidBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
		AI.base->GetRigidBody()->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
	}

	//action given
	switch (AI.direction){
		// Right
		case 1:
			AI.base->rotate(glm::vec3 (0.0f, -1.0, 0.0f));
		break;
		//Left
		case 2:
			AI.base->rotate(glm::vec3 (0.0f, 1.0, 0.0f));
		break;
		//Forward
		case 3:
			switch (AI.compassPosition){
				//North
				case 1:
					AI.base->translate(glm::vec3 (0.0f, 0.0, 1.0f));
				break;
				//West
				case 2:
					AI.base->translate(glm::vec3 (+1.0f, 0.0, .0f));
				break;
				//South
				case 3:
					AI.base->translate(glm::vec3 (0.0f, 0.0, -1.0f));
				break;
				//East
				case 4:
					AI.base->translate(glm::vec3 (-1.0f, 0.0, 0.0f));
				break;
			}
		break;
		//Backwards
		case 4:
			switch (AI.compassPosition){
				//North
				case 1:
					AI.base->translate(glm::vec3 (0.0f, 0.0, -1.0f));
				break;
				//West
				case 2:
					AI.base->translate(glm::vec3 (-1.0f, 0.0, .0f));
				break;
				//South
				case 3:
					AI.base->translate(glm::vec3 (0.0f, 0.0, 1.0f));
				break;
				//East
				case 4:
					AI.base->translate(glm::vec3 (1.0f, 0.0, 0.0f));
				break;
			}
		break; 
		//Nothing
		case 5:
		break;
	}

	AI.base->GetRigidBody()->getMotionState()->getWorldTransform(lower);
	upper = lower;
	upperPos = upper.getOrigin();
	upperPos.setY(upperPos.getY()+6.6); 
	upper.setOrigin(upperPos);
	AI.head->GetRigidBody()->proceedToTransform(upper);

	SetOrientation(); 
}

Object* TankAI::GetAIBase(){
	return AI.base;
}

Object* TankAI::GetAIHead(){
	return AI.head;
}

void TankAI::SetOrientation(){
	AI.base->setOrientation();
	AI.head->setOrientation();
}