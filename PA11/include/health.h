#ifndef HEALTH_H
#define HEALTH_H

#include "object.h"
#include "graphics_headers.h"
 
class Health
{
	public:
		Health();
		~Health();
		void ResetTimer();
		void Update(btDiscreteDynamicsWorld* dynamicsWorld, unsigned int dt);
		void Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos);
		void Collision(btDiscreteDynamicsWorld* dynamicsWorld);
		glm::vec3 GetLighting();

	private:
		Object* healthPack; 
		int countdownTimer;
		glm::vec3 healthLight;
};

#endif 
