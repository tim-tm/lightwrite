#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024

typedef struct s_buffer_context {
  char buffer[MAX_BUFFER_SIZE];
  size_t b_size;
  size_t b_cursor;
} buffer_context;

typedef struct s_font_data {
  SDL_Texture *texture;
  SDL_Rect rect;
} font_data;

static buffer_context context;

static const int screen_width = 800;
static const int screen_height = 600;
static int font_w, font_h;

void buffer_ins_cursor(const char *text) {
  size_t in_len = strlen(text);
  const size_t max_len = MAX_BUFFER_SIZE - context.b_size;
  if (in_len > max_len) {
    in_len = max_len;
  }

  memmove(context.buffer + context.b_cursor + in_len,
          context.buffer + context.b_cursor, context.b_size - context.b_cursor);
  memcpy(context.buffer + context.b_cursor, text, in_len);
  context.b_size += in_len;
  context.b_cursor += in_len;
}

void buffer_del_cursor() {
  if (context.b_cursor > 0 && context.b_size > 0) {
    memmove(context.buffer + context.b_cursor - 1,
            context.buffer + context.b_cursor,
            context.b_size - context.b_cursor);
    context.b_size--;
    context.buffer[context.b_size] = '\0';
    context.b_cursor--;
  }
}

void buffer_del() {
  if (context.b_cursor < context.b_size && context.b_size > 0) {
    memmove(context.buffer + context.b_cursor,
            context.buffer + context.b_cursor + 1,
            context.b_size - context.b_cursor);
    context.b_size--;
  }
}

font_data prepare_string(TTF_Font *font, SDL_Renderer *renderer, int x, int y,
                         const char *string, SDL_Color color) {
  SDL_Surface *f_surface = TTF_RenderText_Blended(font, string, color);
  SDL_Texture *f_texture = SDL_CreateTextureFromSurface(renderer, f_surface);
  SDL_FreeSurface(f_surface);

  TTF_SizeText(font, string, &font_w, &font_h);
  SDL_Rect rect = {.x = x, .y = y, .w = font_w, .h = font_h};

  font_data fd = {.texture = f_texture, .rect = rect};
  return fd;
}

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
          buffer_del_cursor();
        } break;
        case SDLK_LEFT: {
          if (context.b_cursor > 0) {
            context.b_cursor--;
          }
        } break;
        case SDLK_RIGHT: {
          if (context.b_cursor < context.b_size) {
            context.b_cursor++;
          }
        } break;
        case SDLK_DELETE: {
          buffer_del();
        } break;
        }
      } break;
      case SDL_TEXTINPUT: {
        buffer_ins_cursor(ev.text.text);
      }
      }
      break;
    }
    SDL_SetRenderDrawColor(renderer, 35, 35, 35, 0);
    SDL_RenderClear(renderer);

    const font_data data =
        prepare_string(font, renderer, 0, 0, context.buffer, f_colr);
    SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect = {.x = context.b_cursor * (font_w / (float)context.b_size),
                     .y = 0,
                     .w = 3,
                     .h = font_h};
    SDL_RenderFillRect(renderer, &rect);

    SDL_DestroyTexture(data.texture);
    SDL_RenderPresent(renderer);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
