#include "UserTank.h"
#define MAXLIVES 3;
#define MAXAMMO 5;

UserTank::UserTank(){

	user.base = new Object("tradeFederationTank2.obj", 1000, btVector3(0, 0, 0), btVector3(20, 5, 5), .9, 0, .9, 5);

	//adjust so that it lied on top of the base
	//user.head = new Object("turret.obj", 1000, btVector3(0, 0, 0), btVector3(20, 4.0995, 5), 1, 0, 0, 5);
	user.placeholder = new Object("placeholder.obj");
	user.projectile = NULL;
	SetOrientation();
	//user.lives = MAXLIVES;
	user.lives = 3;
	user.compassPosition = 4; 

	//set restrictions for body movement
	user.base->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	user.base->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	//set restrictions for head movement
	//user.head->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	//user.head->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	

}

UserTank::~UserTank(){

}

void UserTank::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.base->GetModel()));
  user.base->Render(scalar, spec, spot, height, eyePos);

  /*glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.head->GetModel()));
  user.head->Render(scalar, spec, spot, height, eyePos);*/

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.placeholder->GetModel()));
  user.placeholder->Render(scalar, spec, spot, height, eyePos);

  if (user.projectile != NULL){
  	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.projectile->GetModel()));
  	user.projectile->Render(scalar, spec, spot, height, eyePos);
  }
}

void UserTank::Update(std::vector <unsigned int> keyPress, int mouseMovement,int launch, btDiscreteDynamicsWorld* dynamicsWorld){
	btTransform lower;
	btTransform upper;
	btTransform placeholder;
	btVector3 upperPos;
	btVector3 placeholderPos;
	btQuaternion rotato;
	if (launch){
		LaunchProjectile(dynamicsWorld);
	}

	for (int i = 0; i < keyPress.size(); i ++){
		switch (keyPress[i]){
			//forward
			case 119:
				switch (user.compassPosition){
					//North
					case 1:
						user.base->translate(glm::vec3 (0.0f, 0.0, 1.0f));
					break;
					//West
					case 2:
						user.base->translate(glm::vec3 (+1.0f, 0.0, .0f));
					break;
					//South
					case 3:
						user.base->translate(glm::vec3 (0.0f, 0.0, -1.0f));
					break;
					//East
					case 4:
						user.base->translate(glm::vec3 (-1.0f, 0.0, 0.0f));
					break;
				}
			break;

			//backwards
			case 115:
				switch (user.compassPosition){
					//North
					case 1:
						user.base->translate(glm::vec3 (0.0f, 0.0, -1.0f));
					break;
					//West
					case 2:
						user.base->translate(glm::vec3 (-1.0f, 0.0, .0f));
					break;
					//South
					case 3:
						user.base->translate(glm::vec3 (0.0f, 0.0, 1.0f));
					break;
					//East
					case 4:
						user.base->translate(glm::vec3 (1.0f, 0.0, 0.0f));
					break;
				}
			break;

			//left
			case 97:
				user.base->rotate(glm::vec3 (0.0f, 360.0/800 * (mouseMovement - previousMouse), 0.0f));
				previousMouse = mouseMovement;
			break;

			//right 
			case 100:
				user.base->rotate(glm::vec3 (0.0f, 360.0/800 * (mouseMovement - previousMouse), 0.0f));
				previousMouse = mouseMovement;
			break;

	
		}
	}
	// if (mouseMovement != 0){
	// 	std::cout << 360.0/800 * mouseMovement - previousMouse << std::endl;
	// 	user.head->rotate (glm::vec3(0.0f, 360.0/800 * (mouseMovement - previousMouse), 0.0f));
	// }

	/*user.base->GetRigidBody()->getMotionState()->getWorldTransform(lower);
	upper = lower;
	upperPos = upper.getOrigin();
	upperPos.setY(upperPos.getY()+6.6); 
	upper.setOrigin(upperPos);
	//user.head->GetRigidBody()->proceedToTransform(upper);*/
	glm::vec4 basePosition = user.base->getPosition();
	user.placeholder->translateModel(basePosition);

	SetOrientation(); 
}

void UserTank::LaunchProjectile(btDiscreteDynamicsWorld* dynamicsWorld){
	std::cout << "FIRE!" << std::endl;

	if(user.projectile == NULL){
		glm::vec4 temp = user.placeholder->getPosition();
		user.projectile = new Object("placeholder.obj", 500, btVector3(0, 0, 0), btVector3(temp.x, temp.y, temp.z), 1, 0, 0, 15);
		
		SetOrientation();
		dynamicsWorld->addRigidBody (user.projectile->GetRigidBody());
		user.projectile->applyForce();

	}
}

int UserTank::ProjectileHit (btDiscreteDynamicsWorld* dynamicsWorld, int tankOrGround){
	dynamicsWorld->removeRigidBody (user.projectile->GetRigidBody());
	user.projectile = NULL;
	std::cout << tankOrGround << std::endl;
	if (tankOrGround > 0){
		std::cout << "tank hit!" << std::endl;
		return 100;
	}
	return 0;
}

void UserTank::AddHealth(){
	if (user.lives != 5){
		user.lives ++;
	}
}

Object* UserTank::GetBase(){
	return user.base;
}

/*Object* UserTank::GetHead(){
	return user.head;
}*/

Object* UserTank::GetPlaceholder(){
	return user.placeholder;
}

void UserTank::SetOrientation(){
	user.base->setOrientation();
	//user.head->setOrientation();
	if (user.projectile != NULL){
		user.projectile->setOrientation();
	}
}

glm::vec4 UserTank::getPosition()
{
	return user.base->getPosition();
}