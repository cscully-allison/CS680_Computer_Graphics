#include "tankAI.h"
#include <time.h>
#include <math.h>
#define MAXLIVES 3;
//#define COMPASS [NORTH, WEST, SOUTH, EAST];

TankAI::TankAI(btDiscreteDynamicsWorld* dynamicsWorld){
	srand (time (NULL));
	Initialize (dynamicsWorld, one, btVector3 (150,0,0), 1);
	Initialize (dynamicsWorld, two, btVector3 (-10,0,5), 2);
	Initialize (dynamicsWorld, three, btVector3 (0,0,100), 3);
	Initialize (dynamicsWorld, four, btVector3 (0,0,-100), 4);
	// Initialize (dynamicsWorld, five, btVector3 (90,0,0));
 }

TankAI::~TankAI(){

}


void TankAI::Initialize(btDiscreteDynamicsWorld* dynamicsWorld, Tank& AI, btVector3 startOrigin, int index){
	AI.base = new Object("tradeFederationTank2.obj", 500, btVector3(0, 0, 0), btVector3(startOrigin.getX(), 5, startOrigin.getZ()), .9, 0, .9, index);
	//adjust so that it lied on top of the base
	//AI.head = new Object("turret.obj", 1000, btVector3(0, 0, 0), btVector3(startOrigin.getX(), 4.0995, startOrigin.getZ()), 1, 0, 0, index);
	SetOrientation(AI);

	AI.lives = MAXLIVES;
	AI.initialTime = 0;
	AI.compassPosition = 4; 
	AI.attack = false;

	//set restrictions for body movement
	AI.base->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	AI.base->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	//set restrictions for head movement
	//AI.head->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	//AI.head->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	// in seconds
	AI.timeLeft = 0;
	dynamicsWorld->addRigidBody (AI.base->GetRigidBody());
  	//dynamicsWorld->addRigidBody (AI.head->GetRigidBody());
}

void TankAI::RenderWrapper(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){
	Render (one, modelMatrix, scalar, spec, spot, height, eyePos);
	Render (two, modelMatrix, scalar, spec, spot, height, eyePos);
	Render (three, modelMatrix, scalar, spec, spot, height, eyePos);
	Render (four, modelMatrix, scalar, spec, spot, height, eyePos);
        // Render (five, modelMatrix, scalar, spec, spot, height, eyePos);
 }

void TankAI::Render (Tank AI, GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){
  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(AI.base->GetModel()));
  AI.base->Render(scalar, spec, spot, height, eyePos);

  //glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(AI.head->GetModel()));
  //AI.head->Render(scalar, spec, spot, height, eyePos);
}

void TankAI::UpdateWrapper(unsigned int dt, glm::vec4 user){
	Update (dt, one, user, 1);
	Update (dt, two, user, 2);
	Update (dt, three, user, 3);
	Update (dt, four, user, 4);
	// Update (dt,five);
}

void TankAI::Update(unsigned int dt, Tank& AI, glm::vec4 user, int position){
	btTransform lower;
	btTransform upper;
	btVector3 upperPos;
	btQuaternion rotato;

	//no action given yet
	if (dt-AI.initialTime >= AI.timeLeft){
		AI.direction = (1 + rand() % 3);

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
					AI.base->translate(glm::vec3 (0.0f, 0.0, 5.0f));
				break;
				//West
				case 2:
					AI.base->translate(glm::vec3 (+5.0f, 0.0, .0f));
				break;
				//South
				case 3:
					AI.base->translate(glm::vec3 (0.0f, 0.0, -5.0f));
				break;
				//East
				case 4:
					AI.base->translate(glm::vec3 (-5.0f, 0.0, 0.0f));
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

	LookForOpponent(AI, user, position);

	if (AI.attack){
		Attack(AI);
	}

	SetOrientation(AI); 
}

void TankAI::LookForOpponent(Tank AI, glm::vec4 user, int position){
	int min = position;
	std::vector<float> EuclidenDist;
	glm::vec4 TankPos = AI.base->getPosition();
	glm::vec4 otherPos = one.base->getPosition();
	EuclidenDist.push_back (EuclidenDistance (TankPos, otherPos));

	otherPos = two.base->getPosition();
	EuclidenDist.push_back (EuclidenDistance (TankPos, otherPos));

	otherPos = three.base->getPosition();
	EuclidenDist.push_back (EuclidenDistance (TankPos, otherPos));

	otherPos = four.base->getPosition();
	EuclidenDist.push_back (EuclidenDistance (TankPos, otherPos));

	EuclidenDist.push_back (EuclidenDistance (TankPos, user));

	for (int i = 0; i < EuclidenDist.size(); i++){
		if (EuclidenDist[i] != 0 || EuclidenDist[min] > EuclidenDist[i]  || EuclidenDist[i] != false ){
			min = i;
		}
	}

	if (EuclidenDist[min] < 50){
		AI.attack = true;
	}	
	else
		AI.attack = false;
}

float TankAI::EuclidenDistance(glm::vec4 one, glm:: vec4 two){
	float p = one.x - two.x;
	float q = one.z - two.z;
	return sqrt (p*p-q*q);
}

void TankAI::Attack(Tank AI){

}

void TankAI::AddHealth(Tank AI){
	if (AI.lives != 5){
		AI.lives ++;
	}
}

Tank TankAI::GetTank (int number){
		switch (number){
		case 1:
			return one;
		break;
		case 2:
			return two;
		break;
		case 3:
			return three;
		break;
		case 4:
			return four;
		break;
		case 5:
			return five;
		break;
	}
}

Object* TankAI::GetAIBase(int number){
	switch (number){
		case 1:
			return one.base;
		break;
		case 2:
			return two.base;
		break;
		case 3:
			return three.base;
		break;
		case 4:
			return four.base;
		break;
		case 5:
			return five.base;
		break;
	}
}

/*Object* TankAI::GetAIHead(int number){
	switch (number){
		case 1:
      std::cout << "hello" << std::endl;
			return one.head;
		break;
		case 2:
			return two.head;
		break;
		case 3:
			return three.head;
		break;
		case 4:
			return four.head;
		break;
		case 5:
			return five.head;
		break;
	}
 }*/

void TankAI::SetOrientation(Tank& AI){
	AI.base->setOrientation();
	//AI.head->setOrientation();
}