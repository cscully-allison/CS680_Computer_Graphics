#ifndef TANKAI_H
#define TANKAI_H

#include <object.h>
#include "graphics_headers.h"

struct Tank{
		Object *base;
		//Object *head;
		Object *placeholder;
		Object *projectile;

		int lives;
		int direction;
		int timeLeft;
		int initialTime;
		int compassPosition;
		bool attack;
};

class TankAI{
	public:
		TankAI(btDiscreteDynamicsWorld* dynamicsWorld);
		~TankAI();
		void Initialize (btDiscreteDynamicsWorld* dynamicsWorld, Tank& AI, btVector3 startOrigin, int index);
		void RenderWrapper(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos);
		void Render(Tank AI, GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos);
		void AddHealth(Tank AI);
		int LookForOpponent(Tank AI, glm::vec4, int position);
		float EuclidenDistance(glm::vec4, glm::vec4);
		void Attack(Tank AI, btDiscreteDynamicsWorld* dynamicsWorld);
		void LaunchProjectile(Tank AI, btDiscreteDynamicsWorld* dynamicsWorld);
		Tank GetTank(int number);
		Object* GetAIBase(int number);
		void Hit(btDiscreteDynamicsWorld* dynamicsWorld, int tankNumber);
		//Object* GetAIHead(int number);
		void SetOrientation(Tank& AI);
		void UpdateWrapper(unsigned int dt, glm::vec4, btDiscreteDynamicsWorld* dynamicsWorld);
		void Update(unsigned int dt, Tank& AI, glm::vec4, int position, btDiscreteDynamicsWorld* dynamicsWorld);

	private:
		Tank one;
		Tank two;
		Tank three;
		Tank four;
		Tank five;
};

#endif