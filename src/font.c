#include "./font.h"
#include <string.h>

static font_data last_data = {
    NULL, NULL, {0, 0, 0, 0}, 0, 0,
};

font_data prepare_string(TTF_Font *font, SDL_Renderer *renderer, int x, int y,
                         const char *string, SDL_Color color) {
  if (last_data.texture && strcmp(last_data.text, string) == 0) {
    return last_data;
  }

  SDL_Surface *f_surface = TTF_RenderText_Blended(font, string, color);
  SDL_Texture *f_texture = SDL_CreateTextureFromSurface(renderer, f_surface);
  SDL_FreeSurface(f_surface);

  int w, h;
  TTF_SizeText(font, string, &w, &h);
  SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};

  last_data.text = string;
  last_data.texture = f_texture;
  last_data.rect = rect;
  last_data.font_w = w;
  last_data.font_h = h;
  return last_data;
}
