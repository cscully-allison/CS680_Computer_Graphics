#include "UserTank.h"
#define MAXLIVES 3;
#define MAXAMMO 5;

UserTank::UserTank(){
	user.base = new Object("tankbase.obj", 50, btVector3(0, 0, 0), btVector3(20, -2.5, 5), 0, 0, 0, 1);
	//adjust so that it lied on top of the base
	user.head = new Object("turret.obj", 50, btVector3(0, 0, 0), btVector3(20, 4.0995, 5), 0, 0, 0, 1);
	SetOrientation();

	user.lives = MAXLIVES;
	user.ammo = MAXAMMO;
	user.compassPosition = 4; 
	
	// left, right, forwards, backward, stop

	//set restrictions for body movement
	user.base->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	user.base->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	//set restrictions for head movement
	user.head->GetRigidBody()->setLinearFactor(btVector3(1.0f, 0.0f, 1.0f));
	user.head->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
}

UserTank::~UserTank(){

}

void UserTank::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height){

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.base->GetModel()));
  user.base->Render(scalar, spec, spot, height);

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(user.head->GetModel()));
  user.head->Render(scalar, spec, spot, height);
}

void UserTank::Update(){
	SetOrientation(); 
}

Object* UserTank::GetBase(){
	return user.base;
}

Object* UserTank::GetHead(){
	return user.head;
}

void UserTank::SetOrientation(){
	user.base->setOrientation();
	user.head->setOrientation();
}