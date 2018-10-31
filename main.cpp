#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>
#include <SDL2/SDL.h>

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow(
      "title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

  auto renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED);

  auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_TARGET, 640, 480);
  SDL_SetRenderTarget(renderer, texture);

  bool firsttime = true;

  while (true) {
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
      break;

    auto layer = (CAMetalLayer *)SDL_RenderGetMetalLayer(renderer);
    id<MTLRenderCommandEncoder> encoder =
        (id<MTLRenderCommandEncoder>)SDL_RenderGetMetalCommandEncoder(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    SDL_Rect r{0, 0, 0, 0};
    SDL_RenderFillRect(renderer, &r);

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

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);

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
