/*class accepts a number of variables at time of creation to
  define a camera and can return a given cameras view matrix*/

# ifndef __INCLUDES465__
# include "../includes465/include465.hpp"
# define __INCLUDES465__
# endif

class Camera {

private:

	glm::mat4 viewMatrix;
	glm::vec3 eye, at, up;
	glm::mat4 tempTransMatrix;
	
	enum CamMode {Ship, Top, Front, Planet};

public:

	Camera(glm::vec3 eyeOffset, glm::vec3 atOffset, glm::vec3 upOffset, Shape3D * camSubject, CamMode mode) {

		if (mode == Planet){
			camSubject->getModelMatrix();
		}
		eye = eyeIn;
		at = atIn;
		up = upIn;
	

		viewMatrix = glm::lookAt(eye, at, up);
	}

	glm::mat4 getViewMatrix() {
		return viewMatrix;
	}


	void update() {
		
	}
};