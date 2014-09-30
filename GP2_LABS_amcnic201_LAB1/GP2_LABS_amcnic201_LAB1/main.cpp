//Header Files
#include <iostream>

//Header fo SDL2 functionality
#include <SDL.h>




//Global Variables Go Here

//Pointer to our SDL Windows
SDL_Window*window;

//Constant to control window creation
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
bool running = true;



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
	SDL_DestroyWindow(window);
	SDL_Quit();

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
	}

	CleanUp();

	return 0;

}

