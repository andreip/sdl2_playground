#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils.h"

class Wrapped_SDL_Texture {
public:
  Wrapped_SDL_Texture(SDL_Renderer *renderer = nullptr,
                      SDL_Texture *texture = nullptr)
    : mRenderer(renderer),
    mTexture(texture),
    mWidth(0),
    mHeight(0)
  {
  }

  ~Wrapped_SDL_Texture() {
    if (mTexture) {
      SDL_DestroyTexture(mTexture);
      mTexture = nullptr;
      mHeight = mWidth = 0;
    }
    mRenderer = nullptr;  // outside will free it
  }

  void render(int x = 0, int y = 0, const SDL_Rect *clip = nullptr) {
    SDL_Rect renderQuad { x, y, mWidth, mHeight };
    if (clip) {
      renderQuad.w = clip->w;
      renderQuad.h = clip->h;
    }
    SDL_RenderCopy(mRenderer, mTexture, clip, &renderQuad);
  }

  static Wrapped_SDL_Texture* fromPath(std::string path, SDL_Renderer *renderer) {
    SDL_Texture *newTexture = nullptr;

    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface)
      throw GameException("Couldn't load image", IMG_GetError());

    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (!newTexture) {
      SDL_FreeSurface(loadedSurface);
      throw GameException("Couldn't create texture from img", SDL_GetError());
    }

    Wrapped_SDL_Texture *wrappedTexture = new Wrapped_SDL_Texture(renderer, newTexture);
    wrappedTexture->mWidth = loadedSurface->w;
    wrappedTexture->mHeight = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);

    return wrappedTexture;
  }
private:
  SDL_Renderer *mRenderer;
  SDL_Texture *mTexture;
  int mWidth, mHeight;
};
