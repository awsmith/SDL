// Headers
#include <iostream>
#include <string>
#include <sstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// Prototypes
bool init();
bool load_files();
SDL_Surface *load_image(std::string filename);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL);
void clean_up();

// Classes
class Timer
{
public:
  Timer();
	bool is_paused();
	bool is_started();
	int get_ticks();
	void start();
	void stop();
	void pause();
	void unpause();
private:
	bool started;
	bool paused;
	int pausedTicks;
	int startTicks;

};

// Events
SDL_Event event;

// Surfaces
SDL_Surface *background;
SDL_Surface *screen;
SDL_Surface *message;

// Font
TTF_Font *font = NULL;
SDL_Color textColor = {0, 0, 0};



int main(int argc, char *args[])
{
	// Flag for user to quit
	bool quit = false;
	// Keep track of the frame count
	int frame = 0;

	if(init() == false)
	{
		return 1;
	}

	if(load_files() == false)
	{
		return 1;
	}

	// Timer used to calculate the fps
	Timer fps;

	// Timer used to update the caption
	Timer update;

	// Start timers
	update.start();
	fps.start();
	
	message = TTF_RenderText_Solid(font, "Testing average frames per second", textColor);

	while(quit == false)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}
		// Apply surfaces
		apply_surface(0, 0, background, screen);
		apply_surface((SCREEN_WIDTH - message->w)/2, SCREEN_HEIGHT / 2, message, screen);
		// Update the screen
		if(SDL_Flip(screen) == -1)
		{
			return 1;
		}

		// Increment the frame counter
		frame++;

		// If a second has passed since the caption was last updated
		if(update.get_ticks() > 1000)
		{
			// The frame rate as a string
			std::stringstream caption;

			// Calculate the frames per second and create the string
			caption << "Average Frames Per Second: " << frame / (fps.get_ticks()/1000.f);

			// Reset the caption
			SDL_WM_SetCaption(caption.str().c_str(), NULL);

			// Restart the update timer
			update.start();
		}
	}
	clean_up();
	return 0;
}

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

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,SDL_SWSURFACE);
	if(screen == NULL)
	{
		return false;
	}
	SDL_WM_SetCaption("Frame Rate Calc", NULL);

	return true;
}

SDL_Surface *load_image(std::string filename)
{
	// Pointers to hold image for optimization
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

	// Load image from passed in string
	loadedImage = IMG_Load(filename.c_str());

	// Optimize if image was loaded
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
	startTicks = 0;
	pausedTicks = 0;
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

bool Timer::is_paused()
{
	return paused;
}

bool Timer::is_started()
{
	return started;
}

void Timer::pause()
{
	if((started == true)&&(paused == false))
	{
		paused = true;
		pausedTicks = SDL_GetTicks() - startTicks;
	}
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
