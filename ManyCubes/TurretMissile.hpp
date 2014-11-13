# ifndef __DEFINES__
# include "Defines.hpp"
# define __DEFINES__
# endif

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

# ifndef __SHAPE3D__
# include "Shape3D.hpp"
# define __SHAPE3D__
# endif

# ifndef __SHIP__
# include "SpaceShip.hpp"
# define __SHIP__
# endif

# ifndef __PLANET__
# include "Planet.hpp"
# define __PLANET__
# endif

class TurretMissile : public Shape3D {

private:

	SpaceShip * followTarget;
	Planet * firingPlatform;
	float thrust = 5;

public:

	//constructor for a turret
	TurretMissile(glm::vec3 scale, Shape3D * ship) {

		rotationMatrix = glm::mat4();  // no initial orientation
		scaleMatrix = glm::scale(glm::mat4(), scale);
		translationMatrix = glm::mat4();

		followTarget = (SpaceShip *)ship;
	}

	glm::mat4 getTransMatrix() {
		return translationMatrix;
	}

	glm::mat4 getModelMatrix() {
		//final calculation of modelMatrix done here
		return rotationMatrix * translationMatrix * scaleMatrix;
	}

	void fireMissile(Shape3D * platform){
		firingPlatform = (Planet *)platform;
	}

	void update() {
		//missile movement and orientation code here, ie update the orientation and translation matrixes here
	}

};