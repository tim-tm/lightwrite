#if defined (_WIN32) || defined (WIN32)
#error "lightwrite can not yet be compiled for any version of windows."
#endif /* if windows */

#include "buffer.h"
#include "font.h"
#include "keybinds.h"
#include "logger.h"
#include "filemanager.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FONT_PATH DATA_DIR "/MonoLisaRegular.ttf"

static int screen_width = 800;
static int screen_height = 600;
static const SDL_Color text_color = {255, 255, 255, 255};

static SDL_Window* window;
static SDL_Renderer* renderer;
static TTF_Font* font;
static Buffer_Context context;

static char* filename;
static bool choosing_filename;
static bool file_saved;

static File_Manager file_man;
static bool file_man_opened;
static size_t file_man_cursor;

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
        FILE* fp = fopen(argv[1], "r");
        // buffer_write will automatically create the file anyways.
        if (fp) {
            file_man.files[0].ptr = fp;
            strcpy(file_man.files[0].name, filename);
            file_man.size++;
                
            buffer_read(&context, fp);
        }
    }

    // Moving some variable decls up here for performance reasons.
    int width; // used for font-width
    int height; // used for font-height
    
    Font_Data data;
    SDL_Rect rect;
    
    char substr[MAX_LINE_SIZE];
    
    char name[1024] = "<untitled>";
    int status_height = 18;

    bool closed = false;
	while (!closed) {
		closed = handle_events();
        
        SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255);
		SDL_RenderClear(renderer);

        if (file_man_opened) {
            rect.x = 0;
            rect.y = file_man_cursor * data.font_h;
            rect.w = screen_width;
            rect.h = data.font_h +1;
            SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
            SDL_RenderFillRect(renderer, &rect);
            
            for (size_t i = 0; i <= file_man.size; ++i) {
                File f = file_man.files[i];
                if (!f.ptr) continue;

                data = prepare_string(font, renderer, 0,
                    i * data.font_h, f.name, text_color);
                SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);
                SDL_DestroyTexture(data.texture);
            }
            
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

            // Do not draw the rest of the frame
            SDL_RenderPresent(renderer);
            continue;
        }

        rect.x = 0;
        rect.y = 0;
        rect.w = screen_width;
        rect.h = status_height;
        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
        SDL_RenderFillRect(renderer, &rect);
        
        if (filename) {
            strcpy(name, filename);
        }

        data = prepare_string(font, renderer, 1, 1, name, text_color);
        SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);
        SDL_DestroyTexture(data.texture);   

        if (!file_saved) {
            TTF_SizeText(font, name, &width, &height);
            
            SDL_RenderSetScale(renderer, 0.9f, 0.9f);
            data = prepare_string(font, renderer, (2 + width) * 1.1f, 2, "*", text_color);
            SDL_RenderCopy(renderer, data.texture, NULL, &data.rect);
            SDL_DestroyTexture(data.texture);   
            SDL_RenderSetScale(renderer, 1.f, 1.f);
        }

		for (size_t i = 0; i < context.size; ++i) {
            TTF_SizeText(font, context.lines[i].buffer, &width, &height);

            data = prepare_string(font, renderer, 0,
			    status_height + (i * data.font_h), context.lines[i].buffer, text_color);
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
		rect.y = status_height + (context.cursor * height);
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
        LOG_FATAL("TTF-Error: %s", TTF_GetError());
		SDL_Quit();
		return false;
	}

	window = SDL_CreateWindow("lightwrite", SDL_WINDOWPOS_CENTERED,
	    SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
	    SDL_WINDOW_RESIZABLE);
	if (!window) {
		LOG_FATAL("SDL-Error: %s", SDL_GetError());
        SDL_Quit();
        TTF_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(
	    window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!renderer) {
        LOG_FATAL("SDL-Error: %s", SDL_GetError());
        SDL_Quit();
        TTF_Quit();
		SDL_DestroyWindow(window);
		return false;
	}

	font = TTF_OpenFont(FONT_PATH, 16);
	if (!font) {
        LOG_FATAL("TTF-Error: %s", TTF_GetError());
        SDL_Quit();
        TTF_Quit();
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		return false;
	}

	buffer_init(&context);
    if (!logger_init()) return false;
    if (!keybinds_init()) return false;
    fileman_init(&file_man);
    return true;
}

static void destroy_all(void) {
    fileman_destroy(&file_man);
    keybinds_destroy();
    logger_destroy();
	buffer_free(&context);
	TTF_CloseFont(font);
	TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	SDL_Quit();
    // NOTE: Filename doesnt need to be free'ed
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
                if (file_man_opened) {
                    if (file_man_cursor > 0) {
                        file_man_cursor--;
                    }
                    continue;
                }

                if (context.cursor > 0) {
                    context.cursor--;
                    context.lines[context.cursor].cursor =
                        context.lines[context.cursor].size;
                }
            } break;
            case SDLK_DOWN: {
                if (file_man_opened) {
                    if (file_man_cursor < file_man.size-1) {
                        file_man_cursor++;
                    }
                    continue;
                }

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
                if (file_man_opened) {
                    if (choosing_filename) {
                        if (!fileman_create(&file_man, filename)) {
                            // TODO: Print some error message to the screen.
                        }
                        filename = NULL;
                        choosing_filename = false;
                    } else {
                        if (file_man.files[file_man_cursor].ptr) {
                            buffer_read(&context, file_man.files[file_man_cursor].ptr);
                            filename = file_man.files[file_man_cursor].name;
                            file_man_opened = false;
                        }
                    }
                    continue;
                }

                if (choosing_filename) {
                    buffer_write(&context, file_man.files[0].ptr, filename);
                    LOG_INFO("%s saved!", filename);
                    choosing_filename = false;
                    file_saved = true;
                } else {
                    buffer_push_line(&context);
                }
            } break;
            case SDLK_s: {
                // do not check for file*, buffer_write will create the class anyways.
                if (keybinds_is_down(SDLK_LCTRL)) {
                    if (filename) {
                        buffer_write(&context, file_man.files[0].ptr, filename);
                        LOG_INFO("%s saved!", filename);
                        file_saved = true;
                    } else {
                        filename = "Choose a filename!";
                        choosing_filename = true;
                    }
                }
            } break;
            case SDLK_f: {
                // Filemanager keybind: Ctrl + Shift + f
                if (keybinds_is_down(SDLK_LCTRL) && keybinds_is_down(SDLK_LSHIFT)) {
                    file_man_opened = !file_man_opened;
                }
            } break;
            case SDLK_a: {
                // File creation keybind: Ctrl + a (in Filemanager)
                if (keybinds_is_down(SDLK_LCTRL) && file_man_opened && !filename) {
                    filename = "Choose a filename!";
                    choosing_filename = true;
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
            } else if (!file_man_opened) {
                buffer_ins_cursor(&context, ev.text.text);
                file_saved = false;
            }
        } break;
        case SDL_WINDOWEVENT: {
            if (ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED && renderer) {
                screen_width = ev.window.data1;
                screen_height = ev.window.data2;
                SDL_RenderSetLogicalSize(renderer, screen_width, screen_height);
            }
        } break;
        }
        break;
    }
    return false;
}
