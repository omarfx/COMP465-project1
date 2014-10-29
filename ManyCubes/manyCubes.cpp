
# define __Windows__ // define your target operating system
# define Ruber 0
# define Unum 1
# define Duo 2
# define Primus 3
# define Secundus 4
# define Ship 5
# define Missle 6

# include "../includes465/include465.hpp"  

# include "Planet.hpp"
# include "SpaceShip.hpp"
# include "Camera.hpp"
//# include "Shape3D.hpp"

//Cameras
Camera * topView;
Camera * unumView;
Camera * frontView;
Camera * duoView;
// Shapes
const int nModels = 6;
Shape3D * model[nModels];
// Models for shapes
char * modelFile[nModels] = { "planet.tri", "planet.tri", "planet.tri", "planet.tri", "planet.tri", "ship.tri"}; // name of planet model file
const int nVertices[nModels] = { 480 * 3, 480 * 3, 480 * 3, 480 * 3, 480 * 3, 515 * 3};
float boundingRadius;  // modelFile's bounding radius
float modelBR[nModels];
float shipBoundingRadius;
int Index =  0;  // global variable indexing into VBO arrays

/* current camera view */
int curView = 0;

/* current Timer Quantum */
int timeQuantum = 0;

// display state and "state strings" for title display
// window title strings
char baseStr[50] =    "Solar system: ";
char fpsStr[15], viewStr[15] =    " front view";
char titleStr [100]; 

GLuint VAO[nModels];      // Vertex Array Objects
GLuint buffer[nModels];
GLuint shaderProgram; 
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint MVP ;  // Model View Projection matrix's handle
glm::mat4 projectionMatrix;		// set in reshape()
glm::mat4 modelMatrix[nModels];		// set in display
glm::mat4 viewMatrix;			// set in keyboard()
glm::mat4 ModelViewProjectionMatrix; // set in display();

glm::vec3 scale[nModels];       // set in init()
float modelSize[nModels] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 100.0f};   // size of model
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models

// rotation variables
glm::mat4 identity(1.0f); 
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;  // A delay of 40 milliseconds is 25 updates / second
double currentTime, lastTime, timeInterval; 


void init(void) {
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	// generate VAOs and VBOs
	glGenVertexArrays(nModels, VAO);
	glGenBuffers(nModels, buffer);
	// load the buffers from the model files
	for (int i = 0; i < nModels; i++) {
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] * 1.0f / modelBR[i]);
	}

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");


	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	printf("Cameras created \n");
	// create solar system
	model[Ruber] = new Planet(glm::vec3(scale[Ruber]), glm::vec3(0, 0, 0), "Ruber", 4.0);
	model[Unum] = new Planet(glm::vec3(scale[Unum]), 1.0f, glm::vec3(4000, 0, 0), "Unum", 2.0);
	model[Duo] = new Planet(glm::vec3(scale[Duo]), 0.5f, glm::vec3(-9000, 0, 0), "Duo", 2.0);
	model[Primus] = new Planet(glm::vec3(scale[Primus]), 1.0f, glm::vec3(-8100, 0, 0), model[Duo], "Primus", 2.0);
	model[Secundus] = new Planet(glm::vec3(scale[Secundus]), 0.5f, glm::vec3(-7250, 0, 0), model[Duo], "Sucundus", 2.0);

	printf("%d Planets created \n", (nModels - 1));
	//create space ship
	model[Ship] = new SpaceShip(glm::vec3(scale[Ship]));

	printf("%d Ship created \n", 1);
	//create cameras
	frontView = new Camera(glm::vec3(0.0f, 10000.0f, 20000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), model[Ruber]);
	topView = new Camera(glm::vec3(0.0f, 20000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), model[Ruber]);
	unumView = new Camera(glm::vec3(0.0f, 4000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), model[Unum]);
	duoView = new Camera(glm::vec3(0.0f, 4000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), model[Duo]);

	lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat) width /  (GLfloat) height, 1.0f, 100000.0f); 
  }

// update and display animation state in window title
void updateTitle() {
	strcpy(titleStr, baseStr);
	strcat(titleStr, viewStr);
	strcat(titleStr, fpsStr);
	// printf("title string = %s \n", titleStr);
	glutSetWindowTitle(titleStr);
}

void camUpdate(void){

	//on each pass of display the viewmatrix is set correctly based on which camera we are set to
	if (curView == 0){
		viewMatrix = frontView->getViewMatrix(modelMatrix[Ruber]);
		strcpy(viewStr, " Front View");
	}
	else if (curView == 1){
		viewMatrix = topView->getViewMatrix(modelMatrix[Ruber]);
		strcpy(viewStr, " Top View");
	}
	else if (curView == 2){
		viewMatrix = unumView->getViewMatrix(modelMatrix[Unum]);
		strcpy(viewStr, " Unum View");
	}
	else if (curView == 3) {
		viewMatrix = duoView->getViewMatrix(modelMatrix[Duo]);
		strcpy(viewStr, " Duo View");
	}
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// update model matrix, set MVP, draw
	for (int i = 0; i < nModels; i++) {
		modelMatrix[i] =  model[i]->getModelMatrix();
	}
	camUpdate();
	for (int i = 0; i < nModels; i++) {
		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix[i]; 
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix)); 
		
		glBindVertexArray(VAO[i]);

		glDrawArrays(GL_TRIANGLES, 0, nVertices[i]);
    }

	glutSwapBuffers();

	frameCount++;
	// see if a second has passed to set estimated fps information
	currentTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
	timeInterval = currentTime - lastTime;
	if ( timeInterval >= 1000) {
		sprintf(fpsStr, " fps %4d", (int) (frameCount / (timeInterval / 1000.0f)) );
		lastTime = currentTime;
		frameCount = 0;
		updateTitle();
	}
}
void updateTimer(int timeQuantum) {
	switch (timeQuantum)
	{
	case (0) :
		timerDelay = 40;
		break;
	case (1) :
		timerDelay = 100;
		break;
	case (2) :
		timerDelay = 250;
		break;
	case (3) :
		timerDelay = 500;
		break;
	}
}

// Animate scene objects by updating their transformation matrices
// timerDelay = 40, or 25 updates / second
void update (int i) { 
	
  glutTimerFunc(timerDelay, update, 1);
  for(int i = 0; i < nModels; i++) model[i] -> update();  
  }

// Quit or set the view
// handle basic keyboard functions
void keyboard(unsigned char key, int x, int y) {
	
	switch (key) {
		/* -- quit -- */
	case 033: case 'q':  case 'Q':
		exit(EXIT_SUCCESS);
		break;
		/* -- warp ship -- */
	case 'w': case 'W':
		printf("Warp ship!");
		break;
		/* -- fire missile -- */
	case 'f': case 'F':
		printf("Fire missile!");
		break;
		/* -- fire missile -- */
	case 'g': case 'G':
		printf("Toggle gravity!");
		break;
		/* -- camera view -- */
	case 'v': case 'V':
		/* -- evaluate view -- */
		curView = (curView + 1) % 4;
		break;
		/* -- next tq value -- */
	case 't': case 'T':
		timeQuantum = (timeQuantum + 1) % 4;
		updateTimer(timeQuantum);
		printf("Next TQ value!");
		break;
		/* -- next ship speed -- */
	case 's': case 'S':
		printf("Change ship speed!");
		break;
	}
	/* update title on page base on view */
	updateTitle();
}

// handle special keyboard functions
void specialKeyboard(int key, int x, int y) {
	
	switch (key) {
		/* -- move left -- */
	case GLUT_KEY_LEFT:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			printf("Roll left!");
		}
		else {
			printf("Move left!");
		} break;
		/* -- move forward -- */
	case GLUT_KEY_UP:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			printf("Pitch forward!");
		}
		else {
			printf("Move forward!");
		} break;
		/* -- move right -- */
	case GLUT_KEY_RIGHT:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			printf("Roll right!");
		}
		else {
			printf("Move right!");
		} break;
		/* -- move down -- */
	case GLUT_KEY_DOWN:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
			printf("Pitch backward!");
		}
		else {
			printf("Move backward!");
		} break;
	}
}
    
int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow("465 manyCubes Example {f, t, r} : front view");
  // initialize and verify glew
  glewExperimental = GL_TRUE;  // needed my home system 
  GLenum err = glewInit();  
  if (GLEW_OK != err) 
      printf("GLEW Error: %s \n", glewGetErrorString(err));      
    else {
      printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
      printf("OpenGL %s, GLSL %s\n", 
        glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION));
      }
  // initialize scene
  init();
  // set glut callback functions
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeyboard);
  glutTimerFunc(timerDelay, update, 1);
  glutIdleFunc(display);
  glutMainLoop();
  printf("done\n");
  return 0;
  }