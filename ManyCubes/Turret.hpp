/*
Shape3D.cpp

Represent the scale, translation, and rotation of a 3D shape.
If you overload the constructor you can create a shape with
arguments for scale, translation, and rotation.

Mike Barnes
8/24/2014
*/

# ifndef __SHAPE3D__
# include "Planet.hpp"
# include "Shape3D.hpp"
# define __SHAPE3D__
# endif


class Turret : public Shape3D {

private:

	Planet * followTarget; // the planet the turret is on
	float lift; // how high the turret is lifted off the planet surface

public:

	//constructor for a turret
	Turret(glm::vec3 scale, glm::vec3 trans, Shape3D * followTargetIn, float radius) {

		rotationMatrix = glm::mat4();  // no initial orientation
		scaleMatrix = glm::scale(glm::mat4(), scale);
		translationMatrix = glm::translate(glm::mat4(), trans);

		// load up the planet that's being followed
		followTarget = (Planet *) followTargetIn;

		// load up how high the turret is lifted off the planet surface
		lift = radius;
	}

	glm::mat4 getTransMatrix() {
		return translationMatrix;
	}

	glm::mat4 getModelMatrix() {
		return rotationMatrix * translationMatrix * scaleMatrix;
	}

	void update() {

		// lift the turret to the top of the surface
		glm::mat4 liftTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, lift, 0.0f));

		// make the turret follow the planet
		glm::vec3 transVec = followTarget->getTransVec();
		glm::mat4 followTransMatrix = glm::translate(glm::mat4(1.0f), transVec);

		// move the turret to the proper spot
		translationMatrix = followTransMatrix * liftTransMatrix;

	}

};