// SDL Tutorial 9: Mouse Events
// Added function prototypes and moved function definitions below main

#include <iostream>
#include "SDL.h"
#include "SDL_Image.h"
#include "SDL_ttf.h"
#include <string>

// Constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

// Surfaces
SDL_Surface* background = NULL;
SDL_Surface* screen = NULL;
SDL_Surface* buttonSheet = NULL;

// Events
SDL_Event event;

// Classes
class Button
{
  public:
		// Constructor
		Button(int x, int y, int w, int h);

		// Handles events and set the button's sprite region
		void handle_events();

		// Shows the button on the screen
		void show();

	private:
		// Attributes of the button
		SDL_Rect box;

		// The part of the button sprite sheet that will be shown
		SDL_Rect* clip;
};

// Function Prototypes
void set_clips();
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);
bool init();
bool load_files();
void clean_up();
SDL_Surface* load_image(std::string filename);

// Array of four rectangles, hold the offsets
SDL_Rect clips[4];

int main(int argc, char* args[])
{
	// Quit flag
	bool quit = false;

	// Initialize
	if(init() == false)
	{
		return 1;
	}

	// Load files
	if(load_files() == false)
	{
		return 1;
	}
	
	set_clips();

	Button myButton(170, 120, 320, 240);

	// While the user has not quit
	while(quit == false)
	{
		// While there are events to handle
		if(SDL_PollEvent (&event))
		{
			myButton.handle_events();

			// If the users Xed out the window
			if(event.type == SDL_QUIT)
			{
				// Quit the program
				quit = true;
			}
		}
		// Fill the screen white
		SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));

		// Show the button
		myButton.show();

		// Update the screen
		if(SDL_Flip(screen) == -1)
		{
			return 1;
		}
		
	}
	// Free surfaces
	clean_up();
	return 0;
}

void set_clips()
{
	// Clip the sprite sheet
	clips[CLIP_MOUSEOVER].x = 0;
	clips[CLIP_MOUSEOVER].y = 0;
	clips[CLIP_MOUSEOVER].w = 320;
	clips[CLIP_MOUSEOVER].h = 240;

	clips[CLIP_MOUSEOUT].x = 320;
	clips[CLIP_MOUSEOUT].y = 0;
	clips[CLIP_MOUSEOUT].w = 320;
	clips[CLIP_MOUSEOUT].h = 240;

	clips[CLIP_MOUSEDOWN].x = 0;
	clips[CLIP_MOUSEDOWN].y = 240;
	clips[CLIP_MOUSEDOWN].w = 320;
	clips[CLIP_MOUSEDOWN].h = 240;

	clips[CLIP_MOUSEUP].x = 320;
	clips[CLIP_MOUSEUP].y = 240;
	clips[CLIP_MOUSEUP].w = 320;
	clips[CLIP_MOUSEUP].h = 240;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
{
	// Hold offsets
	SDL_Rect offset;

	// Set offsets
	offset.x = x;
	offset.y = y;

	// SDL_BlitSurface(image to blit, where to blit from image, blit to surface, where on surface)
	SDL_BlitSurface(source, clip, destination, &offset);
}

SDL_Surface* load_image(std::string filename)
{
	//Imagee that is loaded
	SDL_Surface* loadedImage = NULL;

	// The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	// Load the image
	loadedImage = IMG_Load(filename.c_str());
	
	// If the image loaded
	if(loadedImage != NULL)
	{
		// Create an optimized image
		optimizedImage = SDL_DisplayFormat(loadedImage);

		// Free the old image
		SDL_FreeSurface(loadedImage);

		// If optimized
		if(optimizedImage != NULL)
		{
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}
		// Return optimized image
		return optimizedImage;
}

bool init()
{
	// Initialize subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	
	// Set up the screen
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	// If there was an error setting up
	if(screen == NULL)
	{
		return 1;
	}

	// Set the window caption
	SDL_WM_SetCaption("Mouse Event", NULL);

	// If initialized
	return true;
}

bool load_files()
{
	// Load the background image
	background = load_image("background.png");
	buttonSheet = load_image("button.png");

	// If not loaded
	if(background == NULL || buttonSheet == NULL)
	{
		return false;
	}
	return true;
}

void clean_up()
{
	// Free surfaces
	SDL_FreeSurface(background);
	SDL_FreeSurface(buttonSheet);
	// Quit SDL_ttf
	TTF_Quit();
	// Quit
	SDL_Quit();
}

Button::Button(int x, int y, int w, int h)
{
	// Set the button's attributes
	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;

	// Set the default sprite
	clip = &clips[CLIP_MOUSEOUT];
}

void Button::handle_events()
{
	// The mouse offsets
	int x = 0, y = 0;

	// If the mouse moved
	if(event.type == SDL_MOUSEMOTION)
	{
		// Get the mouse offsets
		x = event.motion.x;
		y = event.motion.y;
	}

	// If the mouse is over the button
	if((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
	{
		// Set the button sprite
		clip = &clips[CLIP_MOUSEOVER];
	}
	// If not
	else
	{
		// Set the button sprite
		clip = &clips[CLIP_MOUSEOUT];
	}

	// If a mouse button was pressed
	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		// If the left mouse button was pressed
		if(event.button.button == SDL_BUTTON_LEFT)
		{
			// Get the mouse offsets
			x = event.button.x;
			y = event.button.y;

			// If the mouse is over the button
			if((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
			{
				// Set the button sprite
				clip = &clips[CLIP_MOUSEDOWN];
			}
		}
	}

	// If a mouse button was released
	if(event.type == SDL_MOUSEBUTTONUP)
	{
		// If the left mouse button was released
		if(event.button.button == SDL_BUTTON_LEFT)
		{
			// Get the mouse offsets
			x = event.button.x;
			y = event.button.y;

			// If the mouse is over the button
			if((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
			{
				// Set the button sprite
				clip = &clips[CLIP_MOUSEUP];
			}
		}
	}
}

void Button::show()
{
	// Show the button
	apply_surface(box.x, box.y, buttonSheet, screen, clip);
}
