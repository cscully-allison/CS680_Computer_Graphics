#ifndef USERTANK_H
#define USERTANK_H

#include <object.h>
#include "graphics_headers.h"
#include "tankAI.h"

class UserTank{
	public:
		UserTank();
		~UserTank();
		void Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos);
		void AddHealth();
		Object* GetBase();
		//Object* GetHead();
		Object* GetPlaceholder();
		void SetOrientation();
		glm::vec4 getPosition();
		void setSpec (glm::vec3 s);

		void LaunchProjectile(btDiscreteDynamicsWorld* dynamicsWorld, glm::vec3 forwardsVec);
		void Hit();
		int GetLives();
		int ProjectileHit (btDiscreteDynamicsWorld* dynamicsWorld, int tankOrGround);
		void Update(std::vector <unsigned int> keyPress, int mouseMovement, int launch,
						btDiscreteDynamicsWorld* dynamicsWorld, unsigned int dt, glm::vec3 forwardsVec);

	private:
		Tank user;
		int previousMouse;
		unsigned int lastShot;
};

#endif