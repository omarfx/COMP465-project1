/*
Shape3D.cpp

Represent the scale, translation, and rotation of a 3D shape.
If you overload the constructor you can create a shape with
arguments for scale, translation, and rotation.

Mike Barnes
8/24/2014
*/

# ifndef __SHAPE3D__
# include "Shape3D.hpp"
# define __SHAPE3D__
# endif


class Turret : public Shape3D {

public:

	//constructor for a turret
	Turret(glm::vec3 scale, glm::vec3 trans, char* name){
		rotationMatrix = glm::mat4();  // no initial orientation
		scaleMatrix = glm::scale(glm::mat4(), scale);
		translationMatrix = glm::translate(glm::mat4(), trans);
	}

	glm::mat4 getTransMatrix() {
		return translationMatrix;
	}

	glm::mat4 getModelMatrix() {
		return rotationMatrix * translationMatrix * scaleMatrix;
	}

	void update() {

	}

};