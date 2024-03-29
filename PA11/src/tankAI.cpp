#include "tankAI.h"
#include <time.h>
#include <math.h>
#define MAXLIVES 3;
//#define COMPASS [NORTH, WEST, SOUTH, EAST];

TankAI::TankAI(btDiscreteDynamicsWorld* dynamicsWorld){
	// start rand seed
	srand (time (NULL));
	// initalize tanks
	Initialize (dynamicsWorld, one, btVector3 (150,0,200), 1);
	Initialize (dynamicsWorld, two, btVector3 (-100,0,50), 2);
	Initialize (dynamicsWorld, three, btVector3 (300,0,400), 3);
	Initialize (dynamicsWorld, four, btVector3 (200,0,-500), 4);
 }

TankAI::~TankAI(){

}


void TankAI::Initialize(btDiscreteDynamicsWorld* dynamicsWorld, Tank& AI, btVector3 startOrigin, int index){
	// create ovject
	AI.base = new Object("tradeFederationTank2.obj", 500, btVector3(0, 0, 0), btVector3(startOrigin.getX(), 2, startOrigin.getZ()), .9, 0, .9, index);

	SetOrientation(AI);

	//set restrictions for body movement
	AI.base->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	AI.base->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	// set variables
	AI.projectile = NULL;
	AI.lives = MAXLIVES;
	AI.initialTime = 0;
	AI.compassPosition = 4; 
	AI.attack = false;
	AI.timeLeft = 0;
		dynamicsWorld->addRigidBody (AI.base->GetRigidBody());
}

void TankAI::RenderWrapper(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){
	// render tanks in existance
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
}

void TankAI::UpdateWrapper(unsigned int dt, glm::vec4 user, btDiscreteDynamicsWorld* dynamicsWorld){
	// update tanks in exsistance	
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
	if (one.base == NULL)
	{
		Initialize (dynamicsWorld, one, btVector3 (150,0,200), 1);
	}
	if(two.base == NULL)
	{
		Initialize (dynamicsWorld, two, btVector3 (-300,0,200), 2);
	}
	if(three.base == NULL)
	{
		Initialize (dynamicsWorld, three, btVector3 (300,0,400), 3);
	}
	if(four.base == NULL)
	{
		Initialize (dynamicsWorld, four, btVector3 (200,0,-500), 4);
	}
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
					one.base->rotate(glm::vec3 (0.0f, -1.0, 0.0f));
					two.base->rotate(glm::vec3 (0.0f, -1.0, 0.0f));
					three.base->rotate(glm::vec3 (0.0f, -1.0, 0.0f));
					four.base->rotate(glm::vec3 (0.0f, -1.0, 0.0f));
				break;
				//Left
				case 2:
					one.base->rotate(glm::vec3 (0.0f, 1.0, 0.0f));
					two.base->rotate(glm::vec3 (0.0f, 1.0, 0.0f));
					three.base->rotate(glm::vec3 (0.0f, 1.0, 0.0f));
					four.base->rotate(glm::vec3 (0.0f, 1.0, 0.0f));
				break;
				//Forward
				case 3:
					switch (AI.compassPosition){
						//North
						case 1:
							one.base->translate(glm::vec3 (0.0f, 0.0, 25.0f));
							two.base->translate(glm::vec3 (0.0f, 0.0, 25.0f));
							three.base->translate(glm::vec3 (0.0f, 0.0, 25.0f));
							four.base->translate(glm::vec3 (0.0f, 0.0, 25.0f));
						break;
						//West
						case 2:
							one.base->translate(glm::vec3 (+25.0f, 0.0, .0f));
							two.base->translate(glm::vec3 (+25.0f, 0.0, .0f));
							three.base->translate(glm::vec3 (+25.0f, 0.0, .0f));
							four.base->translate(glm::vec3 (+25.0f, 0.0, .0f));
						break;
						//South
						case 3:
							one.base->translate(glm::vec3 (0.0f, 0.0, -25.0f));
							two.base->translate(glm::vec3 (0.0f, 0.0, -25.0f));
							three.base->translate(glm::vec3 (0.0f, 0.0, -25.0f));
							four.base->translate(glm::vec3 (0.0f, 0.0, -25.0f));
						break;
						//East
						case 4:
							one.base->translate(glm::vec3 (-25.0f, 0.0, 0.0f));
							two.base->translate(glm::vec3 (-25.0f, 0.0, 0.0f));
							three.base->translate(glm::vec3 (-25.0f, 0.0, 0.0f));
							four.base->translate(glm::vec3 (-25.0f, 0.0, 0.0f));
						break;
					}
				break;
				//Backwards
				case 4:
					switch (AI.compassPosition){
						//North
						case 1:
							one.base->translate(glm::vec3 (0.0f, 0.0, -25.0f));
							two.base->translate(glm::vec3 (0.0f, 0.0, -25.0f));
							three.base->translate(glm::vec3 (0.0f, 0.0, -25.0f));
							four.base->translate(glm::vec3 (0.0f, 0.0, -25.0f));
						break;
						//West
						case 2:
							one.base->translate(glm::vec3 (-25.0f, 0.0, .0f));
							two.base->translate(glm::vec3 (-25.0f, 0.0, .0f));
							three.base->translate(glm::vec3 (-25.0f, 0.0, .0f));
							four.base->translate(glm::vec3 (-25.0f, 0.0, .0f));
						break;
						//South
						case 3:
							one.base->translate(glm::vec3 (0.0f, 0.0, 25.0f));
							two.base->translate(glm::vec3 (0.0f, 0.0, 25.0f));
							three.base->translate(glm::vec3 (0.0f, 0.0, 25.0f));
							four.base->translate(glm::vec3 (0.0f, 0.0, 25.0f));
						break;
						//East
						case 4:
							one.base->translate(glm::vec3 (25.0f, 0.0, 0.0f));
							two.base->translate(glm::vec3 (25.0f, 0.0, 0.0f));
							three.base->translate(glm::vec3 (25.0f, 0.0, 0.0f));
							four.base->translate(glm::vec3 (25.0f, 0.0, 0.0f));
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
	//LaunchProjectile (AI, dynamicsWorld);

}
void TankAI::LaunchProjectile(Tank AI, btDiscreteDynamicsWorld* dynamicsWorld){
	std::cout << "Tank " << AI.attack << " attacking!" << std::endl; 
	if(AI.projectile == NULL){
		glm::vec4 temp = AI.base->getPosition();
		AI.projectile = new Object("placeholder.obj", 500, btVector3(0, 0, 0), btVector3(temp.x-16, temp.y+2, temp.z-3), 1, 0, 0, 15);
		
		SetOrientation(AI);
		dynamicsWorld->addRigidBody (AI.projectile->GetRigidBody());
		//AI.projectile->applyForce(temp, 0, 0);
	}
}

void TankAI::ProjectileHit (btDiscreteDynamicsWorld* dynamicsWorld, int tankOrGround, int projectile){
	if (projectile == 11){
		dynamicsWorld->removeRigidBody (one.projectile->GetRigidBody());
		delete one.projectile;
		one.projectile = NULL;
	}
	else if (projectile == 12){
		dynamicsWorld->removeRigidBody (two.projectile->GetRigidBody());
		delete two.projectile;
		two.projectile = NULL;
	}
	else if (projectile == 13){
		dynamicsWorld->removeRigidBody (three.projectile->GetRigidBody());
		delete three.projectile;
		three.projectile = NULL;
	}
	else if (projectile == 14){
		dynamicsWorld->removeRigidBody (four.projectile->GetRigidBody());
		delete four.projectile;
		four.projectile = NULL;
	}
	if (tankOrGround > 0){
		std::cout << "Tank " << tankOrGround << " hit!" << std::endl;
	}
}



void TankAI::Hit(btDiscreteDynamicsWorld* dynamicsWorld, int tankNumber){
	// find tank, decrease lives
	// if lives == 0, delete tank
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

void TankAI::setSpec (glm::vec3 s){
	if (one.base != NULL)
		one.base->setSpec (s);
	if (two.base != NULL)
		two.base->setSpec (s);
	if (three.base != NULL)
		three.base->setSpec (s);
	if (four.base != NULL)
		four.base->setSpec (s);
}

void TankAI::SetOrientation(Tank& AI){
	AI.base->setOrientation();

}