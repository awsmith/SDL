// Headers
#include <iostream>
#include <string>
#include <sstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

// Constants
int const SCREEN_WIDTH = 640;
int const SCREEN_HEIGHT = 480;
int const SCREEN_BPP = 32;

// Events
SDL_Event event;

// Surfaces
SDL_Surface *screen;
SDL_Surface *background;
SDL_Surface *startStop;
SDL_Surface *pauseMessage;
SDL_Surface *seconds;

// Font
TTF_Font *font;
// Font color
SDL_Color textColor = {0, 0, 0};

// Prototypes
bool init();
SDL_Surface *load_image(std::string filename);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination);
bool load_files();
void clean_up();

// The timer
class Timer
{
  public:
	// Default constructor
	Timer();
	
	// The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	// Get the timer's time
	int get_ticks();

	// Checks the status of the timer
	bool is_started();
	bool is_paused();

	private:
	// The time when the timer startered
	int startTicks;

	// The ticks stored when the timer was paused
	int pausedTicks;

	// The timer status
	bool paused;
	bool started;
};

int main(int argc, char *args[] )
{
	// Make timer
	Timer myTimer;

	// Quit flag
	bool quit = false;

	if(init() == false)
	{
		return 1;
	}

	if(load_files() == false)
	{
		return 2;
	}

	// Generate the message surfaces
	startStop = TTF_RenderText_Solid(font, "Press S to start or stop the timer", textColor);
	pauseMessage = TTF_RenderText_Solid(font, "Press P to pause or unpause the timer", textColor);

	// Start the timer
	myTimer.start();

	// While the user has not quit
	while(quit == false)
	{
		// While there's an event to handle
		while(SDL_PollEvent(&event))
		{
			// If a key was pressed
			if(event.type == SDL_KEYDOWN)
			{
				// If s was pressed
				if(event.key.keysym.sym == SDLK_s)
				{
					// If the timer is running
					if(myTimer.is_started() == true)
					{
						myTimer.stop();
					}
					else
					{
						myTimer.start();
					}
				}
				// If p was pressed
				if(event.key.keysym.sym == SDLK_p)
				{
					// If the timer is running
					if(myTimer.is_paused() == true)
					{
						myTimer.unpause();
					}
					else
					{
						myTimer.pause();
					}
				}
			}
			// If the user wants to quit
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		// Apply background image
		apply_surface(0, 0, background, screen);
		apply_surface((SCREEN_WIDTH - startStop->w)/2 , 200, startStop, screen);
		apply_surface((SCREEN_WIDTH - pauseMessage->w)/2, 250, pauseMessage, screen);

		// The timer's time as a string
		std::stringstream time;

		// Convert the timer's time to a string
		time << "Timer:" << myTimer.get_ticks()/1000.f;

		// Render the time surface
		seconds = TTF_RenderText_Solid(font, time.str().c_str(), textColor);

		// Apply the surface
		apply_surface((SCREEN_WIDTH - seconds->w)/2, 0, seconds, screen);

		// Free the time surface
		SDL_FreeSurface(seconds);

		// Update the screen
		if(SDL_Flip(screen) == -1)
		{
			return 3;
		}
	}

	clean_up();
	return 0;

}

// Initialize all
bool init()
{
	// Initialize SDL and verify it was
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	
	// Initialize the screen 
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	// Verify screen was initialized
	if(screen == NULL)
	{
		return false;
	}

	// Initialize fonts and very it was 
	if(TTF_Init() == -1)
	{
		return false;
	}

	// Set window caption
	SDL_WM_SetCaption("Advanced timers", NULL);

	// Return true if everything was initialized
	return true;
}

// Image load
SDL_Surface *load_image(std::string filename)
{
	// Pointers for loading and optimizing image
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

	// Convert filename to C string for IMG_Load() and set loadedImage to file loaded
	loadedImage = IMG_Load(filename.c_str());

	if(loadedImage != NULL)
	{
		// Optimized loadedImage to set optimizedImage to
		optimizedImage = SDL_DisplayFormat(loadedImage);
		// Delete unneeded pointer
		SDL_FreeSurface(loadedImage);
		if(optimizedImage != NULL)
		{
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}
	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
	// Temproary rectangle
	SDL_Rect offset;
	// Hold the height and width arguments
	offset.x = x;
	offset.y = y;

	// Blit the screen
	SDL_BlitSurface(source, NULL, destination, &offset);

}

bool load_files()
{
	// Load an image
	background = load_image("background.png");

	// Load font
	font = TTF_OpenFont("lazy.ttf", 24);

	if(background == NULL)
	{
		return false;
	}

	if(font == NULL)
	{
		return false;
	}
	// If everything loaded without error
	return true;
}

void clean_up()
{
	// Free surfaces
	SDL_FreeSurface(background);
	SDL_FreeSurface(startStop);
	SDL_FreeSurface(pauseMessage);
	TTF_CloseFont(font);
	// Quit TTF and SDL
	TTF_Quit();
	SDL_Quit();
}


Timer::Timer()
{
	// Initialize the variables
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

void Timer::start()
{
	// Start the timer
	started = true;
	
	// Unpause the timer
	paused = false;

	// Get the current clock time 
	startTicks =SDL_GetTicks();	
}

void Timer::stop()
{
	// Stop the timer
	started = false;

	// Unpause the timer
	paused = false;

}

int Timer::get_ticks()
{
	// If the timer is running
	if(started == true)
	{
		// If the timer is paused
		if(paused == true)
		{
			// Return the number of ticks when the timer was paused
			return pausedTicks;
		}
		else
		{
			// Return the current time minus the start time
			return SDL_GetTicks() - startTicks;	
		}

	}
	// If the timer isn't running
	return 0;
}

void Timer::pause()
{
	// If the timer is running and isn't already paused
	if((started == true) && (paused == false))
	{
		// Pause the timer
		paused = true;

		// Calculate the paused ticks
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause()
{
	// If the timer is paused
	if(paused == true)
	{
		// Unpaused the timer
		paused = false;

		// Reset the starting ticks
		startTicks = SDL_GetTicks() - pausedTicks;

		// Reset the paused ticks
		pausedTicks = 0;
	}
}

bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
}
