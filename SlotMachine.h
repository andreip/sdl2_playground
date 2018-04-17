#pragma once

#include <SDL2/SDL.h>

#include "Texture.h"

class SlotMachine {
public:
  SlotMachine(SDL_Renderer *renderer=nullptr);
  ~SlotMachine();
  void render(int x, int y, int width, int height);
  void run();

private:
  Wrapped_SDL_Texture *mTextureSprites;
  SDL_Renderer *mRenderer;
  int mRow;

  static constexpr int COLUMNS = 3;
  static const double COLUMN_RATIOS[];
  // sprites data
  static const char *SPRITES_PATH;
  static constexpr int SPRITES_TOTAL_SYMBOLS = 8;
  SDL_Rect mSpriteRects[SPRITES_TOTAL_SYMBOLS];
  static constexpr int SPRITE_WIDTH = 111;
  static constexpr int SPRITE_HEIGHT = 111;

};
