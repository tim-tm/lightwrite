#include "font.h"
#include <string.h>

// NOTE: Removed Font_Data caching since it caused a invisibility bug and was not that much of a performance boost.
Font_Data prepare_string(TTF_Font *font, SDL_Renderer *renderer, int x, int y,
    const char *string, SDL_Color color) {
    int stringlen = strlen(string);
    char* draw_string = (char*) string;

    // Do not draw newline chars
    if (string[stringlen-1] == '\n') {
        draw_string[stringlen-1] = ' ';
    }

	SDL_Surface *f_surface = TTF_RenderText_Blended(font, string, color);
	SDL_Texture *f_texture = SDL_CreateTextureFromSurface(renderer, f_surface);
	SDL_FreeSurface(f_surface);

	int w, h;
	TTF_SizeText(font, string, &w, &h);
	SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};

    Font_Data data;
	data.text = draw_string;
	data.texture = f_texture;
	data.rect = rect;
	data.font_w = w;
	data.font_h = h;
	return data;
}
