/*
 * Mike Landers
 * Ross Fahan
 * Chris Eng
 * COMP465
 * Fall 2014
 */

/*class accepts a number of variables at time of creation to
define a camera and can return a given cameras view matrix*/

# ifndef __DEFINES__
# include "Defines.hpp"
# define __DEFINES__
# endif

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

class ShipCamera //: public Camera 
{

private:

	glm::mat4 viewMatrix;
	glm::vec3 eyeOffset, atOffset, upOffset;
	Planet * camPlanet;
	SpaceShip * camShip;



public:

	ShipCamera(glm::vec3 eyeOffsetIn, glm::vec3 atOffsetIn, glm::vec3 upOffsetIn) {

	}


	glm::mat4 getViewMatrix(glm::mat4 subPos){
		
		glm::mat4 temp;
		glm::vec3 eye, at, up;
		float camBack = 40.0;
		float camUp = 20.0;
		
		
		up = getUp(subPos);

		temp = glm::translate(glm::mat4(), getOut(subPos) * camBack);
		temp = glm::translate(temp, getUp(subPos) * camUp);
		temp = glm::translate(temp, getPosition(subPos));
		eye = getPosition(temp);

		at = getPosition(subPos) + up * 10.0f;

		return glm::lookAt(eye, at, up);
	}


	void update() {
		;
	}
};