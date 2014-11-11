# ifndef __SHAPE3D__
# include "Shape3D.hpp"
# define __SHAPE3D__
# endif

class SpaceShip : public Shape3D{

private:

	float xRadians = 0.02f;
	float yRadians = 0.02f;
	float zRadians = 0.02f;
	float thrust = 1.0f;
	glm::mat4 thrustTranslate = glm::mat4();

public:

	SpaceShip(glm::vec3 scale) {

		rotationMatrix = glm::mat4();
		scaleMatrix = glm::scale(glm::mat4(), scale);
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-7250, 0, 0));
	}



	glm::mat4 getModelMatrix() {

		translationMatrix = translationMatrix * thrustTranslate;
		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		thrustTranslate = glm::mat4();
		return(modelMatrix);
	}

	void update() {
		
	}

	void printMat4(glm::mat4 matIn){

		int i = 0;
		int j = 0;

		printf("\n");
		for (; i < 4; i++){
			for (j = 0; j < 4; j++)
				printf("%d   ", matIn[j][i]);
			printf("\n");
		}
		printf("\n");
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
			thrustTranslate = glm::translate(glm::mat4(), getIn(modelMatrix) * thrust);
			break;
		case THRUST_BACKWARD:
			thrustTranslate = glm::translate(glm::mat4(), getOut(modelMatrix) * thrust);
			break;
		}

		//printMat4(rotationMatrix);
		
	}
};