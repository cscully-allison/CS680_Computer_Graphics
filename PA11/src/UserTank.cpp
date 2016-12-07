#include "UserTank.h"
#define MAXLIVES 3;
#define MAXAMMO 5;

UserTank::UserTank(){
	user.base = new Object("tankbase.obj", 500, btVector3(0, 0, 0), btVector3(20, -2.5, 5), 10, 0, 0, 1);
	//adjust so that it lied on top of the base
	user.head = new Object("turret.obj", 500, btVector3(0, 0, 0), btVector3(20, 4.0995, 5), 0, 0, 0, 1);
	user.placeholder = new Object("placeholder.obj", 50, btVector3(0, 0, 0), btVector3(20, 4.0995, 10), 0, 0, 0, 1);
	SetOrientation();

	user.lives = MAXLIVES;
	user.compassPosition = 4; 

	//set restrictions for body movement
	user.base->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	user.base->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	//set restrictions for head movement
	user.head->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	user.head->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	user.placeholder->GetRigidBody()->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
	user.placeholder->GetRigidBody()->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
}

UserTank::~UserTank(){

}

void UserTank::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.base->GetModel()));
  user.base->Render(scalar, spec, spot, height, eyePos);

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.head->GetModel()));
  user.head->Render(scalar, spec, spot, height, eyePos);

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.placeholder ->GetModel()));
  user.placeholder ->Render(scalar, spec, spot, height, eyePos);
}

void UserTank::Update(std::vector <unsigned int> keyPress){
	btTransform lower;
	btTransform upper;
	btTransform placeholder;
	btVector3 upperPos;
	btVector3 placeholderPos;
	btQuaternion rotato;

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
				//user.base->rotate(glm::vec3 (0.0f, -1.0, 0.0f));
			break;

			//right 
			case 100:
				//user.base->rotate(glm::vec3 (0.0f, 1.0, 0.0f));
			break;

		}
	}

	user.base->GetRigidBody()->getMotionState()->getWorldTransform(lower);
	upper = lower;
	upperPos = upper.getOrigin();
	upperPos.setY(upperPos.getY()+6.6); 
	upper.setOrigin(upperPos);
	user.head->GetRigidBody()->proceedToTransform(upper);
	
	user.head->GetRigidBody()->getMotionState()->getWorldTransform(lower);
	placeholder = lower;
	placeholderPos = placeholder.getOrigin();
	placeholderPos.setX (placeholderPos.getX()-11);
	placeholder.setOrigin(placeholderPos);
	user.placeholder->GetRigidBody()->proceedToTransform(placeholder);

	SetOrientation(); 
}

Object* UserTank::GetBase(){
	return user.base;
}

Object* UserTank::GetHead(){
	return user.head;
}

Object* UserTank::GetPlaceholder(){
	return user.placeholder;
}

void UserTank::SetOrientation(){
	user.base->setOrientation();
	user.head->setOrientation();
	user.placeholder->setOrientation();
}