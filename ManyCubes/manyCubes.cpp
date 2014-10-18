
# define __Windows__ // define your target operating system
# define Ruber 0
# define Unum 1
# define Duo 2
# define Primus 3
# define Secundus 4

# include "../includes465/include465.hpp"  

# include "Planet.hpp"
# include "SpaceShip.hpp"
# include "Camera.hpp"

// Shapes
const int nPlanets = 5;
Planet * planet[nPlanets];
SpaceShip * spaceShip;
Camera * topView;
Camera * unumView;
Camera * frontView;
Camera * duoView;
// Model for shapes
char * planetModFile = "cube-1-1-1.tri"; // name of planet model file
char * shipModFile = "ship.tri"; // name of ship model File
const GLuint nVertices = 480 * 3;  // 3 vertices per line (surface) of model file  
const GLuint shipVertices = 515 * 3;
float boundingRadius;  // modelFile's bounding radius
float shipBoundingRadius;
int Index =  0;  // global variable indexing into VBO arrays

/* current camera view */
int curView = 0;

// display state and "state strings" for title display
// window title strings
char baseStr[50] =    "465 manyCubes Example {f, t, r} : ";
char fpsStr[15], viewStr[15] =    " front view";
char titleStr [100]; 

GLuint vao;  // VertexArrayObject
GLuint buffer;
GLuint shipBuffer;
GLuint shaderProgram; 
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint MVP ;  // Model View Projection matrix's handle
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 modelMatrix;          // set in shape[i]-->updateDraw()
glm::mat4 viewMatrix;           // set in keyboard()
glm::mat4 ModelViewProjectionMatrix; // set in display();


// vectors and values for lookAt
glm::vec3 eye, at, up;

// vectors for "model"
glm::vec4 vertex[nVertices];
glm::vec3 normal[nVertices];
glm::vec4 diffuseColorMaterial[nVertices];

glm::vec4 shipVertex[shipVertices];
glm::vec3 shipNormal[shipVertices];
glm::vec4 shipDiffuseColorMaterial[shipVertices];

// rotation variables
glm::mat4 identity(1.0f); 
glm::mat4 rotation;
int timerDelay = 40, frameCount = 0;  // A delay of 40 milliseconds is 25 updates / second
double currentTime, lastTime, timeInterval; 


void init(void) {
	boundingRadius = loadTriModel(planetModFile, nVertices, vertex, diffuseColorMaterial, normal);
	if (boundingRadius == -1.0f) {
		printf("loadTriModel error:  returned -1.0f \n");
		exit(1);
	}
	else
		printf("loaded %s model with %7.2f bounding radius \n", planetModFile, boundingRadius);

	shipBoundingRadius = loadTriModel(shipModFile, shipVertices, shipVertex, shipDiffuseColorMaterial, shipNormal);
	if (shipBoundingRadius == -1.0f) {
		printf("loadTriModel error:  returned -1.0f \n");
		exit(1);
	}
	else
		printf("loaded %s model with %7.2f bounding radius \n", shipModFile, shipBoundingRadius);

	shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
	glUseProgram(shaderProgram);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	// GLuint buffers;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) + sizeof(diffuseColorMaterial) + sizeof(normal), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex), sizeof(diffuseColorMaterial), diffuseColorMaterial);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex) + sizeof(diffuseColorMaterial), sizeof(normal), normal);


	// set up vertex arrays (after shaders are loaded)
	GLuint vPosition = glGetAttribLocation(shaderProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(shaderProgram, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertex)));

	// Normal code is here for later use with lighting and shaders
	GLuint vNormal = glGetAttribLocation(shaderProgram, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertex) + sizeof(diffuseColorMaterial)));

	MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");


	// set render state values
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	// create shape

	planet[Ruber] = new Planet(glm::vec3(2000.0f, 2000.0f, 2000.0f), 0.0f, glm::vec3(0, 0, 0), false);
	planet[Unum] = new Planet(glm::vec3(200.0f, 200.0f, 200.0f), 0.004f, glm::vec3(4000, 0, 0), true, planet[Ruber]);
	planet[Duo] = new Planet(glm::vec3(400.0f, 400.0f, 400.0f), 0.002f, glm::vec3(-9000, 0, 0), true, planet[Ruber]);
	planet[Primus] = new Planet(glm::vec3(100.0f, 100.0f, 100.0f), 0.004f, glm::vec3(-8100, 0, 0), true, planet[Duo]);
	planet[Secundus] = new Planet(glm::vec3(150.0f, 150.0f, 150.0f), 0.002f, glm::vec3(-7250, 0, 0), true, planet[Duo]);

	printf("%d Planets created \n", nPlanets);

	frontView = new Camera(glm::vec3(0.0f, 10000.0f, 20000.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), planet[0]);
	topView = new Camera(glm::vec3(0.0f, 20000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), planet[0]);
	unumView = new Camera(glm::vec3(0.0f, 4000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), planet[1]);
	duoView = new Camera(glm::vec3(0.0f, 4000.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), planet[2]);

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
		viewMatrix = frontView->getViewMatrix();
		strcpy(viewStr, " Front View");
	}
	else if (curView == 1){
		viewMatrix = topView->getViewMatrix();
		strcpy(viewStr, " Top View");
	}
	else if (curView == 2){
		viewMatrix = unumView->getViewMatrix();
		strcpy(viewStr, " Unum View");
	}
	else if (curView == 3) {
		viewMatrix = duoView->getViewMatrix();
		strcpy(viewStr, " Duo View");
	}
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	camUpdate();
	// update model matrix, set MVP, draw
	for(int i = 0; i < nPlanets; i++) { 
		modelMatrix = planet[i]->getModelMatrix(); 
		/*
		printf("%d\n",i);
		for (int c = 0; c < 4; c++){
			for (int j = 0; j < 4; j++){
				printf("%f  ", modelMatrix[c][j]);
			}
			printf("\n");
		}
		printf("\n");
		*/

		ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix; 
		glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix)); 
		glDrawArrays(GL_TRIANGLES, 0, nVertices);
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

// Animate scene objects by updating their transformation matrices
// timerDelay = 40, or 25 updates / second
void update (int i) { 
  glutTimerFunc(timerDelay, update, 1);
  for(int i = 0; i < nPlanets; i++) planet[i] -> update();  
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
  

