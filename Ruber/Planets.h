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
		
		//I believe radians var is set corectly but scale and rotation are place holder for now
		switch (number) { // make model larger
		case 0: //Ruber
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(20, 20, 20));
			rotationAxis = glm::vec3(1, 0, 0);
			break; 
		case 1: //Unum
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(20, 30, 40));
			rotationAxis = glm::vec3(0, 1, 0);
			radians = glm::radians(.004f);
			break;
		case 2: //Duo
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(30, 40, 20));
			rotationAxis = glm::vec3(0, 0, -1);
			radians = glm::radians(.002f);
			break;
		case 3: //Primus
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(40, 20, 30));
			rotationAxis = glm::vec3(0, 0, -1);
			radians = glm::radians(.004);
			break;
		case 4: //Secundus
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(40, 20, 30));
			rotationAxis = glm::vec3(0, 0, -1);
			radians = glm::radians(.002f);
			break;


		default: printf("Shape:: randomRotate selection error\n");
			exit(-1); break;
		}
		rotationMatrix = glm::mat4();  // no initial orientation

		// random initial placement +/- 500 from origin in X, Y, Z
		translationMatrix = glm::translate(glm::mat4(),
			glm::vec3(-500 + rand() % 1000, -500 + rand() % 1000, -500 + rand() % 1000));
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