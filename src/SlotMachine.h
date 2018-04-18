#pragma once

#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Texture.h"

enum Sounds {
  SOUND_COLUMN_STOP,
  SOUND_WINNER,
  SOUND_TOTAL,
};

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
  bool atLeastOneRunning();
  bool running(int column);
  void stopLeftmostSpin();
  int getSpriteOnDisplay(int column);
  void columnStoppedAndAlligned(int column);
  bool winner();
private:
  static constexpr int COLUMNS = 3;
  Wrapped_SDL_Texture *mTextureSprites;
  SDL_Renderer *mRenderer;
  int mX, mY, mWidth, mHeight;
  // keep account of where we draw each column.
  int mRows[COLUMNS];
  // if column should be advanced; but column still is advanced up
  // until a stripe gets vertically alligned on the display.
  bool mRunning[COLUMNS];
  // marks when a column is not advanced anymore and is alligned.
  bool mColumnStoppedAndAlligned[COLUMNS];

  // sound related
  Mix_Chunk *mSounds[SOUND_TOTAL];
  static const char* SOUND_COLUMN_STOP_PATH;
  static const char* SOUND_WINNER_PATH;

  static const double COLUMN_RATIOS[COLUMNS];
  // sprites data
  static const char *SPRITES_PATH;
  static constexpr int SPRITES_TOTAL_SYMBOLS = 8;
  SDL_Rect mSpriteRects[SPRITES_TOTAL_SYMBOLS];
  static constexpr int SPRITE_WIDTH = 111;
  static constexpr int SPRITE_HEIGHT = 111;
  static constexpr int SPRITE_ROWOFFSET = 112;

};
