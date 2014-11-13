# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# ifndef __DEFINES__
# include "Defines.hpp"
# define __DEFINES__
# endif

# ifndef __SHAPE3D__
# include "Shape3D.hpp"
# define __SHAPE3D__
# endif

# ifndef __PLANET__
# include "Planet.hpp"
# define __PLANET__
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
		//glm::mat4 liftTransMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, lift, 0.0f));

		// make the turret follow the planet
		glm::vec3 transVec = followTarget->getTransVec();
		glm::mat4 followTransMatrix = glm::translate(glm::mat4(1.0f), transVec + glm::vec3(0.0f, lift, 0.0f));

		// move the turret to the proper spot
		translationMatrix = followTransMatrix; //* liftTransMatrix;

	}

};