# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Shape3D{

protected:
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix; //size
	glm::mat4 translationMatrix;//position (x,y,z)
	glm::mat4 translate; //where to move to
	glm::mat4 modelMatrix; //hold the value of a planets matrix so it does not have to recalculated later
	glm::vec3 rotationAxis = glm::vec3(0, 1, 0);; //orbit axis

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
	
};