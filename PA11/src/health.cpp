#include "health.h"

Health::Health(){
	healthPack = NULL;
	ResetTimer();
}

Health::~Health(){

}

void Health::Update(btDiscreteDynamicsWorld* dynamicsWorld, unsigned int dt){
	if (healthPack != NULL){
		healthPack->setOrientation();
	}

	else if (countdownTimer > 0){
		countdownTimer --;
	}

	else {
		healthPack = new Object ("placeholder.obj", 1, btVector3(0, 0, 0), btVector3(0, 2.5, 0), 0, 0, 0, 6);
		healthPack->setOrientation();
		dynamicsWorld->addRigidBody (healthPack->GetRigidBody());
		healthPack->rotate (glm::vec3(0,1,0));
	}

}

void Health::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height){
	if (healthPack != NULL){
  		glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(healthPack->GetModel()));
  		healthPack->Render(scalar, spec, spot, height);
  }
}

void Health::Collision(){
	if (healthPack != NULL){
		healthPack = NULL;
		countdownTimer = ResetTimer();
	}
}

int Health::ResetTimer(){
	countdownTimer = rand() % 10000;
	//countdownTimer = 0;
}