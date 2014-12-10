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

class Starfield : public Shape3D{

private:

	bool isTexture;

public:
	//constructor for the starfield
	Starfield(glm::vec3 scale, glm::vec3 trans){
		rotationMatrix = glm::mat4();  // no initial orientation
		scaleMatrix = glm::scale(glm::mat4(), scale);
		translationMatrix = glm::translate(glm::mat4(), trans);
		isTexture = true;
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

			return(translationMatrix * rotationMatrix * scaleMatrix);
	}

	void update() {
		
	}
};