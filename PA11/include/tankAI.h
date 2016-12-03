#ifndef TANKAI_H
#define TANKAI_H

#include <object.h>
#include "graphics_headers.h"

struct Tank{
	Object *base;
	Object *head;
};

class TankAI{
	public:
		TankAI();
		~TankAI();
		void Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height);
		Object* GetAIBase();
		Object* GetAIHead();

	private:
		Tank AI;
};

#endif