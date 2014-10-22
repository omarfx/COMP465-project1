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
	glm::vec3 translation; //used to move the point of rotation to the origin and back
	float radians; //spin rate
	bool orbital, isTheSun;
	bool isAMoon;
	Planet * orbitTarget;
	char * planetName;
	glm::mat4 tempMatrix;

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
		radians = glm::radians(rads);
		translationMatrix = glm::translate(glm::mat4(), trans);
		isTheSun = false;
		isAMoon = false;
		planetName = name;
	}
	//constructor for the moons
	Planet(glm::vec3 scale, float rads, glm::vec3 trans, Planet * orbitTargetIn, char* name, float spin){
		orbitTarget = orbitTargetIn;
		rotationMatrix = glm::mat4();  // no initial orientation
		scaleMatrix = glm::scale(glm::mat4(), scale);
		radians = glm::radians(rads);
		
		translationMatrix = glm::translate(glm::mat4(), trans);
		tempMatrix = orbitTarget->getTransMatrix();
		for (int i = 0; i < 3; i++){
			translationMatrix[3][i] = translationMatrix[3][i] - tempMatrix[3][i];
		}

		isTheSun = false;
		isAMoon = true;
		planetName = name;
	}

	glm::mat4 getTransMatrix() {
		return translationMatrix;
	}

	glm::vec3 getTransVec(){
		glm::mat4 tempMat = getModelMatrix();
		glm::vec3 tempVec;

		tempVec[0] = tempMat[3][0];
		tempVec[1] = tempMat[3][1];
		tempVec[2] = tempMat[3][2];

		return tempVec;
	}

	glm::mat4 getModelMatrix() {

		if (!isAMoon)
			return(rotationMatrix * translationMatrix * scaleMatrix);
		else if (isAMoon){ // orbital 
			glm::vec3 trans = orbitTarget->getTransVec();
			
			tempMatrix = glm::translate(glm::mat4(), trans);
			return(tempMatrix * rotationMatrix * translationMatrix * scaleMatrix);
		}
		else
			return(translationMatrix * rotationMatrix * scaleMatrix);
	}

	void update() {
		
		rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		

		//printf("%s Rotation \n", planetName);
		//for (int c = 0; c < 4; c++){
			//for (int j = 0; j < 4; j++){
				//printf("%f  ", rotationMatrix[j][c]);
			//}
			//printf("\n");
		//}
		//printf("\n");
	}
};