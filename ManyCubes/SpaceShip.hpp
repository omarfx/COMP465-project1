# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class SpaceShip {

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

	SpaceShip(int number) {

		//rotationMatrix = glm::mat4();
		
		scaleMatrix = glm::scale(glm::mat4(), glm::vec3(100, 100, 100));
		//radians = glm::radians(1.0f);
		translationMatrix = glm::translate(glm::mat4(),
			glm::vec3(1500, 0, 0));
	}



	glm::mat4 getModelMatrix() {
			return(translationMatrix * scaleMatrix);
	}

	void update() {
		//rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};