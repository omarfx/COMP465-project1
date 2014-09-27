/*

viewTriModel.cpp

465 utility include files:  shader465.hpp, triModel465.hpp, glmUtils465.hpp  
Shaders:  simpleVertex.glsl  amd simpleFragment.glsl
Added a comment

C OpenGL Core 3.3 example that loads cube.tri model file
and rotates the cube.

User commands:
'a' switch between interval timer (25 fps) and idle timer (faster?)
'f' view from front (0, 0, 3) looking at origin
't' view from above (0, 3, 3) looking at origin
'b' view from below (0, -3, 0) looking at origin
'o' orthographic projection (note cube doesn't render as cube)
'p' perspective projection (aspect ration renders a cube)
'w' wireframe or surface 

Current state is displayed in the window title.
 
The cube.tri model uses 12 triangular surfaces to display 6 "quad" surfaces.  
So, there are 36 (3 * 12) vertices.

The triModel465.hpp utility creates colors, and normals for ever vertex.
However the simple*.glsl shaders do not use the normal. 
Since the same color is created for every vertex of every surface,
the object is rendered with flat shading.

A reference for estimating fps using glut functions:
http://mycodelog.com/2010/04/16/fps/

Mike Barnes
8/16/2014
*/

# define __Windows__ // define your target operating system
# include "../includes465/include465.hpp"

char * modelFile =  "cube.tri";  // "obelisk-10-20-10.tri"; // name of tri model file
const GLuint nVertices = 12 * 3;       // 14 * 3; //  3 vertices per line of model file  
float boundingRadius;  // modelFile's bounding radius
int Index =  0;  // global variable indexing into VBO arrays
// display state and "state strings" for title display
bool perspective = true;
GLfloat aspectRatio;
// window title strings
char baseStr[50] =    "Project Ruber - Phase 1 {v, q} : ";
char viewStr[15] =    " Front View";
char fpsStr[15], timerStr[20] = " interval timer";
char titleStr [100]; 

GLuint vao;  // VertexArrayObject
GLuint shaderProgram; 
char * vertexShaderFile = "simpleVertex.glsl";
char * fragmentShaderFile = "simpleFragment.glsl";
GLuint MVP;  // ModelViewProjection hangle
glm::mat4 projectionMatrix;     // set in display
glm::mat4 modelViewMatrix;      // set in display()
glm::mat4 modelViewProjectionMatrix;  // set in display()

// vectors and values for lookAt
glm::vec3 eye, at, up;

// vectors for "model"
glm::vec4 vertex[nVertices];
glm::vec3 normal[nVertices];
glm::vec4 diffuseColorMaterial[nVertices];

// rotation variables  -- the modelMatrix
GLfloat rotateRadian = 0.0f;
float eyeDistanceMultiplier = 6.0f;
float eyeDistance;
glm::mat4 identity(1.0f); 
glm::mat4 rotation;  // the modelMatrix
int timerDelay = 40, frameCount = 0;
double currentTime, lastTime, timeInterval;
bool idleTimerFlag = false;  // interval or idle timer ?
bool wireFrame = false;     // initially show surfaces

void init (void) {
  printf("init:  sizeof glm::vec4 %d point %d diffuseColorMaterial %d normal %d \n", 
    sizeof(glm::vec4), sizeof(vertex), sizeof(diffuseColorMaterial), sizeof(normal));

  boundingRadius = loadTriModel(modelFile, nVertices, vertex, diffuseColorMaterial, normal);
  if (boundingRadius == -1.0f) {
    printf("loadTriModel error:  returned -1.0f \n");
    exit(1); }
    else
      printf("loaded %s model with %7.2f bounding radius \n", modelFile, boundingRadius);

  shaderProgram = loadShaders(vertexShaderFile,fragmentShaderFile);
  glUseProgram(shaderProgram);

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(vertex) + sizeof(diffuseColorMaterial) + sizeof(normal), NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vertex), vertex );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertex), sizeof(diffuseColorMaterial), diffuseColorMaterial );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertex) + sizeof(diffuseColorMaterial), sizeof(normal), normal );


  // set up vertex arrays (after shaders are loaded)
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertex)) );

  GLuint vNormal = glGetAttribLocation( shaderProgram, "vNormal" );
  glEnableVertexAttribArray( vNormal);
  glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertex) + sizeof(diffuseColorMaterial)) );

  MVP = glGetUniformLocation(shaderProgram, "ModelViewProject");
  
  eyeDistance = eyeDistanceMultiplier * boundingRadius;
  // initially use a front view
  eye = glm::vec3(0.0f, 10000.0f, 20000.0f);	// camera slightly above and back
  at = glm::vec3(0.0f, 0.0f, 0.0f);			// look at origin
  up = glm::vec3(0.0f, 1.0f, 0.0f);			// up vector Y


  // set render state values
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

  lastTime = glutGet(GLUT_ELAPSED_TIME);  // get elapsed system time
  }

// set viewport and projectionMatrix on window resize events
void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  aspectRatio = (GLfloat) width /  (GLfloat) height;

  projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 1.0f, 1000.0f); 
  printf("reshape: width = %4d height = %4d aspect = %5.2f \n", width, height, aspectRatio);
  }

// update and display animation state in window title
void updateTitle() {
  strcpy(titleStr, baseStr);
  strcat(titleStr, viewStr);
  strcat(titleStr, timerStr);
  strcat(titleStr, fpsStr);
  // printf("title string = %s \n", titleStr);
  glutSetWindowTitle( titleStr);
  }

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // update model view projection matrix
  modelViewMatrix = glm::lookAt(eye, at, up) * rotation;  // modelview = view * model   
  modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
  glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
  // draw 
  if (wireFrame)
      glDrawArrays(GL_LINES, 0, nVertices);
    else
      glDrawArrays(GL_TRIANGLES, 0, nVertices);
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

// for use with Idle and intervalTimer functions 
// to set rotation
void update(void){
  rotateRadian += 0.1f;
  if (rotateRadian >  2 * PI ) rotateRadian = 0.0f;
  rotation = glm::rotate(identity, rotateRadian, glm::vec3(0, 1, 0));
  glutPostRedisplay();
  }

// Estimate FPS, use for fixed interval timer driven animation
void intervalTimer (int i) { 
  glutTimerFunc(timerDelay, intervalTimer, 1);   
  if (! idleTimerFlag) update();  // fixed interval timer
  }

/* current camera view */
int curView = 0;

/* listens for keyboard clicks */
void keyboard (unsigned char key, int x, int y) {
	switch(key) {

		/* -- quit -- */
		case 033 : case 'q' :  case 'Q' :

			exit(EXIT_SUCCESS);
			break;

		/* -- change view -- */
		case 'v': case 'V':

			/* -- evaluate view -- */
			curView = (curView + 1) % 4;

			/* -- front camera -- */
			if (curView == 0) {
				eye = glm::vec3(0.0f, 10000.0f, 20000.0f);	// camera slightly above and back
				at = glm::vec3(0.0f, 0.0f, 0.0f);			// look at origin
				up = glm::vec3(0.0f, 1.0f, 0.0f);			// up vector Y
				strcpy(viewStr, " Front View");
				break;
			}

			/* -- top camera -- */
			else if (curView == 1) {
				eye = glm::vec3(0.0f, 20000.0f, 1.0f);		// camera straight above field
				at = glm::vec3(0.0f, 0.0f, 0.0f);			// look at origin
				up = glm::vec3(0.0f, 1.0f, 0.0f);			// up vector Y
				strcpy(viewStr, " Top View");
				break;
			}

			/* -- unum camera -- */
			else if (curView == 2) {
				eye = glm::vec3(0.0f, 4000.0f, 1.0f);		// camera slightly above and back
				at = glm::vec3(0.0f, 0.0f, 0.0f);			// look at unum ** NEEDS UPDATING **
				up = glm::vec3(0.0f, 1.0f, 0.0f);			// up vector Y
				strcpy(viewStr, " Unum View");
				break;
			}

			/* -- duo camera -- */
			else if (curView == 3) {
				eye = glm::vec3(0.0f, 4000.0f, 1.0f);		// camera straight above field
				at = glm::vec3(0.0f, 0.0f, 0.0f);			// look at duo ** NEEDS UPDATING **
				up = glm::vec3(0.0f, 1.0f, 0.0f);			// up vector Y
				strcpy(viewStr, " Duo View");
				break;
			}

			// /* -- ship camera -- */
			// else if (curView == 4) {
			// 
			// }
			//
			// /* -- ship camera -- */
			// else if (curView == 5) {
			// 
			// }

			/* update title on page base on view */
			updateTitle();

	}
}
    
int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow("Project Ruber - Phase 1 {v, q} : Front View");
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
  glutIdleFunc(NULL);  // start with intervalTimer
  glutTimerFunc(timerDelay, intervalTimer, 1);
  glutMainLoop();
  printf("done\n");
  return 0;
  }
  

