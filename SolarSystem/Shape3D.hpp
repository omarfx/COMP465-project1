/*
 * Mike Landers
 * Ross Fahan
 * Chris Eng
 * COMP465
 * Fall 2014
 */

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# ifndef __DEFINES__
# include "Defines.hpp"
# define __DEFINES__
# endif

class Shape3D{

protected:
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix; //size
	glm::mat4 translationMatrix;//position (x,y,z)
	glm::mat4 translate; //where to move to
	glm::mat4 modelMatrix; //hold the value of a models matrix so it does not have to recalculated later
	
	//Axis' of rotation
	glm::vec3 xRotationAxis = glm::vec3(1, 0, 0);
	glm::vec3 yRotationAxis = glm::vec3(0, 1, 0);
	glm::vec3 zRotationAxis = glm::vec3(0, 0, 1);


public:
	
	virtual glm::mat4 getTransMatrix() {
		return translationMatrix;
	}

	virtual glm::vec3 getTransVec(){
		 glm::vec3 tempVec;
		 return tempVec;
	}

	virtual glm::mat4 getModelMatrix() {
		return modelMatrix;
	}

	virtual void update() {
	}

	virtual void move(int movement){
	}

	virtual void fireMissile(float speed, Shape3D * target){
	}

	virtual void warp(int warpLoc, glm::vec3 unumEye, glm::vec3 duoEye){
	}
	
};