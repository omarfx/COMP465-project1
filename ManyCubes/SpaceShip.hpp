# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

#define YAW_RIGHT 0
#define YAW_LEFT 1
#define PITCH_FORWARD 2
#define PITCH_BACKWARD 3
#define ROLL_RIGHT 4
#define ROLL_LEFT 5

//# include "Shape3D.hpp" //Why does this stop my program from building

class SpaceShip : public Shape3D{

private:

	float xRadians = 0.02f;
	float yRadians = 0.02f;
	float zRadians = 0.02f;

public:

	SpaceShip(glm::vec3 scale) {

		//rotationMatrix = glm::mat4();
		
		scaleMatrix = glm::scale(glm::mat4(), scale);
		//radians = glm::radians(1.0f);
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-7250, 0, 0));
	}



	glm::mat4 getModelMatrix() {
			return(translationMatrix * rotationMatrix * scaleMatrix);
	}

	void update() {
		
	}

	void move(int movement){
		switch (movement){
		case YAW_LEFT:
			rotationMatrix = glm::rotate(rotationMatrix, yRadians, yRotationAxis);
			break;
		case YAW_RIGHT:
			rotationMatrix = glm::rotate(rotationMatrix, -yRadians, yRotationAxis);
			break;
		case PITCH_FORWARD:
			rotationMatrix = glm::rotate(rotationMatrix, -xRadians, xRotationAxis);
			break;
		case PITCH_BACKWARD:
			rotationMatrix = glm::rotate(rotationMatrix, xRadians, xRotationAxis);
			break;
		case ROLL_RIGHT:
			rotationMatrix = glm::rotate(rotationMatrix, -zRadians, zRotationAxis);
			break;
		case ROLL_LEFT:
			rotationMatrix = glm::rotate(rotationMatrix, zRadians, zRotationAxis);
			break;
		}
		
	}
};