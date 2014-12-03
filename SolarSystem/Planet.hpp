/*
 * Mike Landers
 * Ross Fahan
 * Chris Eng
 * COMP465
 * Fall 2014
 */

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

# ifndef __SHAPE3D__
# include "Shape3D.hpp"
# define __SHAPE3D__
# endif

class Planet : public Shape3D{

private:

	float radians; //orbit rate
	bool isTheSun, isAMoon;
	Planet * orbitTarget; //the planet a moon orbits
	char * planetName; //for debugging

public:
	//constructor for the sun
	Planet(glm::vec3 scale, glm::vec3 trans, char* name, float spin){
		rotationMatrix = glm::mat4();  // no initial orientation
		scaleMatrix = glm::scale(glm::mat4(), scale);
		radians = 0.0;
		translationMatrix = glm::translate(glm::mat4(), trans);
		isTheSun = true;
		planetName = name;
	}
	//constructor for the planets
	Planet(glm::vec3 scale, float rads, glm::vec3 trans, char* name, float spin){
		rotationMatrix = glm::mat4();  // no initial orientation
		scaleMatrix = glm::scale(glm::mat4(), scale);
		//radians = glm::radians(rads);
		radians = rads;
		translationMatrix = glm::translate(glm::mat4(), trans);
		isTheSun = false;
		isAMoon = false;
		planetName = name;
	}
	//constructor for the moons
	Planet(glm::vec3 scale, float rads, glm::vec3 trans, Shape3D * orbitTargetIn, char* name, float spin){
		orbitTarget = (Planet *) orbitTargetIn;
		glm::mat4 pTransMatrix;
		rotationMatrix = glm::mat4();  // no initial orientation
		scaleMatrix = glm::scale(glm::mat4(), scale);
		//radians = glm::radians(rads);
		radians = rads;

		translationMatrix = glm::translate(glm::mat4(), trans);
		pTransMatrix = orbitTarget->getTransMatrix();
		for (int i = 0; i < 3; i++){
			translationMatrix[3][i] = translationMatrix[3][i] - pTransMatrix[3][i];
		}

		isTheSun = false;
		isAMoon = true;
		planetName = name;
	}

	glm::mat4 getTransMatrix() {
		return translationMatrix;
	}

	glm::vec3 getTransVec(){
		glm::vec3 tempVec;

		tempVec[0] = modelMatrix[3][0];
		tempVec[1] = modelMatrix[3][1];
		tempVec[2] = modelMatrix[3][2];

		return tempVec;
	}

	glm::mat4 getModelMatrix() {

		if (isTheSun)
			return(translationMatrix * rotationMatrix * scaleMatrix);
		else if (!isAMoon){
			modelMatrix = rotationMatrix * translationMatrix * scaleMatrix;
			return modelMatrix;
		}
		else{
			glm::mat4  pTransMatrix;// = glm::translate();
			glm::vec3 trans = orbitTarget->getTransVec();
			
			pTransMatrix = glm::translate(glm::mat4(), trans);
			return(pTransMatrix * rotationMatrix * translationMatrix * scaleMatrix);
		}
	}

	void update() {
		
		rotationMatrix = glm::rotate(rotationMatrix, radians, yRotationAxis);
	}
};