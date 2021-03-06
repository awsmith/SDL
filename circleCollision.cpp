#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "SDL.h"
#include "SDL_image.h"

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int DOT_WIDTH = 20;
const int DOT_HEIGHT = 20;
const int FRAMES_PER_SECOND = 20;

// Surfaces
SDL_Surface *screen = NULL;
SDL_Surface *dot = NULL;

// Events
SDL_Event event;

// Structs
struct Circle
{
        // Coordinates
	int x, y;

	// Radius
	int r;
};

// Classes
class Dot
{
	public:
	// Constructor
	Dot();
	
	// Take key presses and adjust the dot's velocity
	void handle_input();

	// Moves the dot
	void move(std::vector<SDL_Rect> &rects, Circle &circle);

	// Shows the dot on the screen
	void show();

	private:
	// The area of the dot
	Circle c;

	// The veolicty of the dot
	int xVel, yVel;
};

class Timer
{
	public:
		Timer();
		bool is_started();
		bool is_paused();
		int get_ticks();
		void start();
		void pause();
		void stop();
		void unpause();

	private:
		int startTicks;
		int pausedTicks;
		bool paused;
		bool started;
};

// Prototypes
bool init();
SDL_Surface *load_image(std::string filename);
bool load_files();
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface * destination, SDL_Rect* clip = NULL);
void clean_up();
double distance(int x1, int y1, int x2, int y2);
bool check_collision(Circle &A, Circle &B);
bool check_collision(Circle &A, std::vector<SDL_Rect> &B);

int main(int argc, char **args)
{
	if(init() == false)
	{
		return 1;
	}

	if(load_files() == false)
	{
		return 1;
	}

	// Flag
	bool quit = false;

	// Make the shapes
	std::vector<SDL_Rect> box(1);
	Circle otherDot;
	Dot myDot;

	// Set the shape attributes
	box[0].x = 60;
	box[0].y = 60;
	box[0].w = 20;
	box[0].h = 20;

	otherDot.x = 30;
	otherDot.y = 30;
	otherDot.r = DOT_WIDTH / 2;
	
	// Create timer for frame limit
	Timer fps;

	// While the user hasn't quit
	while(quit == false)
	{
		// Start timer
		fps.start();

		// While there are events to handle
		while(SDL_PollEvent(&event))
		{
			// Handle input for dot
			myDot.handle_input();

			// Quit window if user hits "x"
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		// Move the dot
		myDot.move(box, otherDot);
	
		// Fill the screen white
		SDL_FillRect(screen, &screen->clip_rect,  SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

		// Show the box
		SDL_FillRect(screen, &box[0], SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

		// Show moving dot
		myDot.show();

		// Show stationary dot
		apply_surface(otherDot.x - otherDot.r, otherDot.y - otherDot.r, dot, screen);

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

// General Function Definitions
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
	SDL_WM_SetCaption("Circle Collision", NULL);
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
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage ->format, 0, 0xFF, 0xFF));
		}
	}
	return optimizedImage;
}

bool load_files()
{
	dot = load_image("dot.bmp");
	
	if(dot == NULL)
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
	SDL_FreeSurface(dot);
	SDL_Quit();
}

double distance(int x1, int y1, int x2, int y2)
{
	// Return the distance between the two points
	return sqrt(pow(x2 - x1, 2.) + pow( y2 - y1, 2.));
}

// Check collision for two instances of circle
bool check_collision(Circle &A, Circle &B)
{
	// If the distance between the centers of the circles is less than the sum of their radii
	if(distance(A.x, A.y, B.x, B.y) < (A.r + B.r))
	{
		// The circles have collided
		return true;
	}

	// If not
	return false;
}

// Check collision for for instance of cicle vs 
bool check_collision(Circle &A, std::vector<SDL_Rect> &B)
{
	// Closest point on collision box
	int cX, cY;

	// Go through the boxes
	for(int Bbox = 0; Bbox < ((int)B.size()); Bbox++)
	{
		// Find the closest X offset
		if(A.x < B[Bbox].x)
		{
			cX = B[Bbox].x;
		}
		else if(A.x > B[Bbox].x + B[Bbox].h)
		{
			cX = B[Bbox].x + B[Bbox].h;
		}
		else
		{
			cX = A.x;
		}
		// Find the closest Y offset
		if(A.y < B[Bbox].y)
		{
			cY = B[Bbox].y;
		}
		else if(A.y > B[Bbox].y + B[Bbox].h)
		{
			cY = B[Bbox].y + B[Bbox].h;
		}
		else
		{
			cY = A.y;
		}
		// If the closest point is inside the circle
		if(distance(A.x, A.y, cX, cY) < A.r)
		{
			// This box and the circle have collided
			return true;
		}
	}
	
	// If the shapes have not collided
	return false;
}

// Timer Class Function Definitions
Timer::Timer()
{
	started = false;
	paused = false;
	pausedTicks = 0;
	startTicks = 0;
}

bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
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

void Timer::start()
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}

void Timer::pause()
{
	if(started == true && paused == false)
	{
		paused = true;
	}
	pausedTicks = SDL_GetTicks() - startTicks;
}

void Timer::stop()
{
	started = false;
	paused = false;
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

// Dot Class Function Definitions
Dot::Dot()
{
	c.x = DOT_WIDTH / 2;
	c.y = DOT_WIDTH / 2;
	c.r = DOT_WIDTH / 2;
	// Dot will not move if velocities are not initialized to 0
	xVel = 0;
	yVel = 0;
}

void Dot::handle_input()
{
	if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
		case SDLK_UP: yVel -= 1; break;
		case SDLK_DOWN: yVel += 1; break;
		case SDLK_LEFT: xVel -= 1; break;
		case SDLK_RIGHT: xVel += 1; break;
		}
	}
	if(event.type == SDL_KEYUP)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_UP: yVel += 1; break;
			case SDLK_DOWN: yVel -= 1; break;
			case SDLK_LEFT: xVel += 1; break;
			case SDLK_RIGHT: xVel -= 1; break;
		}
	}
}

void Dot::move(std::vector<SDL_Rect> &rects, Circle &circle)
{
	// Move the dot on the x-axis
	c.x += xVel;

	// If the dot went too far or has collided
	if((c.x - DOT_WIDTH / 2 < 0) || (c.x + DOT_WIDTH / 2 > SCREEN_WIDTH) || (check_collision(c, rects)) || (check_collision(c, circle)))
	{
		// Move back
		c.x -= xVel;
	}

	// Move the dot on the y-axis
	c.y += yVel;

	if((c.y - DOT_WIDTH / 2 < 0) || (c.y + DOT_WIDTH / 2 > SCREEN_HEIGHT) || (check_collision(c, rects)) || (check_collision(c, circle)))
	{
		// Move back
		c.y -= yVel;
	}
}

void Dot::show()
{
	apply_surface(c.x - c.r, c.y - c.r, dot, screen);
}
