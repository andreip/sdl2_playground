#pragma once

#include <random>
#include <SDL2/SDL.h>

#include "Texture.h"

class SlotMachine {
public:
  SlotMachine(SDL_Renderer *renderer=nullptr,
              int x = 0, int y = 0,
              int width = 0, int height = 0);
  ~SlotMachine();
  void render();
  void start();
  bool running();
  void advance();
private:
  double getRandomDouble();
  void stopLeftmostSpin();
  bool atLeastOneRunning();
private:
  static constexpr int COLUMNS = 3;
  Wrapped_SDL_Texture *mTextureSprites;
  SDL_Renderer *mRenderer;
  int mX, mY, mWidth, mHeight;
  int mRow;
  bool mRunning[COLUMNS];

  static const double COLUMN_RATIOS[COLUMNS];
  // sprites data
  static const char *SPRITES_PATH;
  static constexpr int SPRITES_TOTAL_SYMBOLS = 8;
  SDL_Rect mSpriteRects[SPRITES_TOTAL_SYMBOLS];
  static constexpr int SPRITE_WIDTH = 111;
  static constexpr int SPRITE_HEIGHT = 111;

};
