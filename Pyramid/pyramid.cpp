/*

pyramid.cpp
Shaders:  pyramidVertex.glsl  amd pyramidFragment.glsl

Example C OpenGL Core 3.3 example using:
VAOs   Vertex Array Objects
VBOs   Vertex Buffer Objectss 

Losely based on Angel & Shreiner's Introduction to Modern OpenGL Programming,
SIGGRAPH 2012
www.cs.unm.edu/~angel/SIGGRAPH12/SIGGRAPH_2012.pptx

Uses include465.h, freeglut, glew32, and glm.

With Visual Studio you need the shader files in the same directory as the source 
files for compile | execution within the Visual Studio IDE.
If you want to execute outside the IDE also put copies of the shader files where
the executable is (Debug).

Mike Barnes
8/16/2014
*/

# define __Windows__ // define your target operating system
# include "../includes465/include465.hpp"

const GLuint NumVertices = 18;
int Index =  0;  // global variable indexing into VBO arrays
// display state and "state strings" for title display
bool perspective = true;
GLfloat aspectRatio;
char viewStr[25] =    " front view";
char projectStr[25] = " perspective";
char baseStr[50] =    "465 DrawArrays Example {o, p, f, l, r, t, b} : ";
char titleStr [75]; 

GLuint vao;  // VertexArrayObject
GLuint shaderProgram; 
char * vertexShaderFile = "pyramidVertex.glsl";
char * fragmentShaderFile = "pyramidFragment.glsl";
// GLuint modelView, projection ;  // handles for uniform model - view  and projection matrices
GLuint MVP;  // ModelViewProjection handle
glm::mat4 projectionMatrix;     // set in display
glm::mat4 modelViewMatrix;      // set in display()
glm::mat4 modelViewProjectionMatrix;  // set in display()

// vectors and values for lookAt
glm::vec3 eye, at, up;

// vectors for "model"

glm::vec4 point[NumVertices];
glm::vec4 diffuseColorMaterial[NumVertices];

glm::vec4 const vertex_positions[5] = {
  glm::vec4( -5.0f, -8.0f,  5.0f, 1.0f ), // 0 front left bottom 
  glm::vec4(  5.0f, -8.0f,  5.0f, 1.0f ), // 1 front right bottom
  glm::vec4(  5.0f, -8.0f, -5.0f, 1.0f ), // 2 back right bottom
  glm::vec4( -5.0f, -8.0f, -5.0f, 1.0f ), // 3 back left bottom
  glm::vec4(  0.0f,  8.0f,  0.0f, 1.0f ), // 4 apex
  };

// RGBA colors for each vertex
glm::vec4 const vertex_color[5] = {
  glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ),  // 0, red
  glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ),  // 1, green
  glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ),  // 2, blue
  glm::vec4( 0.8f, 0.8f, 0.8f, 1.0f ),  // 3, light gray
  glm::vec4( 0.8f, 0.8f, 0.8f, 1.0f )   // 4, light gray
  };

// Make a triagle surface by vertex reference
void triangle(int a, int b, int c) {
  point[Index] = vertex_positions[a]; diffuseColorMaterial[Index] = vertex_color[a]; Index++;
  point[Index] = vertex_positions[b]; diffuseColorMaterial[Index] = vertex_color[b]; Index++;
  point[Index] = vertex_positions[c]; diffuseColorMaterial[Index] = vertex_color[c]; Index++;
  };


void pyramid() {
  triangle(0, 1, 4); // Front Face
	triangle(1, 2, 4); // Right Face 
	triangle(2, 3, 4); // Back Face
	triangle(3, 0, 4); // Left Face
 	triangle(0, 2, 1); // Bottom Face front
	triangle(0, 3, 2); // Bottom Face back
  };

void init (void) {
  printf("init:  sizeof glm::vec4 %d point %d diffuseColorMaterial %d \n", sizeof(glm::vec4), sizeof(point), sizeof(diffuseColorMaterial));

  pyramid();

  shaderProgram = loadShaders(vertexShaderFile,fragmentShaderFile);
  glUseProgram(shaderProgram);

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof(point) + sizeof(diffuseColorMaterial), NULL, GL_STATIC_DRAW );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point), point );
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(point), sizeof(diffuseColorMaterial), diffuseColorMaterial );


  // set up vertex arrays (after shaders are loaded)
  GLuint vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  GLuint vColor = glGetAttribLocation( shaderProgram, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point)) );

  // modelView =  glGetUniformLocation(shaderProgram, "ModelView");
  // projection = glGetUniformLocation(shaderProgram, "Projection");
  MVP = glGetUniformLocation(shaderProgram, "ModelViewProject");
  
// initially use a front view
  eye = glm::vec3(0.0f, 0.0f, 70.0f);   // eye is 50 "out of screen" from origin
  at  = glm::vec3(0.0f, 0.0f,  0.0f);   // looking at origin
  up  = glm::vec3(0.0f, 1.0f,  0.0f);   // camera'a up vector

  // set render state values
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  }

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
  aspectRatio = (GLfloat) width /  (GLfloat) height;
  printf("reshape: width = %4d height = %4d aspect = %5.2f \n", width, height, aspectRatio);
  }

void display(void) {
  printf("display %s %s \n", projectStr, viewStr);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // update model view projection matrix
  modelViewMatrix = glm::lookAt(eye, at, up);
  if (perspective)  // 60 degrees = 1.047 radians
      projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 1.0f, 1000.0f); 
    else
      projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 1000.0f);    
  // update for vertex shader
  modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
  glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
  // draw 
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  glutSwapBuffers();
  }

void keyboard (unsigned char key, int x, int y) {
  switch(key) {
    case 033 : case 'q' :  case 'Q' : exit(EXIT_SUCCESS); break;
    case 'b' : case 'B' :  // bottom view
        eye = glm::vec3(0.0f, -50.0f, 0.0f);   // eye is -50 "into screen" from origin
        at  = glm::vec3(0.0f,   0.0f, 0.0f);   // looking at origin
        up  = glm::vec3(0.0f,   0.0f, -1.0f);  // camera'a up vector
        strcpy(viewStr, " bottom view"); break;
    case 'f' : case 'F' :  // front view
        eye = glm::vec3(0.0f, 0.0f, 50.0f);   // eye is 50 "out of screen" from origin
        at  = glm::vec3(0.0f, 0.0f,  0.0f);   // looking at origin
        up  = glm::vec3(0.0f, 1.0f,  0.0f);   // camera'a up vector
        strcpy(viewStr, " front view"); break;
    case 'l' : case 'L' :  // left view
        eye = glm::vec3(-50.0f, 0.0f, 0.0f);   // eye is 50 "out of screen" from origin
        at  = glm::vec3(  0.0f, 0.0f, 0.0f);   // looking at origin
        up  = glm::vec3(  0.0f, 1.0f, 0.0f);   // camera'a up vector
        strcpy(viewStr, " left view"); break;
    case 'r' : case 'R' :  // right view
        eye = glm::vec3(50.0f, 0.0f, 0.0f);   // eye is 50 "out of screen" from origin
        at  = glm::vec3( 0.0f, 0.0f, 0.0f);   // looking at origin
        up  = glm::vec3( 0.0f, 1.0f, 0.0f);   // camera's up vector  
        strcpy(viewStr, " right view"); break;
    case 't' : case 'T' :  // right view
        eye = glm::vec3(0.0f, 50.0f,  0.0f);   // eye is 50  up from origin
        at  = glm::vec3(0.0f,  0.0f,  0.0f);   // looking at origin
        up  = glm::vec3(0.0f,  0.0f, -1.0f);   // camera's up is looking towards -Z vector
        strcpy(viewStr, " top view"); break;
    case 'p' : case 'P' : // set perspective projection
      perspective = true;
      strcpy(projectStr, " perspective"); break;
    case 'o' : case 'O' : // set orthographic perspective projection
      perspective = false;
      strcpy(projectStr, " orthographic"); break;
    }
  strcpy(titleStr, baseStr);
  strcat(strcat(titleStr, projectStr), viewStr);
  glutSetWindowTitle( titleStr);
  glutPostRedisplay();
  }
    
int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow("465 DrawArrays Example {o, p, f, l, r, t, b} : perspective front view");
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
  glutMainLoop();
  printf("done\n");
  return 0;
  }
  

