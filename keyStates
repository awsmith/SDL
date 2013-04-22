/* SDL Tutorial 10 Key States.
   SDL_Rect* clip is present in source code from site, not sure why. Removed and code appears to work fine.
   2013/04/21
   a_s_
*/

#include <iostream>
#include <string>
#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_ttf.h"


// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// Surface
SDL_Surface* screen = NULL;
SDL_Surface* image = NULL;
SDL_Surface* up = NULL;
SDL_Surface* down = NULL;
SDL_Surface* left = NULL;
SDL_Surface* right = NULL;

// Events
SDL_Event event;

// Font
TTF_Font *font = NULL;
SDL_Color textColor = {0, 0, 0};

// Prototypes
bool init();
bool load_files();
SDL_Surface *load_image(std::string filename);
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination);
void clean_up();


int main(int argc, char* args[])
{
  // Flag
	bool quit = false;

	// INITIALIZE ALL THE SYSTEMS!
	if(init() == false)
	{
		return 1;
	}

	// Load the files
	if(load_files() == false)
	{
		return 1;
	}


	// Set us up the text
	up = TTF_RenderText_Solid(font, "Up" , textColor);
	down = TTF_RenderText_Solid(font, "Down" , textColor);
	left = TTF_RenderText_Solid(font, "Left" , textColor);
	right = TTF_RenderText_Solid(font, "Right" , textColor);

	// While the user has not quit
	while( quit == false)
	{
		// While there are events to handle
		while(SDL_PollEvent( &event ))
		{
			// If the user has Xed out the window
			if(event.type == SDL_QUIT)
			{
				// Quit the program
				quit = true;
			}
		}

		// Apply background
		apply_surface(0, 0, image, screen);

		// Get the keystates
		Uint8 *keystates = SDL_GetKeyState(NULL);
	
		// If up is pressed
		if(keystates[SDLK_UP])
		{
			apply_surface((SCREEN_WIDTH - up->w)/2,(SCREEN_HEIGHT/2 - up->h)/2, up, screen);
		}

		// If down is pressed
		if(keystates[SDLK_DOWN])
		{
			apply_surface((SCREEN_WIDTH - down->w)/2,(SCREEN_HEIGHT/2 - down->h)/2 + (SCREEN_HEIGHT / 2), down, screen);
		}

		// If left is pressed
		if(keystates[SDLK_LEFT])
		{
			apply_surface((SCREEN_WIDTH/2 - left->w)/2, (SCREEN_HEIGHT - left->h)/2, left, screen);
		}
		// If right is pressed
		if(keystates[SDLK_RIGHT])
		{
			apply_surface((SCREEN_WIDTH/2 - right->w)/2 + (SCREEN_WIDTH/2), (SCREEN_HEIGHT - right->h)/2, right, screen);
		}

		// Update the screen
		if(SDL_Flip(screen) == -1)
		{
			return 1;
		}
	}
	clean_up();

	return 0;
}

bool init()
{
	// Initialize all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	
	// Set up the screen
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	// If there was an error setting up the screen
	if(screen == NULL)
	{
		return false;
	}

	// Initialize font
	if(TTF_Init() == -1)
	{
		return false;
	}

	// Set the window caption
	SDL_WM_SetCaption("Key states", NULL);

	// If all fine
	return true;
}
bool load_files()
{
	// Load the image
	image = load_image("background.png");

	// Load font
	font = TTF_OpenFont("lazy.ttf", 72);

	// If there was an error
	if( image == NULL)
	{
		return false;
	}

	// If there was an error
	if(font == NULL)
	{
		return false;
	}

	// If no error
	return true;
}
SDL_Surface *load_image(std::string filename)
{
	// SLD_Surface pointers. For loaded image and optimized image to return for use
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	// Load the image
	loadedImage = IMG_Load(filename.c_str());

	// If image loaded fine
	if(loadedImage != NULL)
	{
		// Create optimized image
		optimizedImage = SDL_DisplayFormat(loadedImage);
	
		// Free unneeded pointer
		SDL_FreeSurface(loadedImage);
		
		if(optimizedImage != NULL)
		{
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}

	// Return loaded image after being optimized
	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;

	// Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, NULL, destination, &offset);
}

void clean_up()
{
	// Memory leak bad
	SDL_FreeSurface(image);
	SDL_FreeSurface(up);
	SDL_FreeSurface(down);
	SDL_FreeSurface(left);
	SDL_FreeSurface(right);

	// Plug font leak
	TTF_CloseFont(font);
	TTF_Quit();

	// Quit SDL
	SDL_Quit();
}
