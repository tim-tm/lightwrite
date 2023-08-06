#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "./buffer.h"
#include "./font.h"

static buffer_context context;

static const int screen_width = 800;
static const int screen_height = 600;

int main(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL-Error: %s\n", SDL_GetError());
    return -1;
  }

  if (TTF_Init() < 0) {
    printf("TTF-Error: %s\n", TTF_GetError());
    return -1;
  }

  SDL_Window *window = SDL_CreateWindow("lightwrite", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, screen_width,
                                        screen_height, SDL_WINDOW_RESIZABLE);
  if (!window) {
    printf("SDL-Error: %s\n", SDL_GetError());
    return -1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("SDL-Error: %s\n", SDL_GetError());
    return -1;
  }

  TTF_Font *font = TTF_OpenFont("MonoLisaRegular.ttf", 16);
  if (!font) {
    printf("TTF-Error: %s\n", TTF_GetError());
    return -1;
  }

  const SDL_Color f_colr = {255, 255, 255, 255};
  buffer_prepare(&context);

  bool closed = false;
  while (!closed) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      switch (ev.type) {
      case SDL_QUIT: {
        closed = true;
      } break;
      case SDL_KEYDOWN: {
        switch (ev.key.keysym.sym) {
        case SDLK_BACKSPACE: {
          buffer_del_cursor(&context);
        } break;
        case SDLK_LEFT: {
          if (context.lines[context.size - 1].cursor > 0) {
            context.lines[context.size - 1].cursor--;
          }
        } break;
        case SDLK_RIGHT: {
          if (context.lines[context.size - 1].cursor <
              context.lines[context.size - 1].size) {
            context.lines[context.size - 1].cursor++;
          }
        } break;
        case SDLK_DELETE: {
          buffer_del(&context);
        } break;
        case SDLK_RETURN: {
          buffer_push_line(&context);
        } break;
        }
      } break;
      case SDL_TEXTINPUT: {
        buffer_ins_cursor(&context, ev.text.text);
      }
      }
      break;
    }
    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 0);
    SDL_RenderClear(renderer);

    font_data data;
    size_t i;
    for (i = 0; i < context.size; ++i) {
      data = prepare_string(font, renderer, 0, i * data.font_h,
                            context.lines[i].buffer, f_colr);
      SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);
      SDL_DestroyTexture(data.texture);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = {
        .x = context.lines[context.size - 1].cursor *
             (data.font_w / (float)context.lines[context.size - 1].size),
        .y = context.cursor_col * data.font_h,
        .w = 3,
        .h = data.font_h};
    SDL_RenderFillRect(renderer, &rect);

    SDL_RenderPresent(renderer);
  }

  buffer_free(&context);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
