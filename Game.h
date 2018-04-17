#pragma once
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SlotMachine.h"

class Game {
public:
  // initializes SDL logic.
  Game()
    : mWindow(nullptr),
    mRenderer(nullptr)
  {
  }

  // cleanup SDL logic.
  ~Game() { free(); }
  void init();
  void loop();  // game loop
private:
  void free();

  // rendering state
  SDL_Event mEvent;
  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  SlotMachine *mSlotMachine;

  // screen data
  constexpr static int SCREEN_WIDTH = 640;
  constexpr static int SCREEN_HEIGHT = 480;

  // slot machine box
  constexpr static int SLOT_MACHINE_HEIGHT = 200;
  constexpr static int SLOT_MACHINE_WIDTH = SCREEN_WIDTH - 100;
};
