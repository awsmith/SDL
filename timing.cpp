// Headers
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <sstream>

// Constants
int const SCREEN_WIDTH = 640;
int const SCREEN_HEIGHT = 480;
int const SCREEN_BPP = 32;

// Surfaces
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *seconds = NULL;
SDL_Surface *startStop = NULL;

// Font
TTF_Font *font = NULL;

// Color
SDL_Color textColor = {0xF0, 0xFF, 0xF0};

// Event
SDL_Event event;

// Functions

// Loads image from filename arugment and returns as 32bit
SDL_Surface *loadImage(std::string filename)
{
  // Pointers to hold and return image from file as 32bit
  SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

  // Loads image from argument
	loadedImage = IMG_Load(filename.c_str());
	
  // If image was loaded
  if(loadedImage != NULL)
	{
    // Convert loadedImage and sets optimizedImage to address of conversion
		optimizedImage = SDL_DisplayFormat(loadedImage);
    // Delete pointer
		SDL_FreeSurface(loadedImage);
		// If image was converted
    if(optimizedImage != NULL)
		{
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}
	return optimizedImage;
}

// Given x, y coordinates surface is applied to another surface, the screen
void applySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(source, clip, destination, &offset);
}

// Initialize all systems. If anything fails to load function will return false
bool init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	if(TTF_Init() == -1)
	{
		return false;
	}

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if(screen == NULL)
	{
		return false;
	}

	SDL_WM_SetCaption("Timer Test", NULL);

	return true;
}

// Load images and fonts. Returns false if there is an error while loading.
bool loadFiles()
{
	background = loadImage("background.png");
	font = TTF_OpenFont("lazy.ttf", 36);

	if(background == NULL)
	{
		return false;
	}
	if(font == NULL)
	{
		return false;
	}

	return true;
}

// Frees up memory from all pointers and closes TTF and SDL.
void cleanUp()
{
	SDL_FreeSurface(background);
	SDL_FreeSurface(startStop);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

int main(int arc, char* args[])
{
	// Quit flag
	bool quit = false;
  
  // Closes program if any error occurs initializing
	if(init() == false)
	{
		return 1;
	}

  // Closes program if any error occurs loading files
	if(loadFiles() == false)
	{
		return 2;
	}

	// The timer starting time - Unit32 = unsigned integer 32 bits
	Uint32 start = 0;

  // Sets startStop to inform user of timer functionality
	startStop = TTF_RenderText_Solid(font, "Press S to start or stop the timer", textColor);
	
	// The timer start / stop flag
	bool running = true;

	// Start the timer
	start = SDL_GetTicks();

	// While the user hasn't quit
	while(quit == false)
	{
		// While there's an event to handle
		while(SDL_PollEvent(&event))
		{
			// If a key was presed
			if(event.type == SDL_KEYDOWN)
			{
				// If s was pressed. SDLK_(whatever) = key on keyboard
				if(event.key.keysym.sym == SDLK_s)
				{
					// If the timer is running
					if(running == true)
					{
						// Stop the timer
						running = false;
            // Reset cycle
						start = 0;
					}
					else
					{
						// Start the timer
						running = true;
						start = SDL_GetTicks();
					}
				}
			}
			else if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
    // Background applied to screen
		applySurface(0, 0, background, screen);
    // Display string of info for user how to manipulate timer
		applySurface((SCREEN_WIDTH- startStop->w)/2 , 200, startStop, screen);

		// If the timer is running
		if(running == true)
		{
			// The timer's time as a string
			std::stringstream time;

			// Conver the timer's time to a string
      // time = current time from GetTicks() - start time
			time << "Timer: " << SDL_GetTicks() - start;

			// Render the time surface
			seconds = TTF_RenderText_Solid(font, time.str().c_str(), textColor);
			// Apply the time surface
			applySurface((SCREEN_WIDTH - seconds->w)/2, 50, seconds, screen);

			// Free the time surface
			SDL_FreeSurface(seconds);
		}

		// Update the screen
		if(SDL_Flip(screen) == -1)
		{
			return 3;
		}
	}
	cleanUp();
	return 0;
		
}	
