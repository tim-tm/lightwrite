#include "./font.h"

font_data prepare_string(TTF_Font *font, SDL_Renderer *renderer, int x, int y,
                         const char *string, SDL_Color color) {
  SDL_Surface *f_surface = TTF_RenderText_Blended(font, string, color);
  SDL_Texture *f_texture = SDL_CreateTextureFromSurface(renderer, f_surface);
  SDL_FreeSurface(f_surface);

  int w, h;
  TTF_SizeText(font, string, &w, &h);
  SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};

  font_data fd = {.texture = f_texture, .rect = rect, .font_w = w, .font_h = h};
  return fd;
}
