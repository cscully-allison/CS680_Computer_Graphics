#include "tankAI.h"

TankAI::TankAI(){
	AI.base = new Object("tankbase.obj", 500, btVector3(0, 0, 0), btVector3(0, .5, 0), 0, 0, 0, 1);
	AI.head = new Object("tankhead.obj", 500, btVector3(0, 0, 0), btVector3(0, .5, 0), 0, 0, 0, 1);

}

TankAI::~TankAI(){

}

void TankAI::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height){

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(AI.base->GetModel()));
  AI.base->Render(scalar, spec, spot, height);

  glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(AI.head->GetModel()));
  AI.head->Render(scalar, spec, spot, height);
}

Object* TankAI::GetAIBase(){
	return AI.base;
}

Object* TankAI::GetAIHead(){
	return AI.head;
}