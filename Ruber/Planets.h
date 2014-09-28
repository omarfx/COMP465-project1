/*
Shape3D.cpp

Represent the scale, translation, and rotation of a 3D shape.
If you overload the constructor you can create a shape with
arguments for scale, translation, and rotation.

Mike Barnes
8/24/2014
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Planets {

private:

	int id;
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;
	glm::vec3 rotationAxis;
	float radians;
	bool orbital;

public:

	Planet(int number) {
		id = number;  // for debugging
		
		rotationMatrix = glm::mat4();

		//I believe radians var and translation matrix are set corectly but scale and rotation are place holder for now
		switch (number) { // make model larger
		case 0: //Ruber
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(20, 20, 20));
			rotationAxis = glm::vec3(1, 0, 0);
			translationMatrix = glm::translate(glm::mat4(),
			glm::vec3(0, 0, 0));
			break; 
		case 1: //Unum
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(20, 30, 40)); //size
			rotationAxis = glm::vec3(0, 1, 0); //orbit
			radians = glm::radians(.004f); //spin
			translationMatrix = glm::translate(glm::mat4(), //part of the next line down
			glm::vec3(4000, 0, 0)); //position (x,y,z)
			break;
		case 2: //Duo
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(30, 40, 20));
			rotationAxis = glm::vec3(0, 0, -1);
			radians = glm::radians(.002f);
			translationMatrix = glm::translate(glm::mat4(),
				glm::vec3(-9000, 0, 0));
			break;
		case 3: //Primus
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(40, 20, 30));
			rotationAxis = glm::vec3(0, 0, -1);
			radians = glm::radians(.004);
			translationMatrix = glm::translate(glm::mat4(),
				glm::vec3(-8100, 0, 0));
			break;
		case 4: //Secundus
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(40, 20, 30));
			rotationAxis = glm::vec3(0, 0, -1);
			radians = glm::radians(.002f);
			translationMatrix = glm::translate(glm::mat4(),
				glm::vec3(-7250, 0, 0));
			break;
		default: printf("Planet:: selection error\n");
			exit(-1); break;
		}

		// determine rotation type
		if (random % 2 == 0)
			orbital = true;
		else
			orbital = false;
	}

	glm::mat4 getModelMatrix() {
		if (orbital) // orbital rotation
			return(rotationMatrix * translationMatrix * scaleMatrix);
		else  // center rotation
			return(translationMatrix * rotationMatrix * scaleMatrix);
	}

	void update() {
		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};