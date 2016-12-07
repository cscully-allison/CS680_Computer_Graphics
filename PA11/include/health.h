#ifndef HEALTH_H
#define HEALTH_H

#include "object.h"
#include "graphics_headers.h"
 
class Health
{
	public:
		Health();
		~Health();
		int ResetTimer();
		void Update(btDiscreteDynamicsWorld* dynamicsWorld, unsigned int dt);
		void Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos);
		void Collision();

	private:
		Object* healthPack; 
		int countdownTimer;
};

#endif 
