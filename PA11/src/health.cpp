#include "health.h"

Health::Health(){
	// set variables
	healthPack = NULL;
	ResetTimer();
}

Health::~Health(){

}

void Health::Update(btDiscreteDynamicsWorld* dynamicsWorld, unsigned int dt){
	// if helathpack exists, move it
	if (healthPack != NULL){
		healthPack->setOrientation();
	}

	// decrease countdown timer
	else if (countdownTimer > 0){
		countdownTimer --;
	}

	else {
		// set position
		int neg = rand() % 2;
		if(neg == 0){
			neg = -1;
		}
		healthLight = glm::vec3((rand() % 500)*neg, 10, (rand() % 500) * neg);
		healthPack = new Object ("healthpack.obj", 1, btVector3(0, 2.5, 0), btVector3(healthLight.x, 5.5, healthLight.z), 0, 0, 0, 6);
		healthPack->setOrientation();
		healthPack->GetRigidBody()->setAngularFactor(btVector3(0.0,1.0,0.0));
		dynamicsWorld->addRigidBody(healthPack->GetRigidBody());
		// begin rotation
		healthPack->rotate (glm::vec3(0,1,0));
	}

}

void Health::Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos){
	// render health pack
	if (healthPack != NULL){
  		glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(healthPack->GetModel())); 
  		spot.value = glm::vec3(healthLight.x, 11.0, healthLight.z);
  		healthPack->Render(scalar, spec, spot, height, eyePos);
  }
}

void Health::Collision(btDiscreteDynamicsWorld* dynamicsWorld){
	// if collided with
	if (healthPack != NULL){
		// delete object
		dynamicsWorld->removeRigidBody (healthPack->GetRigidBody());
		delete healthPack;
		healthPack = NULL;
		// reset timer
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