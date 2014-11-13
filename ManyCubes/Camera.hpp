/*class accepts a number of variables at time of creation to
  define a camera and can return a given cameras view matrix*/
# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# ifndef __PLANET__
# include "Planet.hpp"
# define __PLANET__
# endif

# ifndef __SHIP__
# include "SpaceShip.hpp"
# define __SHIP__
# endif

class Camera {

private:

	glm::mat4 viewMatrix;
	glm::vec3 eye, at, up, eyeOffset, atOffset, upOffset;
	glm::mat4 tempTransMatrix;
	Planet * camPlanet;
	SpaceShip * camShip;



public:

	Camera(glm::vec3 eyeOffsetIn, glm::vec3 atOffsetIn, glm::vec3 upOffsetIn) {

		
			eyeOffset = eyeOffsetIn;
			atOffset = atOffsetIn;
			upOffset = upOffsetIn;
	}

	glm::mat4 getViewMatrix(glm::mat4 subPos) {
		
		tempTransMatrix = subPos;
		eye = glm::vec3(tempTransMatrix[3][0] + eyeOffset[0], tempTransMatrix[3][1] + eyeOffset[1], tempTransMatrix[3][2] + eyeOffset[2]);
		at = glm::vec3(tempTransMatrix[3][0] + atOffset[0], tempTransMatrix[3][1] + atOffset[1], tempTransMatrix[3][2] + atOffset[2]);;
		up = upOffset;
		
		return glm::lookAt(eye, at, up);
	}


	void update() {
		;
	}
};