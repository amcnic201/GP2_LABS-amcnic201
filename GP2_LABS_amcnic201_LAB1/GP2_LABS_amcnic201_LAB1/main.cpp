//Header Files
#include <iostream>
#include "Vertex.h"
#include "Shader.h"
//Header fo SDL2 functionality
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>

//maths headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//matrices
mat4 viewMatrix;
mat4 projMatrix;
mat4 worldMatrix;



#ifdef _DEBUG && WIN32
const std::string ASSET_PATH = "assets";
#else
const std::string ASSET_PATH = "assets";
#endif

const std::string SHADER_PATH = "/shaders";

//Global Variables Go Here

GLuint triangleEBO;
//Pointer to our SDL Windows
SDL_Window*window;

//SDL GL context
SDL_GLContext glcontext = NULL;


//Constant to control window creation
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
bool running = true;
Vertex triangleData[] = { //Front
		{-0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Left
			
		{-0.5f,-0.5f, 0.5f,
		1.0f, 1.0f, 0.0f, 1.0f },// Bottom Left
				
		{ 0.5f,-0.5f, 0.5f,
		0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right
				
		{ 0.5f, 0.5f, 0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Right
				
		//back
		{-0.5f, 0.5f,-0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Left
					
		{-0.5f,-0.5f,-0.5f,
		1.0f, 1.0f, 0.0f, 1.0f },// Bottom Left
						
		{ 0.5f,-0.5f,-0.5f,
		0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right
						
		{ 0.5f, 0.5f,-0.5f,
		1.0f, 0.0f, 1.0f, 1.0f },// Top Right

} ;

GLuint indices[] = {
	//Front 
	0, 1, 2,
	0, 3, 2,

	//Left
	4, 5, 1,
	4, 1, 0,

	//right
	3, 7, 2,
	7, 6, 2,

	//bottom
	1, 5, 2,
	6, 2, 1,

	//top
	5, 0, 7,
	5, 7, 3,

	//back
	4, 5, 6,
	4, 7, 6,


};


GLuint triangleVBO;



//Global Functions

void InitWindow(int width, int height, bool fullscreen) {

	//Create a window
	window = SDL_CreateWindow(
		"LAB 1", // window title
		SDL_WINDOWPOS_CENTERED, // X POSITION CENTERED
		SDL_WINDOWPOS_CENTERED, // Y POSTION CENTRED
		width,
		height,
		SDL_WINDOW_OPENGL  //flags
		);

}


//Used To CleanUp once we exit 

void CleanUp()
{
	glDeleteProgram(shaderProgram);
	glDeleteBuffers(1, &triangleEBO);
	glDeleteBuffers(1, &triangleVBO);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

//Function to initialise OpenGL
void initOpenGL(){

	//Ask for version 3.2 of open GL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create OpenGL context
	glcontext = SDL_GL_CreateContext(window);
		//Something went wrong in creating the conext if it is still NULL
		if (!glcontext)
		{
		std::cout << "Error Creating OPenGl context" << SDL_GetError() <<
			std::endl;
		}

		//Smooth shading 
		glShadeModel(GL_SMOOTH);

		//clear the background to black
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		//Clear the depth buffer to 1.0
		glClearDepth(1.0f);

		//Enable depth testing
		glEnable(GL_DEPTH_TEST);

		//The dpeth test to use
		glDepthFunc(GL_LEQUAL);

		//Turn on the best perspective correction
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glewExperimental = GL_TRUE;

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			//Problem: glewInit failed, something is seriosuly worng.
			std::cout << "Error" << glewGetErrorString(err) << std::endl;

		}



}

void initGeometry()
{
	//Create buffer
	glGenBuffers(1, &triangleVBO);
	//Make the new VBO active 
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	//Copy the vertex Data to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData),
		triangleData,
		GL_STATIC_DRAW);

	//create buufer
	glGenBuffers(1, &triangleEBO);
	//Make the EBO active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
	//Copy the index data to the ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}


//Function tp set/reset viewport 

void setViewport(int width, int height)
{
	//screen ration
	GLfloat ratio;

	//make sure height is always above 0
	if (height == 0){
		height = 1;

	}

	//calculate screen rotation
	ratio = (GLfloat)width / (GLfloat)height;

	//Setup viewport 
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//OLD FIXED FUNCTION FROM LAB 2

	//Change to project matrix mode
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//Calculate perspective matrix,using glu library functions
	//gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	//Switch to ModelView
	//glMatrixMode(GL_MODELVIEW);

	//Reset using the identity Matrix
	//glLoadIdentity();



}

void createShader()
{
	GLuint vertexShaderProgram = 0;
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
	vertexShaderProgram = loadShaderFromFile(vsPath, VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
	fragmentShaderProgram = loadShaderFromFile(fsPath, FRAGMENT_SHADER);

	shaderProgram + glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderProgram);
	glAttachShader(shaderProgram, fragmentShaderProgram);
	glLinkProgram(shaderProgram);
	checkForLinkErrors(shaderProgram);

	glBindAttribLocation(shaderProgram, 0, "vertexPosition");


	//Now we can delete the VS and FS programs
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);

}

GLuint shaderProgram = 0;

//Function to draw
void render()
{
	//Set the clear color (background)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Make the new VBO active.Repeat here as a sanity check
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);

	glUseProgram(shaderProgram);

	GLint MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
	mat4 MVP = projMatrix*viewMatrix*worldMatrix;
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));

	//Tell the shader that 0 is the position element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


	//Establish its 3 coordinates per vertex with zero stride(space between elements)
	//glVertexPointer(3, GL_FLOAT, sizeof(Vertex), NULL);

	//The last parameter basically says that the colours start 3 floats into 
	//each element of the array
	//glColorPointer(4, GL_FLOAT, sizeof(Vertex), (void**)(3 * sizeof(float)));

	//Etablish array contains verices
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

	//Swith to model view
	//glMatrixMode(GL_MODELVIEW);

	//Reset using the identity matrix
	//glLoadIdentity();

	//gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, - 1.0f, 0.0, 1.0, 0.0);

	//Translate to -5.0f on z axis
	//glTranslatef(0.0f, 0.0f, -5.0f);

	//Begin drawing triangles
	/*glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); // Colour of ther vertices
		glVertex3f(0.0f, 1.0f, 0.0f); // Top
		glVertex3f(-1.0f, -1.0f, 0.0f); //Bottom left
		glVertex3f(1.0f, -1.0f, 0.0f); //Bottom right
		glEnd(); */



	//require tp swap the front and back buffer
	SDL_GL_SwapWindow(window);

	//Switch to modelView
	//glMatrixMode(GL_MODELVIEW);

	//Reset using the Indentity matrix
	//glLoadIdentity();

	//translate
	//glTranslatef(0.0f, 0.0f, -6.0f);

	//Actually draw the trianlge,giving the number of vertices provided
	//glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleData) / 3 * sizeof(float));

	 

}

//Function to update the game state
void update()
{
	//projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH) / 
		//(float)WINDOW_HEIGHT, 0.1f, 100.0f);

	projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH /
		(float)WINDOW_HEIGHT, 0.1f, 100.0f);

	viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f,
		0.0f));

	worldMatrix = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));

}



//Main Method-Entry Point
int main(int argc, char*arg[]){

	//init everything - SDL, if it is non zero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;

		return -1;
	}

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	//Call our InitOpenGL function
	initOpenGL();
	initGeometry();
	//Set our viewport
	setViewport(WINDOW_WIDTH, WINDOW_HEIGHT);

	SDL_Event event;

	createShader();

	while (running)
	{
		while (SDL_PollEvent(&event)){
			//Get event type
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
				//set our boolean, which controls the game loop to false
				running = false;
			}
		}

		update();
		render();
	}

	CleanUp();

	return 0;

}



