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
};

class TankAI{
	public:
		TankAI(btDiscreteDynamicsWorld* dynamicsWorld);
		~TankAI();
		void Initialize (btDiscreteDynamicsWorld* dynamicsWorld, Tank& AI, btVector3 startOrigin, int index);
		void RenderWrapper(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos);
		void Render(Tank AI, GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height, Uniform eyePos);
		void AddHealth(Tank AI);
		Tank GetTank(int number);
		Object* GetAIBase(int number);
		//Object* GetAIHead(int number);
		void SetOrientation(Tank& AI);
		void UpdateWrapper(unsigned int dt);
		void Update(unsigned int dt, Tank& AI);

	private:
		Tank one;
		Tank two;
		Tank three;
		Tank four;
		Tank five;
};

#endif