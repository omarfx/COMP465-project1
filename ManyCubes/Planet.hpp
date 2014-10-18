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

class Planet {

private:

	int id;
	glm::mat4 rotationMatrix; 
	glm::mat4 scaleMatrix; //size
	glm::mat4 translationMatrix;//position (x,y,z)
	glm::mat4 translate; //where to move to
	glm::vec3 rotationAxis = glm::vec3(0, 1, 0);; //spin axis
	glm::vec3 orbitAxis; //may be unnessesary
	float radians; //spin rate
	bool orbital;

public:

	Planet(int number) {
		
		rotationMatrix = glm::mat4();

		//I believe radians var and translation matrix are set corectly but scale and rotation are place holder for now
		switch (number) { // make model larger
		case 0: //Ruber
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(2000, 2000, 2000));
			radians = glm::radians(0.0f);
			translationMatrix = glm::translate(glm::mat4(),
				glm::vec3(0, 0, 0));
			orbital = false;
			break; 
		case 1: //Unum
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(200, 200, 200));
			radians = glm::radians(1.0f);
			translationMatrix = glm::translate(glm::mat4(),
				//glm::vec3(4000, 0, 0));
				glm::vec3(4000, 0, 0)); /* chris debugging */
			orbital = true;
			break;
		case 2: //Duo
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(400, 400, 400));
			radians = glm::radians(1.0f);
			translationMatrix = glm::translate(glm::mat4(),
				//glm::vec3(-9000, 0, 0));
				glm::vec3(-9000, 0, 0)); /* chris debugging */
			orbital = true;
			break;
		case 3: //Primus
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(100, 100, 100));
			radians = glm::radians(.004f);
			translationMatrix = glm::translate(glm::mat4(),
				glm::vec3(-8100, 0, 0));
			orbital = true;
			break;
		case 4: //Secundus
			scaleMatrix = glm::scale(glm::mat4(), glm::vec3(150, 150, 150));
			radians = glm::radians(.002f);
			translationMatrix = glm::translate(glm::mat4(),
				glm::vec3(-7250, 0, 0));
			orbital = true;
			break;
		default: printf("Planet:: selection error\n");
			exit(-1); break;
		}

		// determine rotation type
		//if (random % 2 == 0)
			//orbital = true;
		//else
			//orbital = false;
	}

	glm::mat4 getTransMatrix() {
		return translationMatrix;
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