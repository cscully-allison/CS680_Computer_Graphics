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
		healthLight = glm::vec3(rand() % 100, 10, rand() % 100);
		healthPack = new Object ("healthpack.obj", 1, btVector3(0, 2.5, 0), btVector3(healthLight.x, 5.5, healthLight.z), 0, 0, 0, 6);
		healthPack->setOrientation();
		dynamicsWorld->addRigidBody(healthPack->GetRigidBody());
		healthPack->rotate (glm::vec3(0,1,0));
	}

}

void Health::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){
	if (healthPack != NULL){
  		glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(healthPack->GetModel())); 
  		spot.value = glm::vec3(healthLight.x, 30.0, healthLight.z);
  		healthPack->Render(scalar, spec, spot, height, eyePos);
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
	countdownTimer = rand() % 100;
	//countdownTimer = 0;
}