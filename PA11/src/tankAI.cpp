#include "tankAI.h"
#define MAXLIFES 3;
#define MAXAMMO 5;

TankAI::TankAI(){
	AI.base = new Object("tankbase.obj", 500, btVector3(0, 0, 0), btVector3(0, .5, 0), 0, 0, 0, 1);
	//adjust so that it lied on top of the base
	AI.head = new Object("turret.obj", 500, btVector3(0, 0, 0), btVector3(0, 10, 0), 0, 0, 0, 1);
	SetOrientation();

	AI.lives = MAXLIFES;
	AI.ammo = MAXAMMO;
	// left, right, forwards, backward, stop
	
	// in seconds
	AI.timeLeft = rand() % 60;
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
	AI.timeLeft --;
	if (AI.timeLeft < 0){
		AI.timeLeft = rand() % 60;
		AI.direction = rand() % 5;
	}
	switch (AI.direction){
		// Left
		case 1:
		break;
		//Right
		case 2:
		break;
		//Forward
		case 3:
		break;
		//Backwards
		case 4:
		break;
	}
	//std:: cout << AI.timeLeft << " " << AI.direction << std:: endl; 
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