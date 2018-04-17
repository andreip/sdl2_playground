#include <random>
#include <iostream>
#include <SDL2/SDL.h>

#include "SlotMachine.h"

const char* SlotMachine::SPRITES_PATH = "img/slot_machine_sprites.png";
const double SlotMachine::COLUMN_RATIOS[COLUMNS] = {0.166, 0.5, 0.833};

SlotMachine::SlotMachine(SDL_Renderer *renderer,
                         int x, int y, int width, int height)
  :mRenderer(renderer),
  mTextureSprites(nullptr),
  mX(x),
  mY(y),
  mWidth(width),
  mHeight(height)
{
  mTextureSprites = Wrapped_SDL_Texture::fromPath(SPRITES_PATH, mRenderer);

  for (int i = 0; i < SPRITES_TOTAL_SYMBOLS; ++i) {
    mSpriteRects[i] = {i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
  }

  for (int i = 0; i < COLUMNS; ++i) {
    mRunning[i] = false;
    mRows[i] = 0;
  }
}

SlotMachine::~SlotMachine()
{
  delete mTextureSprites;
  mTextureSprites = nullptr;
  mRenderer = nullptr;
}

void SlotMachine::render()
{
  int midVertical = mY + mHeight / 2 - SPRITE_HEIGHT / 2;
  // draw slot machine symbols
  for (int i = 0; i < COLUMNS; ++i) {
    int colX = mX + COLUMN_RATIOS[i] * mWidth - SPRITE_WIDTH / 2;

    // start from above row and go upwards.
    int colY = midVertical + mRows[i] - SPRITE_HEIGHT;
    int j = SPRITES_TOTAL_SYMBOLS;
    while (colY >= mY - SPRITE_HEIGHT) {
      mTextureSprites->render(colX, colY, &mSpriteRects[--j]);
      colY -= SPRITE_HEIGHT;
    }

    // draw below the row.
    colY = midVertical + mRows[i];
    j = -1;
    while (colY < mY + mHeight) {
      mTextureSprites->render(colX, colY, &mSpriteRects[++j]);
      colY += SPRITE_HEIGHT;
    }
  }

  // draw mid reference line of the machine
  SDL_SetRenderDrawColor(mRenderer, 0xFF, 0, 0, 0xFF);
  SDL_RenderDrawLine(mRenderer, mX, mY + mHeight / 2, mX + mWidth - 1, mY + mHeight / 2);

  // outer bounding box draw
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0xFF);
  SDL_Rect slotMachineBoxQuad {mX, mY, mWidth, mHeight};
  SDL_RenderDrawRect(mRenderer, &slotMachineBoxQuad);


  // draw  vertical lines between slots
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0xFF);
  SDL_Rect verticalRect1 {mX + mWidth / 3, mY, 3, mHeight};
  SDL_RenderFillRect(mRenderer, &verticalRect1);
  SDL_Rect verticalRect2 {mX + mWidth * 2 / 3, mY, 3, mHeight};
  SDL_RenderFillRect(mRenderer, &verticalRect2);

}

void SlotMachine::start() {
  // can't restart until it's stopped.
  if (running())
    return;

  for (int i = 0; i < COLUMNS; ++i)
    mRunning[i] = true;
}

double SlotMachine::getRandomDouble() {
  // random generator
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<> dis(0.0, 1.0);
  // between 0 and 1, as distribution was created.
  return dis(gen);
}

bool SlotMachine::running() {
  bool isRunning = atLeastOneRunning();

  if (isRunning && getRandomDouble() < 0.03) {
    stopLeftmostSpin();
    isRunning = atLeastOneRunning();
  }

  return isRunning;
}

void SlotMachine::advance() {
  for (int i = 0; i < COLUMNS; ++i)
    if (running(i))
      mRows[i] = (mRows[i] + SPRITES_TOTAL_SYMBOLS) % (SPRITE_HEIGHT * SPRITES_TOTAL_SYMBOLS);
}

bool SlotMachine::atLeastOneRunning() {
  for (int i = 0; i < COLUMNS; ++i)
    if (running(i))
      return true;
  return false;
}

bool SlotMachine::running(int column) {
  // 112 is "adjusted to work here" since we're incrementing each row
  // by 8, and 112 is divisible and very close to 111, which is the
  // vertical length of each sprite. So if this happens (because
  // a sprite is vertically alligned at the beginning), it means a
  // sprite is vertically alligned on the screen and we can stop.
  bool notVerticallyAllignedSprite = mRows[column] % 112 != 0;
  // run until it's vertically aligned, even if it's stopped.
  return mRunning[column] || notVerticallyAllignedSprite;
}

void SlotMachine::stopLeftmostSpin() {
  for (int i = 0; i < COLUMNS; ++i) {
    // use running(i) here to only stop a next column after a previous
    // one has stopped completely, to avoid having a right sprite
    // actually stop before a left one.
    if (running(i)) {
      mRunning[i] = false;
      return;
    }
  }
}
