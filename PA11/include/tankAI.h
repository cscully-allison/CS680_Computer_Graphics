#ifndef TANKAI_H
#define TANKAI_H

#include <object.h>
#include "graphics_headers.h"

struct Tank{
		Object *base;
		Object *head;

		int lives;
		int ammo;
		int direction;
		int timeLeft;
};

class TankAI{
	public:
		TankAI();
		~TankAI();
		void Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height);
		Object* GetAIBase();
		Object* GetAIHead();
		void SetOrientation();
		void Update(unsigned int dt);

	private:
		Tank AI;
};

#endif