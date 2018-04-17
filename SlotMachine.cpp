#include <random>
#include <SDL2/SDL.h>

#include "SlotMachine.h"

const char* SlotMachine::SPRITES_PATH = "img/slot_machine_sprites.png";
const double SlotMachine::COLUMN_RATIOS[] = {0.166, 0.5, 0.833};

SlotMachine::SlotMachine(SDL_Renderer *renderer)
  :mRenderer(renderer), mTextureSprites(nullptr), mRow(0)
{
  mTextureSprites = Wrapped_SDL_Texture::fromPath(SPRITES_PATH, mRenderer);

  for (int i = 0; i < SPRITES_TOTAL_SYMBOLS; ++i) {
    mSpriteRects[i] = {i * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
  }

  //std::random_device rd;
  //std::mt19937 g(rd());
}

SlotMachine::~SlotMachine()
{
  delete mTextureSprites;
  mTextureSprites = nullptr;
  mRenderer = nullptr;
}

void SlotMachine::render(int x, int y, int width, int height)
{
  // outer bounding box draw
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0xFF);
  SDL_Rect slotMachineBoxQuad {x, y, width, height};
  SDL_RenderDrawRect(mRenderer, &slotMachineBoxQuad);

  // draw slot machine symbols
  for (int i = 0; i < COLUMNS; ++i) {
    int colX = x + COLUMN_RATIOS[i] * width - SPRITE_WIDTH / 2;

    // start from above row and go upwards.
    int colY = y + mRow - SPRITE_HEIGHT;
    int j = SPRITES_TOTAL_SYMBOLS;
    while (colY >= y - SPRITE_HEIGHT) {
      mTextureSprites->render(colX, colY, &mSpriteRects[--j]);
      colY -= SPRITE_HEIGHT;
    }

    // draw below the row.
    colY = y + mRow;
    j = -1;
    while (colY < y + height) {
      mTextureSprites->render(colX, colY, &mSpriteRects[++j]);
      colY += SPRITE_HEIGHT;
    }
  }

    //// draw  vertical lines between slots
    //SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 0xFF);
    //SDL_Rect verticalRect1 {SCREEN_WIDTH / 3, 0, 3, SCREEN_HEIGHT};
    //SDL_RenderFillRect(mRenderer, &verticalRect1);
    //SDL_Rect verticalRect2 {SCREEN_WIDTH * 2 / 3, 0, 3, SCREEN_HEIGHT};
    //SDL_RenderFillRect(mRenderer, &verticalRect2);
    //// draw mid line
    //SDL_SetRenderDrawColor(mRenderer, 0xFF, 0, 0, 0xFF);
    //SDL_RenderDrawLine(mRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

}

void SlotMachine::run() {
  mRow = (mRow + 20) % (SPRITE_HEIGHT * SPRITES_TOTAL_SYMBOLS);
}
