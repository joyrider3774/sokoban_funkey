#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <SDL/SDL_keysym.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_framerate.h>
#include "GameFuncs.h"
#include "CUsbJoystickSetup.h"
#include "CWorldParts.h"
#include "Common.h"
#include "Defines.h"
#include "Game.h"
#include "SetupUSBJoysticks.h"
#include "Game.h"
#include "LevelEditor.h"
#include "LevelEditorMenu.h"
#include "StageSelect.h"
#include "Credits.h"
#include "Titlescreen.h"
#include "CInput.h"

using namespace std;

int main(int argc, char **argv)
{
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == 0)
	{
		bool fullscreen = false;
		for (int i=0; i < argc; i++)
		{
			if(strcasecmp(argv[i], "-f") == 0)
				fullscreen = true;
			if(strcasecmp(argv[i], "-s2") == 0)
			{
				WINDOW_WIDTH = 240 * 2;
				WINDOW_HEIGHT = 240 * 2;
			}

			if(strcasecmp(argv[i], "-s3") == 0)
			{
				WINDOW_WIDTH = 240 * 3;
				WINDOW_HEIGHT = 240 * 3;
			}

			if(strcasecmp(argv[i], "-s4") == 0)
			{
				WINDOW_WIDTH = 240 * 4;
				WINDOW_HEIGHT = 240 * 4;
			}

			if(strcasecmp(argv[i], "-s5") == 0)
			{
				WINDOW_WIDTH = 240 * 5;
				WINDOW_HEIGHT = 240 * 5;
			}
		}
		printf("SDL Succesfully initialized\n");
		Uint32 flags = SDL_SWSURFACE;
		if(fullscreen)
			flags |= SDL_FULLSCREEN;
        Screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,0, flags);
		if(Screen)
		{
			Icon = IMG_Load("./graphics/sokoban.png");
        	SDL_WM_SetIcon(Icon,NULL);
			SDL_WM_SetCaption( "Sokoban GP2X - Created By Willems Davy", NULL );
		    SDL_Surface *Tmp = SDL_CreateRGBSurface(SDL_SWSURFACE,ORIG_WINDOW_WIDTH,ORIG_WINDOW_HEIGHT,SCREEN_BPP,Screen->format->Rmask,Screen->format->Gmask,Screen->format->Bmask,Screen->format->Amask);
			Buffer = SDL_DisplayFormat(Tmp);
			SDL_FreeSurface(Tmp);
			printf("Succesfully Set %dx%dx%d\n",WINDOW_WIDTH,WINDOW_HEIGHT,SCREEN_BPP);
			SDL_ShowCursor(SDL_DISABLE);
				if (Mix_OpenAudio(22050,AUDIO_S16,MIX_DEFAULT_CHANNELS,1024) < 0)
				{
					GlobalSoundEnabled = false;
					printf("Failed to initialise sound!\n");
					printf("%s\n",Mix_GetError());
				}
				else
				{
			 		printf("Audio Succesfully initialised!\n");
				}
				if (TTF_Init() == 0)
				{
					printf("Succesfully initialized TTF\n");					
					font = TTF_OpenFont("./fonts/Roboto-Light.ttf",12*UI_HEIGHT_SCALE);
					font2 = TTF_OpenFont("./fonts/Roboto-Light.ttf",10*UI_HEIGHT_SCALE);
					font3 = TTF_OpenFont("./fonts/Roboto-Light.ttf",11*UI_HEIGHT_SCALE);
					BigFont = TTF_OpenFont("./fonts/Roboto-Light.ttf",13*UI_HEIGHT_SCALE);
					MonoFont = TTF_OpenFont("./fonts/SpaceMono-Regular.ttf",12*UI_HEIGHT_SCALE);
					RobotoBig = TTF_OpenFont("./fonts/Roboto-Black.ttf",35);
					RobotoMedium = TTF_OpenFont("./fonts/Roboto-Black.ttf",15);
					if (font && BigFont && MonoFont)
					{
						printf("Succesfully Loaded fonts\n");
						JoystickSetup = new CUsbJoystickSetup();
                        LoadJoystickSettings();
                        SDL_initFramerate(&Fpsman);
                        SDL_setFramerate(&Fpsman,FPS);
						Mix_HookMusicFinished(MusicFinished);
						TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
						SearchForLevelPacks();
						SearchForMusic();
						LoadSettings();
						LoadSounds();
						LoadGraphics();						
						LevelPackFile = new CLevelPackFile();
						while (GameState != GSQuit)
						{
							switch(GameState)
							{
								case GSIntro :
									break;
								case GSTitleScreen :
									TitleScreen();
									break;
								case GSCredits :
									Credits();
									break;
								case GSGame :
									Game();
									break;
								case GSStageClear:
									break;
								case GSStageSelect:
									StageSelect();
									break;
								case GSLevelEditor:
									LevelEditor();
									break;
								case GSJoystickSetup:
									SetupUsbJoystickButtons();
									break;
								case GSLevelEditorMenu:
									LevelEditorMode = true;
									LevelEditorMenu();
									break;
							}
						}
						delete LevelPackFile;
						SaveSettings();
						UnLoadGraphics();
						UnloadSounds();
						UnloadMusic();
						TTF_CloseFont(font);
						TTF_CloseFont(font2);
						TTF_CloseFont(font3);
						TTF_CloseFont(BigFont);
						TTF_CloseFont(MonoFont);
						TTF_CloseFont(RobotoBig);
						TTF_CloseFont(RobotoMedium);
						MonoFont=NULL;
				    	font=NULL;
				    	BigFont=NULL;
                        delete JoystickSetup;
					}
					else
					{
						printf("Failed to Load fonts\n");
					}
					TTF_Quit();
				}
				else
				{
					printf("Failed to initialize TTF\n");
				}
				SDL_FreeSurface(Screen);
				Screen=NULL;
				SDL_FreeSurface(Icon);
				Icon=NULL;
				Mix_CloseAudio();

		}
		else
		{
			printf("Failed to Set Videomode %dx%dx%d\n",WINDOW_WIDTH, WINDOW_HEIGHT, SCREEN_BPP);
		}
		SDL_Quit();
	}
	else
	{
		printf("Couldn't initialise SDL!\n");
	}
	WorldParts.RemoveAll();
	return 0;

}
