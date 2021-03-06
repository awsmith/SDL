// Headers
#include <iostream>
#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int DOT_HEIGHT = 20;
const int DOT_WIDTH = 20;

const int FRAMES_PER_SECOND = 20;

// Prototypes
bool init();
SDL_Surface *load_image(std::string filename);
bool load_files();
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL);
void clean_up();
bool check_collision(std::vector<SDL_Rect> &A, std::vector<SDL_Rect> &B);

// Surfaces
SDL_Surface *screen = NULL;
SDL_Surface *dot = NULL;


// The wall
SDL_Rect wall;

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
	Dot(int x, int y, int z);
	void handle_input();
	// Moves the dot
	void move(std::vector<SDL_Rect> &rects);
	// Shows the dot on the screen
	void show();
	// Gets the collision boxes
	std::vector<SDL_Rect> &get_rects();
private:
	// The offsets of the dot
	int x, y;
	// Choose controls. 0 = w, a, s, d, 1 = up, left, down, right 
	int keySet;
	// The collision boxes of the dot
	std::vector<SDL_Rect> box;
	// The velocity of the dot
	int xVel, yVel;
	// Moves the collision boxes relative to the dot's offset
	void shift_boxes();
};

int main()
{
	Timer fps;
	
	Dot myDot(0, 0, 1);
	Dot otherDot(20, 20, 2);

	bool quit = false;

	if(init() == false)
	{
		return 1;
	}

	if(load_files() == false)
	{
		return 2;
	}

	while(quit == false)
	{
		// Start the frame timer
		fps.start();

		while(SDL_PollEvent(&event))
		{
			// Handle events for the dot
			myDot.handle_input();
			otherDot.handle_input();
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		myDot.move(otherDot.get_rects());
		otherDot.move(myDot.get_rects());

		// Fill the screen white
		SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

		// Show the wall
		SDL_FillRect(screen, &wall, SDL_MapRGB(screen->format, 0x77, 0x77, 0x77));

		// Show the square on the screen
		myDot.show();
		otherDot.show();

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


bool check_collision(std::vector<SDL_Rect> &A, std::vector<SDL_Rect> &B )
{
	// The side of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;
	int Abox, Bbox;

	// Go through the A boxes
	for(Abox = 0; Abox < A.size(); Abox++)
	{
		leftA = A[Abox].x;
		rightA = A[Abox].x + A[Abox].w;
		topA = A[Abox].y;
		bottomA = A[Abox].y + A[Abox].h;

		// Go through the B boxes
		for(Bbox = 0; Bbox < B.size(); Bbox++)
		{
			leftB = B[Bbox].x;
			rightB = B[Bbox].x + B[Bbox].w;
			topB = B[Bbox].y;
			bottomB = B[Bbox].y + B[Bbox].h;

			if(((bottomA <= topB) || (topA >= bottomB) || (rightA <= leftB) || (leftA >= rightB)) == false)
			{
				// A collision is detected!
				return true;
			}
		}
	}
	// If neither set of collision boxes touched
	return false;
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

Dot::Dot(int x, int y, int z)
{
	// Initialize the offsets
	this->x = x;
	this->y = y;

	// Initialize the velocity
	xVel = 0;
	yVel = 0;

	// Create the needed SDL_Rects
	box.resize(11);

	// Initialize the collision boxes' width and height
	box[0].w = 6;
	box[0].h = 1;

	box[1].w = 10;
	box[1].h = 1;

	box[2].w = 14;
	box[2].h = 1;

	box[3].w = 16;
	box[3].h = 2;

	box[4].w = 18;
	box[4].h = 2;

	box[5].w = 20;
	box[5].h = 6;

	box[6].w = 18;
	box[6].w = 2;

	box[7].w = 16;
	box[7].h = 2;

	box[8].w = 14;
	box[8].h = 1;

	box[9].w = 10;
	box[9].h = 1;

	box[10].w = 6;
	box[10].h = 1;

	// Move the collision boxes to their proper spot
	shift_boxes();

	// Set the movement keys
	keySet = z;
}

void Dot::shift_boxes()
{
	// The row offset
	int r = 0;
	int set;

	// Go through the dot's collision boxes
	for(set = 0; set < box.size(); set++)
	{
		// Center the collision box
		box[set].x = x + (DOT_WIDTH - box[set].w)/2;

		// Set the collision box at its row offset
		box[set].y = y + r;

		// Move the row offset down the hieght of the collision box
		r += box[set].h;
	}
}

void Dot::handle_input()
{
	if(keySet == 1)
	{
		// If a key was pressed
		if(event.type == SDL_KEYDOWN)
		{
			// Adjust the velocity
			switch(event.key.keysym.sym)
			{
			case SDLK_UP: yVel -= 4; break;
			case SDLK_DOWN: yVel += 4; break;
			case SDLK_LEFT: xVel -= 4; break;
			case SDLK_RIGHT: xVel += 4; break;
			}
		}

		// If a key was released
		else if(event.type == SDL_KEYUP)
		{
			// Adjust the velocity
			switch(event.key.keysym.sym)
			{
				case SDLK_UP: yVel += 4; break;
				case SDLK_DOWN: yVel -= 4; break;
				case SDLK_LEFT: xVel += 4; break;
				case SDLK_RIGHT: xVel -= 4; break;
			}
		}
	}
	else
	{
			// If a key was pressed
		if(event.type == SDL_KEYDOWN)
		{
			// Adjust the velocity
			switch(event.key.keysym.sym)
			{
			case SDLK_w: yVel -= 4; break;
			case SDLK_s: yVel += 4; break;
			case SDLK_a: xVel -= 4; break;
			case SDLK_d: xVel += 4; break;
			}
		}

		// If a key was released
		else if(event.type == SDL_KEYUP)
		{
			// Adjust the velocity
			switch(event.key.keysym.sym)
			{
				case SDLK_w: yVel += 4; break;
				case SDLK_s: yVel -= 4; break;
				case SDLK_a: xVel += 4; break;
				case SDLK_d: xVel -= 4; break;
			}
		}
	}
}

void Dot::move(std::vector<SDL_Rect> &rects)
{
	// Move the dot left or right
	x += xVel;

	// Move the collision boxes
	shift_boxes();

	// If the dot went too far to the left or right or has collided with the other dot
	if((x < 0) || (x + DOT_WIDTH > SCREEN_WIDTH) || (check_collision(box, rects)))
	{
		// Move back
		x -= xVel;
		shift_boxes();
	}

	// Move the dot up or down
	y += yVel;

	// Move the collision boxes
	shift_boxes();

	// If the dot went too far up or down or has collided with the other dot
	if((y < 0) || (y + DOT_HEIGHT > SCREEN_HEIGHT) || (check_collision(box, rects)))
	{
		// Move back
		y -= yVel;
		shift_boxes();
	}
}

void Dot::show()
{
	// Show the dot
	apply_surface(x, y, dot, screen);
}

std::vector<SDL_Rect> &Dot::get_rects()
{
	// Retrieve the collision boxes
	return box;
}
