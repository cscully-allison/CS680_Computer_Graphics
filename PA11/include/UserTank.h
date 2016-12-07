#ifndef USERTANK_H
#define USERTANK_H

#include <object.h>
#include "graphics_headers.h"
#include "tankAI.h"

// struct PlayerTank{
// 		Object *base;
// 		Object *head;

// 		int lives;
// 		int ammo;
// 		int direction;
// 		int compassPosition;
// };

class UserTank{
	public:
		UserTank();
		~UserTank();
		void Render(GLint modelMatrix, Uniform scalar, Uniform spec, Uniform spot, Uniform height);
		void AddHealth();
		Object* GetBase();
		Object* GetHead();
		Object* GetPlaceholder();
		void SetOrientation();
		void Update(std::vector <unsigned int> keyPress, int mouseMovement);

	private:
		Tank user;
		int previousMouse;
};

#endif