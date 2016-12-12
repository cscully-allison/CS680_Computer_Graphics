#include "UserTank.h"
#define MAXLIVES 3;
#define MAXAMMO 5;

UserTank::UserTank(){
	// creat objects and move rigid body to position
	user.base = new Object("tradeFederationTank3.obj", 1000, btVector3(0, 0, 0), btVector3(0, 3.5, 0), .9, 0, .9, 5);
	user.placeholder = new Object("placeholder.obj");
	user.projectile = NULL;
	SetOrientation();

	//set restrictions for body movement
	user.base->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	user.base->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	
	// set variables
	user.lives = 3;
	user.compassPosition = 4; 
}

UserTank::~UserTank(){

}

void UserTank::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){
	// render body
  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.base->GetModel()));
  user.base->Render(scalar, spec, eyePos);



  // glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.placeholder->GetModel()));
  // user.placeholder->Render(scalar, spec, eyePos);

  // render projectile
  if (user.projectile != NULL){
  	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.projectile->GetModel()));
  	user.projectile->Render(scalar, spec, eyePos);
  }
}

void UserTank::Update(std::vector <unsigned int> keyPress, int mouseMovement,int launch, 
							btDiscreteDynamicsWorld* dynamicsWorld, unsigned int dt, glm::vec3 forwardsVec, glm::quat angle){
	// reset velocity
	btVector3 currentVel = user.base->GetRigidBody()->getLinearVelocity();
	user.base->GetRigidBody()->setLinearVelocity(btVector3(currentVel.getX()/2, currentVel.getY()/2, currentVel.getZ()/2));
	user.base->GetRigidBody()->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));

	// if user has launched, start projectile
	if (launch){
		LaunchProjectile(dynamicsWorld, forwardsVec, angle);
		lastShot = dt;
	}

	// see if the projectile needs to be deleted
	if (user.projectile != NULL && dt - lastShot > 3000){
		dynamicsWorld->removeRigidBody (user.projectile->GetRigidBody());
		user.projectile = NULL;
	}


	// find key pressed
	for (int i = 0; i < keyPress.size(); i ++){
		switch (keyPress[i]){
			//forward
			case 119:
				user.base->translate(glm::vec3(100.0f*forwardsVec.x, 0.0f, 100.0f*forwardsVec.z));
			break;

			//backwards
			case 115:
				user.base->translate(glm::vec3(-100.0f*forwardsVec.x, 0.0f, -100.0f*forwardsVec.z));
			break;

			//left
			case 97:
				user.base->rotate(glm::vec3 (0.0f, 360.0/800 * (1600), 0.0f));	
			break;

			//right 
			case 100:
				user.base->rotate(glm::vec3 (0.0f, 360.0/800 * (-1600), 0.0f));
			break;

	
		}
	}

	// move object
	glm::vec4 basePosition = user.base->getPosition();
	user.placeholder->translateModel(basePosition);
	SetOrientation(); 
}

void UserTank::LaunchProjectile(btDiscreteDynamicsWorld* dynamicsWorld, glm::vec3 forwardsVec, glm::quat angle){
	glm::vec4 userPos = user.base->getPosition();
	// if null
	if(user.projectile == NULL){
		// create object
		user.projectile = new Object("bullet.obj", 10, btVector3(0, 0, 0), 
										btVector3(userPos.x+forwardsVec.x, 6, userPos.z+forwardsVec.z), 0, .9, .9, 15);
		user.projectile->GetRigidBody()->proceedToTransform(btTransform(btQuaternion(angle.x, angle.y, angle.z, angle.w),
																 btVector3(userPos.x+forwardsVec.x, 6, userPos.z+forwardsVec.z)));

		SetOrientation();
		dynamicsWorld->addRigidBody (user.projectile->GetRigidBody());
		user.projectile->GetRigidBody()->setGravity(btVector3(0.0f, -0.5f, 0.0f));
		//apply force
		user.projectile->applyForce(forwardsVec);
	}
}

int UserTank::ProjectileHit (btDiscreteDynamicsWorld* dynamicsWorld, int tankOrGround){
	// remove projectile and delete
	dynamicsWorld->removeRigidBody (user.projectile->GetRigidBody());
	delete user.projectile;
	user.projectile = NULL;
	// if a tank is hit
	if (tankOrGround > 0){
		std::cout << "Tank " << tankOrGround << " hit!" << std::endl;
		// add to score
		return 100;
	}
	return 0;
}

void UserTank::setSpec (glm::vec3 s){
	user.base->setSpec (s);
}

void UserTank::Hit(){
	user.lives --;
}

int UserTank::GetLives(){
	return user.lives;
}

void UserTank::AddHealth(){
	if (user.lives != 5){
		user.lives ++;
	}
}

Object* UserTank::GetBase(){
	return user.base;
}

Object* UserTank::GetPlaceholder(){
	return user.placeholder;
}

void UserTank::SetOrientation(){
	user.base->setOrientation();
	if (user.projectile != NULL){
		user.projectile->setOrientation();
	}
}

glm::vec4 UserTank::getPosition()
{
	return user.base->getPosition();
}