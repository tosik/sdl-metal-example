#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <SDL2/SDL.h>
#include <string>

#include "shaders_metal_osx.h"

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow(
      "title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  auto renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED);
  SDL_RenderChangeMetalShader(renderer, shader_metallib, shader_metallib_len);

  SDL_RendererInfo info;
  SDL_GetRenderDriverInfo(1, &info);
  if (std::string(info.name) == "metal") {
    printf("Rendering API is Metal\n");
  } else {
    return -1;
  }

  auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_TARGET, 640, 480);

  auto texture_mask = SDL_CreateTextureSpecifiedMetalFragmentShader(
      renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480,
      "mask_fragment");

  auto texture_blur = SDL_CreateTextureSpecifiedMetalFragmentShader(
      renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 640, 480,
      "blur_fragment");

  bool firsttime = true;

  while (true) {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
      break;

    /*
        auto layer = (CAMetalLayer *)SDL_RenderGetMetalLayer(renderer);
        id<MTLRenderCommandEncoder> encoder =
            (id<MTLRenderCommandEncoder>)SDL_RenderGetMetalCommandEncoder(renderer);
    */

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, texture_mask);
    SDL_SetTextureBlendMode(texture_mask, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, texture_blur);
    SDL_SetTextureBlendMode(texture_blur, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, texture);

    // white box
    {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_Rect box{50, 50, 50, 50};
      SDL_RenderFillRect(renderer, &box);
    }

    // red box
    {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_Rect box{100, 50, 50, 50};
      SDL_RenderFillRect(renderer, &box);
    }

    // green box
    {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      SDL_Rect box{150, 50, 50, 50};
      SDL_RenderFillRect(renderer, &box);
    }

    // magenta box
    {
      SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
      SDL_Rect box{150, 50, 50, 50};
      SDL_RenderFillRect(renderer, &box);
    }

    // cyan box
    {
      SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
      SDL_Rect box{200, 50, 50, 50};
      SDL_RenderFillRect(renderer, &box);
    }

    // blue box
    {
      SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
      SDL_Rect box{250, 50, 50, 50};
      SDL_RenderFillRect(renderer, &box);
    }

    /*
    {
      SDL_Rect rect{50, 50, 50, 50};
      const float verts[] = {(float)rect.x,
                             (float)rect.y + (float)rect.h,
                             (float)rect.x,
                             (float)rect.y,
                             (float)rect.x + (float)rect.w,
                             (float)rect.y + (float)rect.h,
                             (float)rect.x + (float)rect.w,
                             (float)rect.y};

      float color[4] = {0.f, 1.f, 1.f, 1.f};
      [encoder setFragmentBytes:color length:sizeof(color) atIndex:0];
      [encoder setVertexBytes:verts length:sizeof(verts) atIndex:0];
      [encoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                  vertexStart:0
                  vertexCount:4];
    }

    {
      SDL_Rect rect{150, 100, 50, 50};
      const float verts[] = {(float)rect.x,
                             (float)rect.y + (float)rect.h,
                             (float)rect.x,
                             (float)rect.y,
                             (float)rect.x + (float)rect.w,
                             (float)rect.y + (float)rect.h,
                             (float)rect.x + (float)rect.w,
                             (float)rect.y};

      float color[4] = {1.f, 1.f, 0.f, 1.f};
      [encoder setFragmentBytes:color length:sizeof(color) atIndex:0];
      [encoder setVertexBytes:verts length:sizeof(verts) atIndex:0];
      [encoder drawPrimitives:MTLPrimitiveTypeTriangleStrip
                  vertexStart:0
                  vertexCount:4];
    }
    */

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, texture_mask);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_SetRenderTarget(renderer, texture_blur);
    SDL_RenderCopy(renderer, texture_mask, NULL, NULL);

    for (int i = 0; i < 100; i++) {
      SDL_SetRenderTarget(renderer, texture_blur);
      SDL_RenderCopy(renderer, texture_blur, NULL, NULL);
    }

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_SetTextureBlendMode(texture_blur, SDL_BLENDMODE_ADD);
    SDL_RenderCopy(renderer, texture_blur, NULL, NULL);

    SDL_RenderPresent(renderer);
    SDL_Delay(10);

    // for Mojave glitch
    if (firsttime) {
      SDL_SetWindowSize(window, 640, 480);
      firsttime = false;
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
