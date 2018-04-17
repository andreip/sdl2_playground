#include <random>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "Game.h"
#include "utils.h"

void Game::init() {
  try {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
      throw GameException("Error initializing sdl", SDL_GetError());

    mWindow = SDL_CreateWindow("SlotMachine",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!mWindow)
      throw GameException("Window couldn't be created", SDL_GetError());

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
      throw GameException("Coudln't create gRenderer!", SDL_GetError());

    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    // need to initialize SDL images for PNG images.
    if (!(IMG_Init(imgFlags) & imgFlags))
      throw GameException("SDL_image could not initialize", IMG_GetError());

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
      throw GameException("SDL_mixer could not initialize", Mix_GetError());

    mSlotMachine = new SlotMachine(
      mRenderer,
      (SCREEN_WIDTH - SLOT_MACHINE_WIDTH) / 2,
      (SCREEN_HEIGHT - SLOT_MACHINE_HEIGHT) / 2,
      SLOT_MACHINE_WIDTH,
      SLOT_MACHINE_HEIGHT
    );

  } catch (GameException &e) {
    free();
    throw;
  }
}

void Game::free() {
  delete mSlotMachine;
  mSlotMachine = nullptr;
  SDL_DestroyRenderer(mRenderer);
  mRenderer = nullptr;
  SDL_DestroyWindow(mWindow);
  mWindow = nullptr;
  IMG_Quit();
  Mix_Quit();
  SDL_Quit();
}

void Game::loop() {
  bool quit = false;

  while (!quit) {
    while (SDL_PollEvent(&mEvent)) {
      if (mEvent.type == SDL_QUIT) {
        quit = true;
      }

      if (mEvent.type == SDL_KEYDOWN) {
        const Uint8 *state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_SPACE]) {
          std::cout << "pressed spacebar key\n";
          mSlotMachine->start();
        }
      }
    }

    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);

    // render slot machine display.
    if (mSlotMachine->running())
      mSlotMachine->advance();
    mSlotMachine->render();

    // draw surroundings of the slot machine.
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    int quadHeight = (SCREEN_HEIGHT - SLOT_MACHINE_HEIGHT) / 2;
    SDL_Rect topQuad {0, 0, SCREEN_WIDTH, quadHeight};
    SDL_RenderFillRect(mRenderer, &topQuad);
    SDL_Rect bottomQuad {0, SCREEN_HEIGHT - quadHeight, SCREEN_WIDTH, quadHeight};
    SDL_RenderFillRect(mRenderer, &bottomQuad);

    SDL_RenderPresent(mRenderer);
  }
}
