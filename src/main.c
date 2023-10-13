#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "./buffer.h"
#include "./font.h"

static Buffer_Context context;

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
	    SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
	    SDL_WINDOW_RESIZABLE);
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

	const SDL_Color line_color = {255, 255, 255, 150};
	const SDL_Color text_color = {255, 255, 255, 255};
	buffer_prepare(&context);

	char *last_text;
	int w;
	int h;

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
					if (buffer_get_cursor_row(&context) > 0) {
						context.lines[context.cursor].cursor--;
					}
				} break;
				case SDLK_RIGHT: {
					if (buffer_get_cursor_row(&context) <
					    context.lines[context.cursor].size) {
						context.lines[context.cursor].cursor++;
					}
				} break;
				case SDLK_UP: {
					if (context.cursor > 0) {
						context.cursor--;
						context.lines[context.cursor].cursor =
						    context.lines[context.cursor].size;
					}
				} break;
				case SDLK_DOWN: {
					if (context.cursor < (context.size - 1)) {
						context.cursor++;
						context.lines[context.cursor].cursor =
						    context.lines[context.cursor].size;
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
		for (size_t i = 0; i < context.size; ++i) {
			char line[MAX_BUFFER_SIZE];
			sprintf(line, "%zu", (i + 1));
			font_data line_num = prepare_string(
			    font, renderer, 0, i * data.font_h, line, line_color);
			SDL_RenderCopy(renderer, line_num.texture, NULL, &line_num.rect);
			SDL_DestroyTexture(line_num.texture);

			data = prepare_string(font, renderer, line_num.font_w + 4,
			    i * data.font_h, context.lines[i].buffer, text_color);
			SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);
			SDL_DestroyTexture(data.texture);
		}

		if (strcmp(last_text, context.lines[context.cursor].buffer) != 0) {
			TTF_SizeText(font, context.lines[context.cursor].buffer, &w, &h);
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect rect = {
		    .x = 15 + (context.lines[context.cursor].cursor *
		                  (w / (float)context.lines[context.cursor].size)),
		    .y = context.cursor * h,
		    .w = 3,
		    .h = h};
		SDL_RenderFillRect(renderer, &rect);
		last_text = context.lines[context.cursor].buffer;

		SDL_RenderPresent(renderer);
	}

	buffer_free(&context);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	return 0;
}
