#include <iostream>
#include <string>
#include <sstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

// Constants
const int FRAMES_PER_SECOND = 20;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// Event
SDL_Event event;

// Surfaces
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;

// Font
TTF_Font *font = NULL;
SDL_Color textColor = {0, 0, 0};

// Function Prototypes
bool init();
SDL_Surface *load_image(std::string filename);
bool load_files();
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL);
void clean_up();

// Timer class
class Timer
{
public:
	Timer();
	void start();
	void stop();
	void pause();
	void unpause();
	bool is_started();
	bool is_paused();
	int get_ticks();
private:
	bool paused;
	bool started;
	int pausedTicks;
	int startTicks;
};


int main(int argc, char *args[])
{
	bool quit = false;

	// Keep track of the current frame
	int frame = 0;

	// Whether or not to cap the frame rate
	bool cap = true;

	// The frame rate regulator
	Timer fps;

	if(init() == false)
	{
		return 1;
	}

	if(load_files() == false)
	{
		return 1;
	}

	// Generate the message surfaces
	message = TTF_RenderText_Solid(font, "Testing Frame Rate", textColor);

	// While the user hasn't quit
	while(quit == false)
	{
		// Start the frame timer
		fps.start();

		// While the user hasn't quit
		while(SDL_PollEvent(&event))
		{
			// If a key is pressed
			if(event.type == SDL_KEYDOWN)
			{
				// If enter was pressed
				if(event.key.keysym.sym == SDLK_RETURN)
				{
					//Switch cap
					cap = (!cap);
				}

			}
			// If the user wants to quit
			else if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		// Apply the background
		apply_surface(0, 0, background, screen);

		// Apply the message
		apply_surface((SCREEN_WIDTH - message->w)/2, ((SCREEN_HEIGHT + message->h *2) / FRAMES_PER_SECOND)*(frame%FRAMES_PER_SECOND)-message->h, message, screen);

		// Update the screen
		if(SDL_Flip(screen) == -1)
		{
			return 1;
		}

		// Increment the frame counter
		frame++;

		// If we want to cap the frame rate
		if((cap == true) && (fps.get_ticks() < 1000 / FRAMES_PER_SECOND))
		{
			// Sleeping the remaining frame time
			SDL_Delay((1000/FRAMES_PER_SECOND) - fps.get_ticks());
		}
	}
	clean_up();
	return 0;
}

// Function Definitions
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

	SDL_WM_SetCaption("Frame Limit", NULL);
	return true;
}

SDL_Surface *load_image(std::string filename)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

	loadedImage = IMG_Load(filename.c_str());
	if(loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
		if(optimizedImage != NULL)
		{
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}
	return optimizedImage;
}
bool load_files()
{
	background = load_image("background.png");
	if(background == NULL)
	{
		return false;
	}
	font = TTF_OpenFont("lazy.ttf", 24);
	if(font == NULL)
	{
		return false;
	}
	return true;
}
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}
void clean_up()
{
	SDL_FreeSurface(background);
	SDL_FreeSurface(message);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}


Timer::Timer()
{
	paused = false;
	started = false;
	pausedTicks = 0;
	startTicks = 0;
}

void Timer::start()
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}

void Timer::stop()
{
	started = false;
	paused = false;
}

int Timer::get_ticks()
{
	if(started == true)
	{
		if(paused == true)
		{
			return pausedTicks;
		}
		else
		{
			return SDL_GetTicks() - startTicks;
		}
	}
	return 0;
}

void Timer::unpause()
{
	if(paused == true)
	{
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;
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

void Timer::pause()
{
	if((started == true)&&(paused == false))
	{
		paused = true;
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

