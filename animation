// WIP

#include <string>
#include "SDL.h"
#include "SDL_Image.h"

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

const int FOO_WIDTH = 64;
const int FOO_HEIGHT = 205;

const int FOO_LEFT = 1;
const int FOO_RIGHT = 0;

const int FRAMES_PER_SECOND = 10;

// Events
SDL_Event event;

// Rects
SDL_Rect clipsRight[4];
SDL_Rect clipsLeft[4];

// Surfaces
SDL_Surface *screen = NULL;
SDL_Surface *foo = NULL;

// Prototypes
SDL_Surface *load_image(std::string filename);
void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip = NULL);
void set_clips() ;
bool init();
bool load_files();
void clean_up();

// Classes
class Foo 
{ 
 	private: 
	    	// The offset 
	    	int offSet; 
	    
	    	// Rate of movement 
	    	int velocity; 
	    
	   	// Its current frame 
	    	int frame; 
	    
	  	// Its animation status 
	    	int status; 
	
	    	// Jump
	    	int yAxis;

  	public:
    
	   	// Default constructor
	    	Foo(); 
	    
	    	// Handles input 
	    	void handle_events(); 
	    
	    	// Moves the stick figure 
	    	void move(); 
	    
	    	// Shows the stick figure 
	    	void show(); 
};

class Timer
{
	private:
  		int pausedTicks;
		int startTicks;
		bool started;
		bool paused;
	public:
		Timer();
		bool is_started();
		bool is_paused();
		int get_ticks();
		void start();
		void stop();
		void pause();
		void unpause();
};

int main(int argc, char *args[])
{
	bool quit = false;

	if(init() == false)
	{
		return 1;
	}

	if(load_files() == false)
	{
		return 1;
	}

	// Set the sprite sheet clips
	set_clips();

	// The frame rate regulator
	Timer fps;

	// Make the stick figure
	Foo walk;

	// While the user hasn't quit
	while(quit == false)
	{
		// Start the frame timer
		fps.start();

		// While there's events to handle
		while(SDL_PollEvent(&event))
		{
			// Handle events for the stick figure
			walk.handle_events();

			// If the user has Xed out the window
			if(event.type == SDL_QUIT)
			{
				// Quit the program
				quit = true;
			}
		}

		// Move the stick figure
		walk.move();

		// Fill the screen white
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

		// Show the stick figure on screen
		walk.show();

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


// Functinon Definitions
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

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, clip, destination, &offset);
}

void set_clips() 
{ 
	//Clip the sprites 
	clipsRight[0].x = 0;
	clipsRight[0].y = 0; 
	clipsRight[0].w = FOO_WIDTH; 
	clipsRight[0].h = FOO_HEIGHT; 

	clipsRight[1].x = FOO_WIDTH; 
	clipsRight[1].y = 0; 
	clipsRight[1].w = FOO_WIDTH; 
	clipsRight[1].h = FOO_HEIGHT; 

	clipsRight[2].x = FOO_WIDTH * 2; 
	clipsRight[2].y = 0; 
	clipsRight[2].w = FOO_WIDTH; 
	clipsRight[2].h = FOO_HEIGHT; 
	
	clipsRight[3].x = FOO_WIDTH * 3; 
	clipsRight[3].y = 0; 
	clipsRight[3].w = FOO_WIDTH; 
	clipsRight[3].h = FOO_HEIGHT; 

	clipsLeft[0].x = 0; 
	clipsLeft[0].y = FOO_HEIGHT; 
	clipsLeft[0].w = FOO_WIDTH; 
	clipsLeft[0].h = FOO_HEIGHT; 

	clipsLeft[1].x = FOO_WIDTH; 
	clipsLeft[1].y = FOO_HEIGHT; 
	clipsLeft[1].w = FOO_WIDTH; 
	clipsLeft[1].h = FOO_HEIGHT; 

	clipsLeft[2].x = FOO_WIDTH * 2; 
	clipsLeft[2].y = FOO_HEIGHT;
	clipsLeft[2].w = FOO_WIDTH; 
	clipsLeft[2].h = FOO_HEIGHT;

	clipsLeft[3].x = FOO_WIDTH * 3;
	clipsLeft[3].y = FOO_HEIGHT; 
	clipsLeft[3].w = FOO_WIDTH; 
	clipsLeft[3].h = FOO_HEIGHT; 
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

	// Set caption
	SDL_WM_SetCaption("Animation", NULL);

	return true;
}

bool load_files()
{
	foo = load_image("foo.png");
	if(foo == NULL)
	{
		return false;
	}
	return true;
}

void clean_up()
{
	SDL_FreeSurface(foo);
	SDL_Quit();
}

// Timer class definitions
Timer::Timer()
{
	pausedTicks = 0;
	startTicks = 0;
	started = false;
	paused = false;
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

// Foo class definitions
Foo::Foo()
{
	// Init movement variables
	offSet = 0;
	velocity = 0;
	yAxis = 0;

	// Init animation variables
	frame = 0;
	status = FOO_RIGHT;
}

void Foo::move()
{
	// Move
	offSet += velocity;

	// Keep the stick figure in bounds
	if((offSet < 0) || (offSet + FOO_WIDTH > SCREEN_WIDTH))
	{
		offSet -= velocity;
	}
}

void Foo::show()
{
	// If Foo is moving left
	if(velocity < 0)
	{
		// Set the animation to left
		status = FOO_LEFT;

		// Move to next frame
		frame++;
	}

	// If Foo is moving right
	else if(velocity > 0)
	{
		// Set the animation to right
		status = FOO_RIGHT;

		// Move to next frame
		frame++;
	}

	// If Foo is standing
	else
	{
		// Restart the animation
		frame = 0;
	}

	// Loop the animation
	if(frame >= 4)
	{
		frame = 0;
	}

	// Shows the stick figure
	if(status == FOO_RIGHT)
	{
		apply_surface(offSet, SCREEN_HEIGHT - FOO_HEIGHT - yAxis, foo, screen, &clipsRight[frame]);
		if(yAxis > 0)
		{
			yAxis -= FOO_HEIGHT / 5;
		}
	}
	
	else if(status == FOO_LEFT)
	{
		apply_surface(offSet, SCREEN_HEIGHT - FOO_HEIGHT - yAxis, foo, screen, &clipsLeft[frame]);
		if(yAxis > 0)
		{
			yAxis -= FOO_HEIGHT / 5;
		}
	}
}

void Foo::handle_events()
{
	if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_LEFT: velocity -= FOO_WIDTH / 4; break;
			case SDLK_RIGHT: velocity += FOO_WIDTH /4; break;
			case SDLK_UP: yAxis += FOO_HEIGHT; break;
		}
	}
	else if(event.type == SDL_KEYUP)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_LEFT: velocity += FOO_WIDTH / 4; break;
			case SDLK_RIGHT: velocity -= FOO_WIDTH / 4; break;
		}
	}
}
