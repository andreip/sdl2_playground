#pragma once
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Texture.h"

class Game {
public:
  // initializes SDL logic.
  Game()
    : mWindow(nullptr),
    mRenderer(nullptr),
    mTextureSprites(nullptr)
  {
  }

  // cleanup SDL logic.
  ~Game() { free(); }
  void init();
  void loop();  // game loop
private:
  void free();
  void loadMedia();

  // rendering state
  SDL_Event mEvent;
  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  Wrapped_SDL_Texture *mTextureSprites;

  // sprites data
  static const char *SPRITES_PATH;
  constexpr static int SPRITES_TOTAL_SYMBOLS = 8;
  SDL_Rect mSpriteRects[SPRITES_TOTAL_SYMBOLS];
  constexpr static int SPRITE_WIDTH = 108;
  constexpr static int SPRITE_HEIGHT = 111;

  // screen data
  constexpr static int SCREEN_WIDTH = 640;
  constexpr static int SCREEN_HEIGHT = 480;
};
