#include "buffer.h"
#include "font.h"
#include "logger.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int screen_width = 800;
static const int screen_height = 600;
static const char font_path[] = "resource/MonoLisaRegular.ttf";
static const SDL_Color text_color = {255, 255, 255, 255};

static SDL_Window* window;
static SDL_Renderer* renderer;
static TTF_Font* font;
static Buffer_Context context;

static FILE* fp;
static bool ctrl_pressed;
static char* filename;

static bool init_all(void);
static void destroy_all(void);
static bool handle_events(void);

int main(int argc, char** argv) {
    if (argc > 2) {
        LOG_FATAL("Usage: %s <file>", argv[0]);
        return -1;
    }
    
    if (!init_all()) {
        return -1;
    }

    // Check the len, just to be sure.
    if (argc == 2 && strlen(argv[1]) > 0) {
        filename = calloc(1024, sizeof(char));
        strcpy(filename, argv[1]);
        
        // First read in the data
        fp = fopen(argv[1], "r");
        // buffer_write will automatically create the file anyways.
        if (fp) {
            buffer_read(&context, fp);
        }
    }

    bool closed = false;
	while (!closed) {
		closed = handle_events();
        
        SDL_SetRenderDrawColor(renderer, 35, 35, 35, 0);
		SDL_RenderClear(renderer);

		for (size_t i = 0; i < context.size; ++i) {
			Font_Data data = prepare_string(font, renderer, 0,
			    i * data.font_h, context.lines[i].buffer, text_color);
			SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);
			SDL_DestroyTexture(data.texture);
		}

        // Getting the string written from the left of the screen until the cursor.
        char substr[MAX_LINE_SIZE];
        memset(substr, 0, sizeof(substr));
        strncpy(substr, context.lines[context.cursor].buffer, context.lines[context.cursor].cursor);
        
        int width;
        int height;
        TTF_SizeText(font, substr, &width, &height);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_Rect rect = {
		    .x = width,
		    .y = context.cursor * height,
		    .w = 3,
		    .h = height};
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderPresent(renderer);
	}
	destroy_all();
    return 0;
}

static bool init_all(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		LOG_FATAL("SDL-Error: %s", SDL_GetError());
		return false;
	}

	if (TTF_Init() < 0) {
		SDL_Quit();
        LOG_FATAL("TTF-Error: %s", TTF_GetError());
		return false;
	}

	window = SDL_CreateWindow("lightwrite", SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
	    SDL_WINDOW_RESIZABLE);
	if (!window) {
        SDL_Quit();
        TTF_Quit();
		LOG_FATAL("SDL-Error: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(
	    window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!renderer) {
        SDL_Quit();
        TTF_Quit();
		SDL_DestroyWindow(window);
        LOG_FATAL("SDL-Error: %s", SDL_GetError());
		return false;
	}

	font = TTF_OpenFont(font_path, 16);
	if (!font) {
        SDL_Quit();
        TTF_Quit();
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
        LOG_FATAL("TTF-Error: %s", TTF_GetError());
		return false;
	}

	buffer_init(&context);
    return true;
}

static void destroy_all(void) {
	buffer_free(&context);
	TTF_CloseFont(font);
	TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	SDL_Quit();
    if (fp) fclose(fp);
    if (filename) free(filename);
}

static bool handle_events(void) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
        case SDL_QUIT: {
            return true;
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
            // TODO: Implement a better way of handling keybinds.
            case SDLK_LCTRL: {
                ctrl_pressed = true;
            } break;
            case SDLK_s: {
                // do not check for file*, buffer_write will create the class anyways.
                if (ctrl_pressed && filename) {
                    buffer_write(&context, fp, filename);
                    LOG_INFO("File saved!");
                }
            } break;
            }
        } break;
        case SDLK_UP: {
            if (ev.key.keysym.sym == SDLK_LCTRL) {
                ctrl_pressed = false;
            }
        } break;
        case SDL_TEXTINPUT: {
            buffer_ins_cursor(&context, ev.text.text);
        }
        }
        break;
    }
    return false;
}
