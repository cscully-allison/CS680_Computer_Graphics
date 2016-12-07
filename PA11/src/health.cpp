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
		healthLight = glm::vec3(rand() % 100, 2.5, rand() % 100);
		healthPack = new Object ("placeholder.obj", 1, btVector3(0, 0, 0), btVector3(healthLight.x, 0, healthLight.z), 0, 0, 0, 6);
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

void Health::Collision(btDiscreteDynamicsWorld* dynamicsWorld){
	if (healthPack != NULL){
		dynamicsWorld->removeRigidBody (healthPack->GetRigidBody());
		delete healthPack;
		healthPack = NULL;
		ResetTimer();
	}
}

glm::vec3 Health:: GetLighting(){
	if (healthPack != NULL){
		return healthLight;
	}
}

void Health::ResetTimer(){
	countdownTimer = rand() % 10000;
}