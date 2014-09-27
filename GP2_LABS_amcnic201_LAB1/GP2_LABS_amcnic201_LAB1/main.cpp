//Header Files
#include <iostream>
//Pointer to our SDL Windows
#include <SDL h>




//Global variables go here
//Pomiter to our SDL Windows
SDL_Window*window;

//Constants to control the windows creation
const int WINDOW_width = 640;
const int WINDOW_height = 480;


void InitWindow(int width, int height, bool fullscreen) {
	
	//Create a window 
	window = SDL - CreateWindow(
		"Lab_1", // window title
		SDL_WINDOWPOS_CENTERED, //x position centered
		SDL_WINDOWPOS_CENTERED, //y position centered
		width,					//width in pixels 
		height,					//height in pixels 
		SDL_WINDOW_OPENGL		//flags
								);
}

//Used to Cleanup once we exit 

void CleanUp(){

	SDL_DestroyWindow(window);
	SDL_Quit();

}

// MIGHT BE MISSING A VOID CLEANUP STATEMENT HERE/ABOVE: LAB INSTRUCTION UNCLEAR ON PAGE 7 ABOUT CLEAN UP WINDOW 

//Main Method - Entry Point
int main(int argc, char* arg[]) {

	//init everything SDL, if it is non-zero we have a problem
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "ERROR SDL_Init" << SDL_GetError() << std::endl;
		return-1;
	}
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	return 0;
}


