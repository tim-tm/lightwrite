#ifndef FONT_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct s_font_data {
  SDL_Texture *texture;
  SDL_Rect rect;

  int font_w;
  int font_h;
} font_data;

font_data prepare_string(TTF_Font *font, SDL_Renderer *renderer, int x, int y,
                         const char *string, SDL_Color color);

#endif // !FONT_H
