/*
 * Mike Landers
 * Ross Fahan
 * Chris Eng
 * COMP465
 * Fall 2014
 */

# ifndef __SHAPE3D__
# include "Shape3D.hpp"
# define __SHAPE3D__
# endif

# ifndef __DEFINES__
# include "Defines.hpp"
# define __DEFINES__
# endif

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# ifndef __PLANET__
# include "Planet.hpp"
# define __PLANET__
# endif

class SpaceShip : public Shape3D{

private:

	float xRadians = 0.02f;
	float yRadians = 0.02f;
	float zRadians = 0.02f;
	float thrust = 10.0f;
	glm::mat4 thrustTranslate = glm::mat4();
	Planet * ruber;

public:

	SpaceShip(glm::vec3 scale, Shape3D * sun) {
		ruber = (Planet *)sun;
		rotationMatrix = glm::mat4();
		scaleMatrix = glm::scale(glm::mat4(), scale);
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-7250, 0, 0));
	}



	glm::mat4 getModelMatrix() {

		modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		return(modelMatrix);
	}

	void update() {
		translationMatrix = translationMatrix * thrustTranslate;
		thrustTranslate = glm::mat4();
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
		float dist;
		float gravForce;
		glm::vec3 gravVec;

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
			dist = distance(getPosition(ruber->getTransMatrix()), getPosition(translationMatrix));
			gravForce = 90000000.0f / (dist*dist);
			gravVec = glm::normalize(getPosition(translationMatrix)) * -gravForce;
			//printf("\n%f", gravForce);
			thrustTranslate = glm::translate(glm::mat4(), glm::normalize(getIn(modelMatrix)) * thrust + gravVec);
			break;
		case THRUST_BACKWARD:
			dist = distance(getPosition(ruber->getTransMatrix()), getPosition(translationMatrix));
			gravForce = 90000000.0f / (dist*dist);
			gravVec = glm::normalize(getPosition(translationMatrix)) * -gravForce;
			//printf("\n%f", gravForce);
			thrustTranslate = glm::translate(glm::mat4(), glm::normalize(getOut(modelMatrix)) * thrust + gravVec);
			break;
		}
	}

	void warp(int warpLoc, glm::vec3 unumEye, glm::vec3 duoEye){
		switch (warpLoc){
		case 0:
			translationMatrix = glm::translate(glm::mat4(), unumEye);
			break;
		case 1:
			translationMatrix = glm::translate(glm::mat4(), duoEye);
			break;
		}
	}
};