# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

//# include "Shape3D.hpp" //Why does this stop my program from building

class SpaceShip : public Shape3D{

private:

	float radians; //spin rate

public:

	SpaceShip(glm::vec3 scale) {

		//rotationMatrix = glm::mat4();
		
		scaleMatrix = glm::scale(glm::mat4(), scale);
		//radians = glm::radians(1.0f);
		translationMatrix = glm::translate(glm::mat4(), glm::vec3(-7250, 0, 0));
	}



	glm::mat4 getModelMatrix() {
			return(translationMatrix * scaleMatrix);
	}

	void update() {
		//rotationMatrix = glm::rotate(rotationMatrix, radians, rotationAxis);
		//translationMatrix = glm::translate(translationMatrix, translation);
	}
};