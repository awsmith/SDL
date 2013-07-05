// Headers
#include <iostream>
#include <string>
#include <fstream>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int DOT_HEIGHT = 40;
const int DOT_WIDTH = 37;
const int FRAMES_PER_SECOND = 20;

// Surfaces
SDL_Surface *screen = NULL;
SDL_Surface *dot = NULL;
Uint32 bg;

// Events
SDL_Event event;

// Classes
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

class Dot
{
	public:
		// Default constructor
		Dot();
		// Takes the key presses and adjusts the dot's velocity
		void handle_input();
		// Move the dot
		void move();
		// Shows the dot on the screen
		void show();
		// Set the dot's x/y offsets
		void set_x(int X);
		void set_y(int Y);
		// Get the dot's x/y offsets
		int get_x();
		int get_y();

	private:
		// The X and Y offsets of the dot
		int x, y;
		// The velocity of the dot
		int xVel, yVel;
};

// Prototypes
bool init();
SDL_Surface *load_image(std::string filename);
bool load_files(Dot &thisDot, Uint32 &bg);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL);
void clean_up(Dot &thisDot, Uint32 &bg);

int main(int argc, char *args[])
{
	// Class instances
	Timer fps;
	Dot myDot;
	
	// Quit flag
	bool quit = false;
	
	// Initialize SDL
	if(init() == false)
	{
		return 1;
	}

	// The background color
	Uint32 background = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	
	// Load files and set level
	if(load_files(myDot, background) == false)
	{
		return 2;
	}

	// Whilte flag has not been set
	while(quit == false)
	{
		// Start the frame timer
		fps.start();

		while(SDL_PollEvent(&event))
		{
			// Handle events for the dot
			myDot.handle_input();
			// If the user has pressed a key
			if(event.type == SDL_KEYDOWN)
			{
				// Change the background according to key press
				switch(event.key.keysym.sym)
				{
					// Set white
					case SDLK_1: background = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF); break;
					// Set Red
					case SDLK_2: background = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00); break;
					// Set Green
					case SDLK_3: background = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00); break;
					// Set Blue
					case SDLK_4: background = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF); break;
				}
			}
			// If user has X'd out the window
			if(event.type == SDL_QUIT)
			{
				// Set quit flag
				quit = true;
			}
		}

		// Move the dot base on user input
		myDot.move();

		// Fill the screen with the background color
		SDL_FillRect( screen, &screen->clip_rect, background);

		// Show the dot on the screen
		myDot.show();

		// Update the screen
		if(SDL_Flip(screen) == -1)
		{
			return 3;
		}

		// Cap the frame rate
		if(fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
		{
			SDL_Delay((1000/FRAMES_PER_SECOND) - fps.get_ticks());
		}
	}
	// Clean up surfaces, save game info and close SDL
	clean_up(myDot, background);
	return 0;
}

bool init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	
	if(screen == NULL)
	{
		return false;
	}
	SDL_WM_SetCaption("Motion", NULL);
	
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
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format,0, 0xFF, 0xFF));
		}
	}
	return optimizedImage;
}

bool load_files(Dot &thisDot, Uint32 &bg)
{
	dot = load_image("dot.png");
	
	if(dot == NULL)
	{
		return false;
	}
	// Open a file for reading
	std::ifstream load("game_save");
	// If the file loaded
	if(load != NULL)
	{
		// The offset
		int offset;
		// The level name
		std::string level;
		// Set the x offset
		load >> offset;
		thisDot.set_x(offset);
		// Set the y offset
		load >> offset;
		thisDot.set_y(offset);
		// If the x offset is invalid
		if((thisDot.get_x() < 0) || (thisDot.get_x() > SCREEN_WIDTH - DOT_WIDTH))
		{
			return false;
		}
		// If the y offset is invalid
		if((thisDot.get_y() < 0) || (thisDot.get_y() > SCREEN_HEIGHT - DOT_HEIGHT))
		{
	
			return false;
		}
		// Skip past the end of the line
		load.ignore();
		// Get the next line
		getline(load, level);
		// If an error ocurred while trying to load data
		if(load.fail() == true)
		{
			return false;
		}
		// If the level was white
		if(level == "White Level")
		{
			// Set the background color
			bg = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
		}
		// If the level was red
		else if(level == "Red Level")
		{
			bg = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
		}
		// If the level was green
		else if(level == "Green Level")
		{
			bg = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
		}
		// If the level was blue
		else if(level == "Blue Level")
		{
			bg = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
		}
		// Close the file
		load.close();
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

void clean_up(Dot &thisDot, Uint32 &bg)
{
	// Free the surface
	SDL_FreeSurface(dot);
	// Open a file for writing
	std::ofstream save("game_save");
	// Write the offsets to the file
	save << thisDot.get_x();
	save << " ";
	save << thisDot.get_y();
	save << "\n";
	// The RGB values from the background
	Uint8 r, g, b;
	// Get the RGB values from the background color
	SDL_GetRGB(bg, screen->format, &r, &g, &b);
	// If the background was white
	if((r == 0xFF) && (g == 0xFF) && (b == 0xFF))
	{
		// Write level type to file
		save << "White Level";
	}
	else if( r == 0xFF)
	{
		save << "Red Level";
	}
	else if( g == 0xFF)
	{
		save << "Green Level";
	}
	else if (b == 0xFF)
	{
		save << "Blue Level";
	}
	// Close the file
	save.close();
	SDL_Quit();
}

Dot::Dot()
{
	// Initialize the offsets
	x = 0;
	y = 0;

	// Initialize the velocity
	xVel = 0;
	yVel = 0;
}

void Dot::handle_input()
{
	// If a key was pressed
	if(event.type == SDL_KEYDOWN)
	{
		// Adjust the velocity
		switch(event.key.keysym.sym)
		{
			case SDLK_UP: yVel -= DOT_HEIGHT / 4; break;
			case SDLK_DOWN: yVel += DOT_HEIGHT / 4; break;
			case SDLK_LEFT: xVel -= DOT_WIDTH / 4; break;
			case SDLK_RIGHT: xVel += DOT_WIDTH / 4; break;
		}
	}

	// If a key was pressed
	else if(event.type == SDL_KEYUP)
	{
		// Adjust the velocity
		switch(event.key.keysym.sym)
		{
		case SDLK_UP: yVel += DOT_HEIGHT / 4; break;
		case SDLK_DOWN: yVel -= DOT_HEIGHT / 4; break;
		case SDLK_LEFT: xVel += DOT_WIDTH / 4; break;
		case SDLK_RIGHT: xVel -= DOT_WIDTH /4; break;
		}
	}
}

void Dot::move()
{
	// Move the dot left or right
	x += xVel;

	// If the dot went too far too the left or right
	if( (x < 0) || (x + DOT_WIDTH > SCREEN_WIDTH))
	{
		// Move back
		x -= xVel;
	}

	// Move the dot up or down
	y += yVel;

	// If the dot went too far up or down
	if( (y < 0) || (y + DOT_HEIGHT > SCREEN_HEIGHT) )
	{
		// Move back
		y -= yVel;
	}
}

void Dot::show()
{
	// Show the dot
	apply_surface(x, y, dot, screen);
}
void Dot::set_x(int X)
{
	x = X;
}
void Dot::set_y(int Y)
{
	y = Y;
}

int Dot::get_x()
{
	return x;
}
int Dot::get_y()
{
	return y;
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
