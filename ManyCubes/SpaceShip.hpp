# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# ifndef __SHAPE3D__
# include "Shape3D.hpp"
# define __SHAPE3D__
# endif

# ifndef __DEFINES__
# include "Defines.hpp"
# define __DEFINES__
# endif

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
		case THRUST_FOREWARD:
			rotationMatrix = glm::rotate(rotationMatrix, zRadians, zRotationAxis);
			break;
		}
		
	}
};