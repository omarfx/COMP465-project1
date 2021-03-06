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


//Textures
int width = 640, height = 487;   // set texture's width and height values here
char * fileName = "starField.raw";  // set the texture file's name here

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
const GLuint starVertices = 6;

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

GLuint VAO[nModels+1];      // Vertex Array Objects
GLuint buffer[nModels+1];
GLuint shaderProgram;
GLuint ibo;
GLuint vTexCoord;
GLuint texture, Texture, showTexture; // texture id and shader handle
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint MVP ;  // Model View Projection matrix's handle
GLuint MV;  //Model Views matrix handle
GLuint NM; //NormalMatrix's matrix handle
glm::mat4 projectionMatrix;		// set in reshape()
glm::mat4 modelMatrix[nModels];		// set in display
glm::mat4 viewMatrix;			// set in keyboard()
glm::mat4 ModelViewProjectionMatrix; // set in display();
glm::mat4 ModelView; //set in display()
glm::mat3 NormalMatrix;
glm::mat4 starRotation;
glm::mat4 starTranslate;


//debug Light vars
GLuint debugSetOn; //handle for debug bool
int debugLightOn = 0; //0 = off 1 = on

GLuint isTheSun;

//Point Light vars
GLuint pointLightSetOn; // handle for bool in shader
GLuint pointLightLoc; //handle
GLuint pointLightIntensity; //handle
int pointLightOn = 1; //0 = off 1 = on
glm::vec3 PointLightIntensity = glm::vec3(1.0, 1.0, 1.0);//RBG values of the light

//Ambient Light vars
GLuint ambientSetOn;
GLuint ambientLightIntensity;
int ambientOn = 0;
glm::vec3 AmbientLightIntensity = glm::vec3(0.5, 0.5, 0.5);//RBG values of the light

//Head Light vars
GLuint headLightSetOn; // handle for bool in shader
GLuint headLightLoc; //handle
GLuint headLightIntensity; //handle
int headLightOn = 0; //0 = off 1 = on
glm::vec3 HeadLightIntensity = glm::vec3(1.0, 1.0, 1.0);//RBG values of the light

glm::vec3 scale[nModels];       // set in init()

float modelSize[nModels] = { 2000.0f, 200.0f, 400.0f, 100.0f, 150.0f, 100.0f, 300.0f, 600.0f, 25.0f, 25.0f};   // size of model

GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models

// rotation variables
glm::mat4 identity(1.0f); 
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;  // A delay of 40 milliseconds is 25 updates / second
double currentTime, lastTime, timeInterval; 

static const GLfloat point[] = {
	-20000.0f,  20000.0f, -20000.0f, 1.0f, 
	-20000.0f, -20000.0f, -20000.0f, 1.0f,  
	 20000.0f, -20000.0f, -20000.0f, 1.0f,  
	 20000.0f,  20000.0f, -20000.0f, 1.0f,
	 20000.0f, -20000.0f, -20000.0f, 1.0f,
	 -20000.0f, 20000.0f, -20000.0f, 1.0f,
}; 

static const unsigned int indices[] = { // 12 triangles, 3 vertices per triangle
	3, 0, 1,
	3, 1, 2  
}; 

static const GLfloat texCoords[] = {
	0.0f, 0.0f,     // 0
	1.0f, 0.0f,     // 1
	1.0f, 1.0f,     // 2
	0.0f, 1.0f };

void init(void) {
	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	// generate VAOs and VBOs
	glGenVertexArrays(nModels+1, VAO);
	glGenBuffers(nModels+1, buffer);
	// load the buffers from the model files
	for (int i = 0; i < nModels; i++) {
		modelBR[i] = loadModelBuffer(modelFile[i], nVertices[i], VAO[i], buffer[i], shaderProgram,
			vPosition[i], vColor[i], vNormal[i], "vPosition", "vColor", "vNormal");
		// set scale for models given bounding radius  
		scale[i] = glm::vec3(modelSize[i] * 1.0f / modelBR[i]);
	}

	// set up the indices buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set up the indexed pyramid vertex attributes
	glBindVertexArray(buffer[Star]);

	//  initialize a buffer object
	glEnableVertexAttribArray(buffer[Star]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[Star]);

	//glBufferData( GL_ARRAY_BUFFER, sizeof(point) + sizeof(texCoords) + sizeof(normal), NULL, GL_STATIC_DRAW );
	glBufferData(GL_ARRAY_BUFFER, sizeof(point) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point), point);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(point), sizeof(texCoords), texCoords);

	// set up vertex arrays (after shaders are loaded)
	vPosition[Star] = glGetAttribLocation(shaderProgram, "vPosition");
	glVertexAttribPointer(vPosition[Star], 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition[Star]);

	vTexCoord = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point)));
	glEnableVertexAttribArray(vTexCoord);


	//Uniforms
	
	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
	MV = glGetUniformLocation(shaderProgram, "ModelView");
	NM = glGetUniformLocation(shaderProgram, "NormalMatrix");
	
	pointLightSetOn = glGetUniformLocation(shaderProgram, "PointLightOn");
	pointLightLoc = glGetUniformLocation(shaderProgram, "PointLightPosition");
	pointLightIntensity = glGetUniformLocation(shaderProgram, "PointLightIntensity");

	headLightSetOn = glGetUniformLocation(shaderProgram, "HeadLightOn");
	headLightLoc = glGetUniformLocation(shaderProgram, "HeadLightPosition");
	headLightIntensity = glGetUniformLocation(shaderProgram, "HeadLightIntensity");
	
	ambientLightIntensity = glGetUniformLocation(shaderProgram, "AmbientLightIntensity");
	ambientSetOn = glGetUniformLocation(shaderProgram, "AmbientOn");

	debugSetOn = glGetUniformLocation(shaderProgram, "DebugOn");
	isTheSun = glGetUniformLocation(shaderProgram, "IsTheSun");

	showTexture = glGetUniformLocation(shaderProgram, "IsTexture");

	// load texture
	texture = loadRawTexture(texture, fileName, width, height);
	if (texture != 0) {
		printf("texture file, read, texture generated and bound  \n");
		Texture = glGetUniformLocation(shaderProgram, "Texture");
	}
	else  // texture file loaded
		printf("Texture in file %s NOT LOADED !!! \n");

	// set render state values
	//glCullFace(GL_BACK);  // show only front faces
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
	glm::vec3 RuberPos;
	glm::vec3 ShipPos;
	int isSun;
	int isTexture;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update model matrix, set MVP, draw
	isTexture = 1;

	glBindVertexArray(VAO[Star]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[Star]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glUniform1i(showTexture, isTexture);

	ModelViewProjectionMatrix = projectionMatrix * viewMatrix;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), 1.57f, glm::vec3(0.0, 1.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * viewMatrix * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), -1.57f, glm::vec3(0.0, 1.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * viewMatrix * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), 3.14159265f, glm::vec3(0.0, 1.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * viewMatrix * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), 1.57f, glm::vec3(1.0, 0.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * viewMatrix * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	starRotation = glm::rotate(glm::mat4(), -1.57f, glm::vec3(1.0, 0.0, 0.0));

	ModelViewProjectionMatrix = projectionMatrix * viewMatrix * starRotation;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));

	glDrawElements(GL_TRIANGLES, starVertices, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	isTexture = 0;
	glUniform1i(showTexture, isTexture);

	for (int i = 0; i < nModels; i++) {
		modelMatrix[i] = model[i]->getModelMatrix();
	}
	camUpdate();
	ShipPos = getPosition((viewMatrix * modelMatrix[Ship]));
	RuberPos = getPosition((viewMatrix * modelMatrix[Ruber]));
	for (int i = 0; i < nModels; i++) {
		
		if (i == Ruber)
			isSun = 1;
		else
			isSun = 0;


		ModelView = viewMatrix * modelMatrix[i];
		NormalMatrix = glm::mat3(ModelView);
		ModelViewProjectionMatrix = projectionMatrix * ModelView;

		glUniformMatrix4fv(MV, 1, GL_FALSE, glm::value_ptr(ModelView));
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
		glUniformMatrix3fv(NM, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
		
		glUniform1i(pointLightSetOn, pointLightOn);
		glUniform3fv(pointLightLoc, 1, glm::value_ptr(RuberPos)); // set location of PointLight
		glUniform3fv(pointLightIntensity, 1, glm::value_ptr(PointLightIntensity)); //sets RGB values in shader

		glUniform1i(headLightSetOn, headLightOn);
		glUniform3fv(headLightLoc, 1, glm::value_ptr(RuberPos)); // set location of HeadLight
		glUniform3fv(headLightIntensity, 1, glm::value_ptr(HeadLightIntensity)); //sets RGB values in shader

		glUniform1i(ambientSetOn, ambientOn);
		glUniform3fv(ambientLightIntensity, 1, glm::value_ptr(AmbientLightIntensity));

		glUniform1i(debugSetOn, debugLightOn);
		glUniform1i(isTheSun, isSun);
		 

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
	case 'p': case 'P':
		pointLightOn = (pointLightOn + 1) % 2;
		break;
	case 'd': case 'D':
		debugLightOn = (debugLightOn + 1) % 2;
		break;
	case 'h': case 'H':
		headLightOn = (headLightOn + 1) % 2;
		break;
	case 'a': case 'A':
		ambientOn = (ambientOn + 1) % 2;
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