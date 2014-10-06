//Header Files
#include <iostream>
//#include <Vertex.h>
//Header fo SDL2 functionality
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>


//Global Variables Go Here

//Pointer to our SDL Windows
SDL_Window*window;

//SDL GL context
SDL_GLContext glcontext = NULL;


//Constant to control window creation
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
bool running = true;
float triangleData[] = { 0.0f, 1.0f, 0.0f, //top
-1.0f, -1.0f, 0.0f, //Bottom left
1.0f, -1.0f, 0.0f }; //Bottom Right
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
	glDeleteBuffers(1, &triangleVBO);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

}

//Function to initialise OpenGL
void initOpenGL(){

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

	//Change to project matrix mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Calculate perspective matrix,using glu library functions
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	//Switch to ModelView
	glMatrixMode(GL_MODELVIEW);

	//Reset using the identity Matrix
	glLoadIdentity();



}

//Function to draw
void render()
{
	//Set the clear color (background)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Make the new VBO active.Repeat here as a sanity check
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	//Establish its 3 coordinates per vertex with zero stride(space between elements)
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//Etablish array contains verices
	glEnableClientState(GL_VERTEX_ARRAY);

	//Swith to model view
	glMatrixMode(GL_MODELVIEW);

	//Reset using the identity matrix
	glLoadIdentity();

	//Translate to -5.0f on z axis
	glTranslatef(0.0f, 0.0f, -5.0f);

	//Begin drawing triangles
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); // Colour of ther vertices
		glVertex3f(0.0f, 1.0f, 0.0f); // Top
		glVertex3f(-1.0f, -1.0f, 0.0f); //Bottom left
		glVertex3f(1.0f, -1.0f, 0.0f); //Bottom right
		glEnd();



	//require tp swap the front and back buffer
	SDL_GL_SwapWindow(window);

	//Switch to modelView
	glMatrixMode(GL_MODELVIEW);

	//Reset using the Indentity matrix
	glLoadIdentity();

	//translate
	glTranslatef(0.0f, 0.0f, -6.0f);

	//Actually draw the trianlge,giving the number of vertices provided
	glDrawArrays(GL_TRIANGLES, 0, sizeof(triangleData) / 3 * sizeof(float));

	 

}

//Function to update the game state
void update()
{

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



