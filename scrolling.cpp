// Header files
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <iostream> 

// The screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// The dot dimensions
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;

// The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

// The frame rate
const int FRAMES_PER_SECOND = 20;

// The surfaces
SDL_Surface *dot = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;

// The event structure
SDL_Event event;

// The camera
SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

// The dot
class Dot
{
	public:
		// Initializes the variables
		Dot();

		// Takes the key presses and adjusts the dot's velocity
		void handle_input();

		// Moves the dot
		void move();

		// Shows the dot on the screen
		void show();

		// Sets the camera over the dot
		void set_camera();
		
	private:
		// The x and y offsets of the dot
		int x, y;

		// The velocity of the dot
		int xVel, yVel;
};

// The timer
class Timer
{
	public:
		Timer();
		void start();
		void stop();
		void pause();
		void unpause();

		int get_ticks();

		bool is_started();
		bool is_paused();

	private:
		int startTicks;
		int pausedTicks;

		bool paused;
		bool started;
};


// Function Prototypes
bool init();
SDL_Surface *load_image(std::string file);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL);
bool load_files();
void clean_up();

int main()
{
	// Quit flag
	bool quit = false;

	// Class instances
	Dot myDot;
	Timer fps;

	if(init() == false)
	{
		return 1;
	}

	if(load_files() == false)
	{
		return 1;
	}

	// While the user has not quit
	while(quit == false)
	{
		// Start the timer
		fps.start();

		// While the there's events to handle
		while(SDL_PollEvent(&event))
		{
			// Handle the events for the dot
			myDot.handle_input();
			
			// If the user has Xed out of the window
			if(event.type == SDL_QUIT)
			{
				// Quit the program
				quit = true;
			}
		}

		// Move the dot	
		myDot.move();

		// Set the camera
		myDot.set_camera();

		// Show the background
		apply_surface(0, 0, background, screen, &camera);

		// Show the dot on the screen
		myDot.show();
		
		// Update the screen
		if(SDL_Flip(screen) == -1)
		{
			return 1;
		}

		// Cap the frame rate
		if(fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
		{
			SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
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

	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	if(screen ==  NULL)
	{
		return false;
	}

	SDL_WM_SetCaption("Scrolling", NULL);
	
	return true;
}

SDL_Surface *load_image(std::string file)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

	loadedImage = IMG_Load(file.c_str());
	if(loadedImage != NULL)
	{
		optimizedImage = SDL_DisplayFormat(loadedImage);
		SDL_FreeSurface(loadedImage);
	}
	if(optimizedImage != NULL)
	{
		SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
	}

	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}
	
bool load_files()
{
	dot = load_image("dot.bmp");
	if(dot == NULL)
	{
		return false;
	}
	background = load_image("bg.png");
	
	if(background == NULL)
	{	
		return false;
	}

	return true;
}

void clean_up()
{
	SDL_FreeSurface(dot);
	SDL_FreeSurface(background);
	SDL_Quit();
}
Dot::Dot()
{
	x = 0;
	y = 0;

	xVel = 0;
	yVel = 0;
}

void Dot::handle_input()
{
	if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_UP: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel += DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel -= DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel += DOT_WIDTH / 2; break;
		}
	}
	if(event.type == SDL_KEYUP)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_UP: yVel += DOT_HEIGHT / 2; break;
			case SDLK_DOWN: yVel -= DOT_HEIGHT / 2; break;
			case SDLK_LEFT: xVel += DOT_WIDTH / 2; break;
			case SDLK_RIGHT: xVel -= DOT_WIDTH / 2; break;
		}
	}
}

void Dot::move()
{
	// move the dot left or right
	x += xVel;

	// if the dot went too far to the left or right
	if((x < 0) || (x + DOT_WIDTH > LEVEL_WIDTH))
	{
		// Move back!
		x -= xVel;
	}

	// Move the dot up or down
	y += yVel;

	// If the dot went too far up or down
	if((y < 0) || (y + DOT_HEIGHT > LEVEL_HEIGHT))
	{
		// Move back!
		y -= yVel;
	}
}

void Dot::set_camera()
{
	// Center the camera over the dot
	camera.x = (x + DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (y + DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	// Keep the camera in bounds
	if(camera.x < 0)
	{
		camera.x = 0;
	}
	if(camera.y < 0)
	{
		camera.y = 0;
	}
	if(camera.x > LEVEL_WIDTH - camera.w)
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if(camera.y > LEVEL_HEIGHT - camera.h)
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Dot::show()
{
	// Show the dot
	apply_surface(x - camera.x, y -camera.y, dot, screen);
}

Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;
	started = false;
	paused = false;
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

void Timer::pause()
{
	if((started == true) && (paused == false))
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

bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
}
