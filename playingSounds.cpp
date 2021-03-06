/* SDL Tutorial 11 Playing Sounds.
   2013/04/23
   a_s_
*/

#include <iostream>
#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <string>

// Constants
int const SCREEN_WIDTH = 640;
int const SCREEN_HEIGHT = 480;
int const SCREEN_BPP = 32;

// The surfaces
SDL_Surface *screen = NULL;
SDL_Surface *background = NULL;
SDL_Surface *message = NULL;

// Events
SDL_Event event;

// The Font
TTF_Font *font = NULL;
SDL_Color textColor = {0, 0, 0};

// The music to be played
Mix_Music *music = NULL;

// The sound effects that will be used
Mix_Chunk *scratch = NULL;
Mix_Chunk *high = NULL;
Mix_Chunk *med = NULL;
Mix_Chunk *low = NULL;

// Function prototypes
SDL_Surface *load_image(std::string filename);
void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destinaion);
bool load_files();
bool init();
void clean_up();

int main(int argc, char* args[])
{
  bool quit = false;

	if(init() == false)
	{
		return 1;
	}

	if(load_files() == false)
	{
		return 2;
	}

	// Apply the background
	apply_surface(0, 0, background, screen);

	// Render the text
	message = TTF_RenderText_Solid(font, "Press 1, 2, 3,  or 4 to play a sound effect", textColor);
	
	// If there was an error rendering the text
	if(message == NULL)
	{
		return 3;
	}

	// Show the message on the screen
	apply_surface((SCREEN_WIDTH - message->w)/2, 100, message, screen);

	// Free the message
	SDL_FreeSurface(message);
	
	// Render the text
	message = TTF_RenderText_Solid(font, "Press 9 to play or puase the music", textColor);

	// If there was an error rendering the text
	if(message == NULL)
	{
		return 4;
	}

    //Show the message on the screen
	apply_surface((SCREEN_WIDTH - message->w)/2, 200, message, screen);
	
	// Free the message
	SDL_FreeSurface(message);

	// Render the text
	message = TTF_RenderText_Solid(font, "Press 0 to stop the music", textColor);

	// If there was an error rendering the text
	if(message == NULL)
	{
		return 5;
	}

	// Show the message on the screen
	apply_surface((SCREEN_WIDTH - message->w)/2, 300, message, screen);

	// Free the message
	SDL_FreeSurface(message);

	if(SDL_Flip(screen) == -1)
	{
		return 6;
	}

	// While the user hasn't quit
	while(quit == false)
	{
		// While there's events to handle
		while(SDL_PollEvent( &event))
		{
			// If a key was pressed
			if(event.type == SDL_KEYDOWN)
			{
				// Mix_PlayChannel(which channel, sound effect, how many times) setting -1 means first channel available
				// If 1 was pressed
				if(event.key.keysym.sym == SDLK_1)
				{
					// Play the scratch effect
					if(Mix_PlayChannel(-1, scratch, 0) == -1)
					{
						return 7;
					}
				}
				// If 2 was pressed
				else if(event.key.keysym.sym == SDLK_2)
				{
					// Play the high hit effect
					if(Mix_PlayChannel(-1, high, 0) == -1)
					{
						return 8;
					}
				}
				// If 3 was pressed
				else if(event.key.keysym.sym == SDLK_3)
				{
					// Play the medium hit effect
					if(Mix_PlayChannel(-1, med, 0) == -1)
					{
						return 9;
					}
				}
				// If 4 was pressed
				else if(event.key.keysym.sym == SDLK_4)
				{
					// Play the low hit effect
					if(Mix_PlayChannel(-1, low, 0) == -1)
					{
						return 10;
					}
				}
				// Mix_PlayMusic(music to play, loop times) -1 = play until paused
				// If 9 was pressed
				else if(event.key.keysym.sym == SDLK_9)
				{
					// If there is no music playing
					if(Mix_PlayingMusic() == 0)
					{
						// Play the music
						if(Mix_PlayMusic(music, -1) == -1)
						{
							return 11;
						}
					}
					// If music is being playing
					else
					{
						// If the music is paused
						if(Mix_PausedMusic() == 1)
						{
							// Resume the music
							Mix_ResumeMusic();
						}
						// If the music is playing 
						else
						{
							// Pause the music
							Mix_PauseMusic();
						}
					}
				}
				// If 0 was pressed
				else if(event.key.keysym.sym == SDLK_0)
				{
					// Stop the music
					Mix_HaltMusic();
				}
			}
			// If the user has Xed out the window
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
			
		}
	}
	clean_up();
	return 0;
}

SDL_Surface *load_image(std::string filename)
{
	// SLD_Surface pointers. For loaded image and optimized image to return for use
	SDL_Surface* loadedImage = NULL;
	SDL_Surface* optimizedImage = NULL;

	// Load the image
	loadedImage = IMG_Load(filename.c_str());

	// If image loaded fine
	if(loadedImage != NULL)
	{
		// Create optimized image
		optimizedImage = SDL_DisplayFormat(loadedImage);
	
		// Free unneeded pointer
		SDL_FreeSurface(loadedImage);
		
		if(optimizedImage != NULL)
		{
			SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 0, 0xFF, 0xFF));
		}
	}

	// Return loaded image after being optimized
	return optimizedImage;
}


void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
	// Make a temporary rectangle to hold the offsets
	SDL_Rect offset;

	// Give the offsets to the rectangle
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, NULL, destination, &offset);
}


bool load_files()
{
	// Load the background image
	background = load_image("background.png");

	// Open the font
	font = TTF_OpenFont("lazy.ttf", 30);

	// If there was a problem in loading the background
	if(background == NULL)
	{
		return false;
	}

	// If there was error in loading the font
	if(font == NULL)
	{
		return false;
	}

	// Load the music
	music = Mix_LoadMUS("beat.wav");

	// If there was a problem loading the music
	if(music == NULL)
	{
		return false;
	}

	// Load the sound effects
	scratch = Mix_LoadWAV("scratch.wav");
	high = Mix_LoadWAV("high.wav");
	med = Mix_LoadWAV("medium.wav");
	low = Mix_LoadWAV("low.wav");

	// If there was a problem loading the sound effects
	if((scratch == NULL) || (high == NULL) || (med == NULL) || (low == NULL))
	{
		return false;
	}

	// If everything loaded fine
	return true;
}

bool init()
{
	// Initialize all SDL subsystems
	if(SDL_Init(SDL_INIT_EVERYTHING) == 1)
	{
		return false;
	}

	// Set up the screen
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	// If there was an error setting up the screen
	if(screen == NULL)
	{
		return false;
	}

	// Initialize SDL_ttf
	if(TTF_Init() == -1)
	{
		return false;
	}

	// Initialize SDL_Mixer
	if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		return false;
	}

	// Set the window caption
	SDL_WM_SetCaption("Monitor Music", NULL);

	// If everything initialized fine
	return true;
}


void clean_up()
{
	// Free the surfaces
	SDL_FreeSurface(background);

	// Free the sound effects
	Mix_FreeChunk(scratch);
	Mix_FreeChunk(high);
	Mix_FreeChunk(med);
	Mix_FreeChunk(low);

	// Free the music
	Mix_FreeMusic(music);

	// Close the font
	TTF_CloseFont(font);

	// Quit SDL_mixer
	Mix_CloseAudio();

	// Quit SDL_ttf
	TTF_Quit();

	// Quit SDL
	SDL_Quit();
}
