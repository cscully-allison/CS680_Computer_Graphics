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
	if (one.base != NULL){
		Render (one, modelMatrix, scalar, spec, spot, height, eyePos);
	}
	if (two.base != NULL){
		Render (two, modelMatrix, scalar, spec, spot, height, eyePos);
	}
	if (three.base != NULL){
		Render (three, modelMatrix, scalar, spec, spot, height, eyePos);
	}
	if (four.base != NULL){
		Render (four, modelMatrix, scalar, spec, spot, height, eyePos);
	}
 }

void TankAI::Render (Tank AI, GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){
  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(AI.base->GetModel()));
  AI.base->Render(scalar, spec, eyePos);

  //glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(AI.head->GetModel()));
  //AI.head->Render(scalar, spec, spot, height, eyePos);
}

void TankAI::UpdateWrapper(unsigned int dt, glm::vec4 user, btDiscreteDynamicsWorld* dynamicsWorld){	
	if (one.base != NULL){
		Update (dt, one, user, 1, dynamicsWorld);
	}
	if (two.base != NULL){;
		Update (dt, two, user, 2, dynamicsWorld);
	}
	if (three.base != NULL){
		Update (dt, three, user, 3, dynamicsWorld);
	}
	if (four.base != NULL){
		Update (dt, four, user, 4, dynamicsWorld);
	}
}

void TankAI::Update(unsigned int dt, Tank& AI, glm::vec4 user, int position, btDiscreteDynamicsWorld* dynamicsWorld){
	AI.attack = LookForOpponent(AI, user, position);
	if (AI.attack  > 0){
		Attack (AI, dynamicsWorld);
	}

	else{
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


		}

	SetOrientation(AI); 
}

int TankAI::LookForOpponent(Tank AI, glm::vec4 user, int position){
	int min = position;
	std::vector<float> EuclidenDist;
	glm::vec4 TankPos = AI.base->getPosition();
	glm::vec4 otherPos;
	if (one.base != NULL){
		otherPos = one.base->getPosition();
		EuclidenDist.push_back (EuclidenDistance (TankPos, otherPos));
	}

	if (two.base != NULL){
		otherPos = two.base->getPosition();
		EuclidenDist.push_back (EuclidenDistance (TankPos, otherPos));
	}

	if (three.base != NULL){
		otherPos = three.base->getPosition();
		EuclidenDist.push_back (EuclidenDistance (TankPos, otherPos));
	}

	if (four.base != NULL){
		otherPos = four.base->getPosition();
		EuclidenDist.push_back (EuclidenDistance (TankPos, otherPos));
	}

	EuclidenDist.push_back (EuclidenDistance (TankPos, user));

	for (int i = 0; i < EuclidenDist.size(); i++){
		if (EuclidenDist[i] != 0 || EuclidenDist[min] > EuclidenDist[i]  || EuclidenDist[i] != false ){
			min = i;
		}
	}

	if (EuclidenDist[min] < 30.0){
		return min + 1;
	}	
	else{
		return 0;
	}
}

float TankAI::EuclidenDistance(glm::vec4 one, glm:: vec4 two){
	float p = one.x - two.x;
	float q = one.z - two.z;
	return sqrt (p*p-q*q);
}

void TankAI::Attack(Tank AI , btDiscreteDynamicsWorld* dynamicsWorld){
	//std::cout << "ready for attack" << std::endl;
	LaunchProjectile (AI, dynamicsWorld);

}
void TankAI::LaunchProjectile(Tank AI, btDiscreteDynamicsWorld* dynamicsWorld){
	std::cout << "Tank " << AI.attack << " under attack!" << std::endl; 
	// if(AI.projectile == NULL){
	// 	glm::vec4 temp = AI.base->getPosition();
	// 	AI.projectile = new Object("placeholder.obj", 500, btVector3(0, 0, 0), btVector3(temp.x-16, temp.y+2, temp.z-3), 1, 0, 0, 15);
		
	// 	SetOrientation(AI);
	// 	dynamicsWorld->addRigidBody (AI.projectile->GetRigidBody());
	// 	AI.projectile->applyForce(temp, 0, 0);
	// }
}



void TankAI::Hit(btDiscreteDynamicsWorld* dynamicsWorld, int tankNumber){

	switch (tankNumber){
		case 1:
			one.lives --;
			if (one.lives <= 0){
				dynamicsWorld->removeRigidBody (one.base->GetRigidBody());
				delete one.base;
				one.base = NULL;
			}
		break;
		case 2:
			two.lives --;
			if (two.lives <= 0){
				dynamicsWorld->removeRigidBody (two.base->GetRigidBody());
				delete two.base;
				two.base = NULL;
			}
		break;
		case 3:
			three.lives --;
			if (three.lives <= 0){
				dynamicsWorld->removeRigidBody (three.base->GetRigidBody());
				delete three.base;
				three.base = NULL;
			}
		break;
		case 4:
			four.lives --;
			if (four.lives <= 0){
				dynamicsWorld->removeRigidBody (four.base->GetRigidBody());
				delete four.base;
				four.base = NULL;
			}
		break; 
		//Nothing
		case 5:
		break;
	}
	//AI.base->GetRigidBody()->getMotionState()->getWorldTransform(lower);

	/*upper = lower;
	upperPos = upper.getOrigin();
	upperPos.setY(upperPos.getY()+6.6); 
	upper.setOrigin(upperPos);
	//AI.head->GetRigidBody()->proceedToTransform(upper);*/

	//SetOrientation(AI); 
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