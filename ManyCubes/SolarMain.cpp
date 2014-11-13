/*
 * Mike Landers
 * Ross Fahan
 * Chris Eng
 * COMP465
 * Fall 2014
 */

# define __Windows__ // define your target operating system

# ifndef __OURINCLUDES__
# include "OurIncludes.hpp"
# define __OURINCLUDES__
# endif

//Cameras
Camera * topView;
Camera * unumView;
Camera * frontView;
Camera * duoView;
ShipCamera * shipCam;
// Shapes
const int nModels = 10;
Shape3D * model[nModels]; // objects for shapes
char * modelFile[nModels] = { "planet.tri", "planet.tri", "planet.tri", "planet.tri", "planet.tri", "ship.tri", "turret.tri", "turret.tri", "missile.tri", "missile.tri"}; // name of planet model file
const int nVertices[nModels] = {480 * 3, 480 * 3, 480 * 3, 480 * 3, 480 * 3, 515 * 3, 492 * 3, 492 * 3, 384 * 3, 384 * 3};
float modelBR[nModels]; // modelFile's bounding radius

/* current camera view */
int curView = 0;

/* current Timer Quantum */
int timeQuantum = 0;

int warpLoc = 0;

// display state and "state strings" for title display
// window title strings
char baseStr[50] =    "Solar system: ";
char fpsStr[15], viewStr[15] =    " front view";
char tqStr[15] = " Ace";
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

float modelSize[nModels] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 100.0f, 300.0f, 600.0f, 25.0f, 25.0f};   // size of model

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
	model[Unum] = new Planet(glm::vec3(scale[Unum]), 0.004f, glm::vec3(4000, 0, 0), "Unum", 2.0);
	model[Duo] = new Planet(glm::vec3(scale[Duo]), 0.002f, glm::vec3(-9000, 0, 0), "Duo", 2.0);
	model[Primus] = new Planet(glm::vec3(scale[Primus]), 0.004f, glm::vec3(-8100, 0, 0), model[Duo], "Primus", 2.0);
	model[Secundus] = new Planet(glm::vec3(scale[Secundus]), 0.002f, glm::vec3(-7250, 0, 0), model[Duo], "Sucundus", 2.0);
	printf("%d Planets created \n", (5));
	
	//create space ship
	model[Ship] = new SpaceShip(glm::vec3(scale[Ship]), model[Ruber]);
	printf("%d Ship created \n", 1);

	// create turrets
	model[TurretUnum] = new Turret(glm::vec3(scale[TurretUnum]), glm::vec3(4000, 0, 0), model[Unum], 150);
	model[TurretDuo] = new Turret(glm::vec3(scale[TurretDuo]), glm::vec3(-7250, 0, 0), model[Duo], 300);
	printf("%d Turrets created \n", 2);

	//create missiles
	model[MissileShip] = new ShipMissile(scale[MissileShip], model[Ship]);
	model[MissileTurret] = new TurretMissile(scale[MissileTurret], model[Ship]);

	//create cameras
	frontView = new Camera(glm::vec3(0.0f, 10000.0f, 20000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Front
	topView = new Camera(glm::vec3(0.0f, 20000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Top
	unumView = new Camera(glm::vec3(0.0f, 4000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //Unum
	duoView = new Camera(glm::vec3(0.0f, 4000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));  //Duo
	shipCam = new ShipCamera(glm::vec3(0.0f, 300.0f, 1000.0f), glm::vec3(0.0f, 200.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
	strcat(titleStr, tqStr);
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
	else if (curView == 4) {
		viewMatrix = shipCam->getViewMatrix(modelMatrix[Ship]);
		strcpy(viewStr, " Ship View");
	}
}

void printMat4(glm::mat4 matIn){ 
	          
	int i = 0;
	int j = 0;
	
	printf("\n");
	for (; i < 4; i++){
		for (j = 0; j < 4; j++)
			printf("%d   ", matIn[j][i]);
		printf("\n");
	}
	printf("\n");
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update model matrix, set MVP, draw

	for (int i = 0; i < nModels; i++) {
		modelMatrix[i] = model[i]->getModelMatrix();
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
		strcpy(tqStr, " Ace");
		break;
	case (1) :
		timerDelay = 100;
		strcpy(tqStr, " Novice");
		break;
	case (2) :
		timerDelay = 250;
		strcpy(tqStr, " Practice");
		break;
	case (3) :
		timerDelay = 500;
		strcpy(tqStr, " Debug");
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
		warpLoc = (warpLoc + 1) % 2;
		model[Ship]->warp(warpLoc, unumView->getEye(modelMatrix[Unum]), duoView->getEye(modelMatrix[Duo]));
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
		curView = (curView + 1) % 5;
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
			model[Ship]->move(ROLL_LEFT);
		}
		else
			model[Ship]->move(YAW_LEFT);
		break;
		/* -- move forward -- */
	case GLUT_KEY_UP:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			model[Ship]->move(PITCH_FORWARD);
		else
			model[Ship]->move(THRUST_FOREWARD);
		break;
		/* -- move right -- */
	case GLUT_KEY_RIGHT:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
			model[Ship]->move(ROLL_RIGHT);
		else
			model[Ship]->move(YAW_RIGHT);
		break;
		/* -- move down -- */
	case GLUT_KEY_DOWN:
		if (glutGetModifiers() == GLUT_ACTIVE_CTRL) 
			model[Ship]->move(PITCH_BACKWARD);
		else 
			model[Ship]->move(THRUST_BACKWARD);
		break;
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