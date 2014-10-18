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

class Planet{

private:

	int id;
	glm::mat4 rotationMatrix; 
	glm::mat4 scaleMatrix; //size
	glm::mat4 translationMatrix;//position (x,y,z)
	glm::mat4 translate; //where to move to
	glm::vec3 rotationAxis = glm::vec3(0, 1, 0);; //spin axis
	glm::vec3 orbitAxis; //may be unnessesary
	float radians; //spin rate
	bool orbital, isTheSun;
	Planet * orbitTarget;

public:
	//constructor for the sun
	Planet(glm::vec3 scale, float rads, glm::vec3 trans, bool orbit){
		scaleMatrix = glm::scale(glm::mat4(), scale);
		radians = glm::radians(rads);
		translationMatrix = glm::translate(glm::mat4(),
			trans);
		orbital = orbit;
		isTheSun = true;
	}
	//constructor for the planets and moons
	Planet(glm::vec3 scale, float rads, glm::vec3 trans, bool orbit, Planet * orbitTargetIn){
		scaleMatrix = glm::scale(glm::mat4(), scale);
		radians = glm::radians(rads);
		translationMatrix = glm::translate(glm::mat4(),
			trans);
		orbital = orbit;
		orbitTarget = orbitTargetIn;
		isTheSun = false;
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
		//printf("Rotation");
		//for (int c = 0; c < 4; c++){
			//for (int j = 0; j < 4; j++){
				//printf("%f  ", rotationMatrix[c][j]);
			//}
			//printf("\n");
		//printf("\n");
		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};