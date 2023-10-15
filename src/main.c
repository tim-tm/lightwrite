#include "buffer.h"
#include "font.h"
#include "keybinds.h"
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
static char* filename;
static bool choosing_filename;

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

    // Moving some variable decls up here for performance reasons.
    char substr[MAX_LINE_SIZE];
    int width;
    int height;
    Font_Data data;
    SDL_Rect rect;

    bool closed = false;
	while (!closed) {
		closed = handle_events();
        
        SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
		SDL_RenderClear(renderer);

		for (size_t i = 0; i < context.size; ++i) {
            TTF_SizeText(font, context.lines[i].buffer, &width, &height);

            data = prepare_string(font, renderer, 0,
			    i * data.font_h, context.lines[i].buffer, text_color);
            SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);
			SDL_DestroyTexture(data.texture);   
		}
        
        // Getting the string written from the left of the screen until the cursor.
        memset(substr, 0, sizeof(substr));
        strncpy(substr, context.lines[context.cursor].buffer, context.lines[context.cursor].cursor);

        if (width >= screen_width) {
            // TODO: Manage writing further than the screen_width
            width = screen_width - 3;
        }

        TTF_SizeText(font, substr, &width, &height);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		rect.x = width;
		rect.y = context.cursor * height;
		rect.w = 3;
		rect.h = height;
		SDL_RenderFillRect(renderer, &rect);
        
        if (choosing_filename) {
            TTF_SizeText(font, filename, &width, &height);

            rect.x = screen_width / 2 - width / 2 - 5;
            rect.y = screen_height / 2 - height / 2 - 2;
            rect.w = width + 10;
            rect.h = height + 4;
            
            SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
            SDL_RenderFillRect(renderer, &rect);

            data = prepare_string(font, renderer, screen_width / 2 - width / 2, screen_height / 2 - height / 2, filename, text_color);
            SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);
            SDL_DestroyTexture(data.texture);
        }
		
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
    if (!logger_init()) return false;
    if (!keybinds_init()) return false;
    return true;
}

static void destroy_all(void) {
    keybinds_destroy();
    logger_destroy();
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
            keybinds_on_down(ev.key.keysym.sym);

            switch (ev.key.keysym.sym) {
            case SDLK_BACKSPACE: {
                if (choosing_filename) {
                    int len = strlen(filename);
                    if (len <= 1) {
                        filename = NULL;
                        choosing_filename = false;
                    }
                    // Not that elegant but filename will be set to NULL if the strlen is <= 1
                    if (filename) filename[len-1] = '\0';
                } else {
                    buffer_del_cursor(&context);
                }
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
                if (choosing_filename) {
                    buffer_write(&context, fp, filename);
                    LOG_INFO("%s saved!", filename);
                    choosing_filename = false;
                } else {
                    buffer_push_line(&context);
                }
            } break;
            case SDLK_s: {
                // do not check for file*, buffer_write will create the class anyways.
                if (keybinds_is_down(SDLK_LCTRL)) {
                    if (filename) {
                        buffer_write(&context, fp, filename);
                        LOG_INFO("%s saved!", filename);
                    } else {
                        filename = "Choose a filename!";
                        choosing_filename = true;
                    }
                }
            } break;
            }
        } break;
        case SDL_KEYUP: {
            keybinds_on_up(ev.key.keysym.sym);
        } break;
        case SDL_TEXTINPUT: {
            if (choosing_filename) {
                if (!strcmp(filename, "Choose a filename!")) {
                    filename = calloc(1024, sizeof(char));
                }
                strncat(filename, ev.text.text, 1024);
            } else {
                buffer_ins_cursor(&context, ev.text.text);
            }
        }
        }
        break;
    }
    return false;
}
