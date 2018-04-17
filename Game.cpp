#include <algorithm>
#include <random>
#include <vector>

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
  int row = 0;

  while (!quit) {
    while (SDL_PollEvent(&mEvent)) {
      if (mEvent.type == SDL_QUIT) {
        quit = true;
      }

      if (mEvent.type == SDL_KEYDOWN) {
        const Uint8 *state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_SPACE]) {
          std::cout << "pressed spacebar key\n";
          row = (row + 20) % (SPRITE_HEIGHT * SPRITES_TOTAL_SYMBOLS);
          std::cout << "row is: " << row << "\n";
        }
      }
    }

    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(mRenderer);

    // draw slot machine symbols
    int col = -1;
    for (double colRatio : {0.166, 0.5, 0.833}) {
      ++col;
      int x = colRatio * SCREEN_WIDTH - SPRITE_WIDTH / 2;

      // start from above row and go upwards.
      int y = row - SPRITE_HEIGHT;
      int i = SPRITES_TOTAL_SYMBOLS;
      while (y >= -SPRITE_HEIGHT) {
        mTextureSprites->render(x, y, &mSpriteCols[col][--i]);
        y -= SPRITE_HEIGHT;
      }

      // draw below the row.
      y = row, i = -1;
      while (y < SCREEN_HEIGHT) {
        mTextureSprites->render(x, y, &mSpriteCols[col][++i]);
        y += SPRITE_HEIGHT;
      }
    }

    // draw  vertical lines between slots
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0xFF);
    SDL_Rect verticalRect1 { SCREEN_WIDTH / 3, 0, 3, SCREEN_HEIGHT };
    SDL_RenderFillRect(mRenderer, &verticalRect1);
    SDL_Rect verticalRect2 { SCREEN_WIDTH * 2 / 3, 0, 3, SCREEN_HEIGHT };
    SDL_RenderFillRect(mRenderer, &verticalRect2);
    // draw mid line
    SDL_SetRenderDrawColor(mRenderer, 0xFF, 0, 0, 0xFF);
    SDL_RenderDrawLine(mRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

    SDL_RenderPresent(mRenderer);
  }
}

void Game::loadMedia() {
  mTextureSprites = Wrapped_SDL_Texture::fromPath(SPRITES_PATH, mRenderer);

  // get all symbols in vector
  std::vector<SDL_Rect> v(SPRITES_TOTAL_SYMBOLS);
  for (int i = 0; i < SPRITES_TOTAL_SYMBOLS; ++i) {
    v[i] = {i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
  }
  // init random generator
  std::random_device rd;
  std::mt19937 g(rd());
  // shuffle it for every slot machine column.
  for (int i = 0; i < SPRITES_COLS; ++i) {
    std::shuffle(v.begin(), v.end(), g);
    for (int j = 0; j < v.size(); ++j)
      mSpriteCols[i][j] = v[j];
  }
}
