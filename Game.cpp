#include "Game.h"
#include "utils.h"

const char* Game::SPRITES_PATH = "img/slot_machine_sprites.png";

void Game::init() {
  try {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
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

    loadMedia();

  } catch (GameException &e) {
    free();
    throw;
  }
}

void Game::free() {
  SDL_DestroyRenderer(mRenderer);
  mRenderer = nullptr;
  SDL_DestroyWindow(mWindow);
  mWindow = nullptr;
  IMG_Quit();
  SDL_Quit();
}

void Game::loop() {
  bool quit = false;
  while (!quit) {
    while (SDL_PollEvent(&mEvent)) {
      if (mEvent.type == SDL_QUIT) {
        quit = true;
      }
    }

    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);

    // draw slot machine symbols
    for (double colRatio : {0.2, 0.5, 0.8}) {
      int x = colRatio * SCREEN_WIDTH - SPRITE_WIDTH / 2;
      for (int i = 0; i < SPRITES_TOTAL_SYMBOLS; ++i)
        mTextureSprites->render(x, i * SPRITE_HEIGHT, &mSpriteRects[i]);
    }

    // draw mid line
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0, 0, 0xFF);
    SDL_RenderDrawLine(mRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

    SDL_RenderPresent(mRenderer);
  }
}

void Game::loadMedia() {
  mTextureSprites = Wrapped_SDL_Texture::fromPath(SPRITES_PATH, mRenderer);

  for (int i = 0; i < SPRITES_TOTAL_SYMBOLS; ++i)
    mSpriteRects[i] = {i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
}
