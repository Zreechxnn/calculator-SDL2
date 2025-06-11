#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

struct Button {
    SDL_Rect rect;
    char label[10];
    SDL_Color color;
    SDL_Color textColor;
};

void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* rect) {
    if (strlen(text) == 0) {
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_Rect textRect = {rect->x + (rect->w - text_width) / 2, rect->y + (rect->h - text_height) / 2, text_width, text_height};

    SDL_RenderCopy(renderer, texture, NULL, &textRect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void skipSpaces(char** p) {
    while (**p == ' ' || **p == '\t') {
        (*p)++;
    }
}

double parseNumber(char** p) {
    char* end;
    errno = 0;
    double num = strtod(*p, &end);
    if (errno != 0 || *p == end) {
        return NAN;
    }
    *p = end;
    return num;
}

double parseExpression(char** p);

double parseBase(char** p) {
    skipSpaces(p);
    double result = NAN;
    
    if (isdigit(**p) || **p == '.') {
        result = parseNumber(p);
    } else if (isalpha(**p)) {
        char func[10] = {0};
        int i = 0;
        while (isalpha(**p) && i < 9) {
            func[i++] = **p;
            (*p)++;
        }
        func[i] = '\0';
        
        if (strcmp(func, "pi") == 0) {
            result = M_PI;
        } else if (strcmp(func, "e") == 0) {
            result = M_E;
        } else {
            skipSpaces(p);
            if (**p != '(') {
                return NAN;
            }
            (*p)++;
            double arg = parseExpression(p);
            if (isnan(arg)) {
                return NAN;
            }
            skipSpaces(p);
            if (**p != ')') {
                return NAN;
            }
            (*p)++;
            
            if (strcmp(func, "sin") == 0) {
                result = sin(arg);
            } else if (strcmp(func, "cos") == 0) {
                result = cos(arg);
            } else if (strcmp(func, "tan") == 0) {
                result = tan(arg);
            } else if (strcmp(func, "log") == 0) {
                result = log10(arg);
            } else if (strcmp(func, "ln") == 0) {
                result = log(arg);
            } else if (strcmp(func, "sqrt") == 0) {
                if (arg < 0) return NAN;
                result = sqrt(arg);
            } else {
                return NAN;
            }
        }
    } else if (**p == '(') {
        (*p)++;
        result = parseExpression(p);
        skipSpaces(p);
        if (**p != ')') {
            return NAN;
        }
        (*p)++;
    } else if (**p == '-') {
        (*p)++;
        result = -parseBase(p);
    } else if (**p == '+') {
        (*p)++;
        result = parseBase(p);
    }
    skipSpaces(p);
    return result;
}

double parsePower(char** p) {
    double left = parseBase(p);
    if (isnan(left)) {
        return NAN;
    }
    skipSpaces(p);
    if (**p == '^') {
        (*p)++;
        double exponent = parsePower(p);
        if (isnan(exponent)) {
            return NAN;
        }
        left = pow(left, exponent);
    }
    return left;
}

double parseFactor(char** p) {
    return parsePower(p);
}

double parseTerm(char** p) {
    double left = parseFactor(p);
    if (isnan(left)) {
        return NAN;
    }
    skipSpaces(p);
    while (**p == '*' || **p == '/') {
        char op = **p;
        (*p)++;
        double right = parseFactor(p);
        if (isnan(right)) {
            return NAN;
        }
        if (op == '*') {
            left *= right;
        } else {
            if (fabs(right) < 1e-12) {
                return NAN;
            }
            left /= right;
        }
        skipSpaces(p);
    }
    return left;
}

double parseExpression(char** p) {
    double left = parseTerm(p);
    if (isnan(left)) {
        return NAN;
    }
    skipSpaces(p);
    while (**p == '+' || **p == '-') {
        char op = **p;
        (*p)++;
        double right = parseTerm(p);
        if (isnan(right)) {
            return NAN;
        }
        if (op == '+') {
            left += right;
        } else {
            left -= right;
        }
        skipSpaces(p);
    }
    return left;
}

double evaluateExpression(const char* expression) {
    if (strlen(expression) == 0) return NAN;
    
    char* exp = strdup(expression);
    if (!exp) return NAN;
    
    char* p = exp;
    double result = parseExpression(&p);
    
    if (!isnan(result)) {
        skipSpaces(&p);
        if (*p != '\0') {
            result = NAN;
        }
    }
    
    free(exp);
    return result;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        return -1;
    }
    if (TTF_Init() < 0) {
        printf("Could not initialize SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Scientific Calculator", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        360, 520, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("assets/lonely.ttf", 24);
    if (!font) {
        font = TTF_OpenFont("arial.ttf", 24);
    }
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    struct Button buttons[29] = {
        {{10, 110, 60, 50}, "sin", {128, 0, 128, 255}, {255, 255, 255, 255}},
        {{80, 110, 60, 50}, "cos", {128, 0, 128, 255}, {255, 255, 255, 255}},
        {{150, 110, 60, 50}, "tan", {128, 0, 128, 255}, {255, 255, 255, 255}},
        {{220, 110, 60, 50}, "log", {128, 0, 128, 255}, {255, 255, 255, 255}},
        {{290, 110, 60, 50}, "ln", {128, 0, 128, 255}, {255, 255, 255, 255}},

        {{10, 170, 60, 50}, "sqrt", {128, 0, 128, 255}, {255, 255, 255, 255}},
        {{80, 170, 60, 50}, "pi", {0, 200, 0, 255}, {255, 255, 255, 255}},
        {{150, 170, 60, 50}, "e", {0, 200, 0, 255}, {255, 255, 255, 255}},
        {{220, 170, 60, 50}, "(", {255, 165, 0, 255}, {255, 255, 255, 255}},
        {{290, 170, 60, 50}, ")", {255, 165, 0, 255}, {255, 255, 255, 255}},

        {{10, 230, 60, 50}, "7", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{80, 230, 60, 50}, "8", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{150, 230, 60, 50}, "9", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{220, 230, 60, 50}, "DEL", {255, 0, 0, 255}, {255, 255, 255, 255}},
        {{290, 230, 60, 50}, "C", {255, 0, 0, 255}, {255, 255, 255, 255}},

        {{10, 290, 60, 50}, "4", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{80, 290, 60, 50}, "5", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{150, 290, 60, 50}, "6", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{220, 290, 60, 50}, "*", {255, 100, 100, 255}, {255, 255, 255, 255}},
        {{290, 290, 60, 50}, "/", {255, 100, 100, 255}, {255, 255, 255, 255}},

        {{10, 350, 60, 50}, "1", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{80, 350, 60, 50}, "2", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{150, 350, 60, 50}, "3", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{220, 350, 60, 50}, "+", {255, 100, 100, 255}, {255, 255, 255, 255}},
        {{290, 350, 60, 50}, "-", {255, 100, 100, 255}, {255, 255, 255, 255}},

        {{10, 410, 60, 50}, "0", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{80, 410, 60, 50}, ".", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{150, 410, 60, 50}, "^", {255, 100, 100, 255}, {255, 255, 255, 255}},
        {{220, 410, 130, 50}, "=", {0, 200, 0, 255}, {255, 255, 255, 255}}
    };

    char currentExpression[200] = "";
    SDL_Color displayColor = {255, 255, 255, 255};
    SDL_Rect displayRect = {10, 10, 340, 90};
    bool running = true;
    
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x;
                int y = e.button.y;

                for (int i = 0; i < 29; i++) {
                    if (x >= buttons[i].rect.x && x <= (buttons[i].rect.x + buttons[i].rect.w) &&
                        y >= buttons[i].rect.y && y <= (buttons[i].rect.y + buttons[i].rect.h)) {
                        
                        if (strcmp(buttons[i].label, "=") == 0) {
                            double result = evaluateExpression(currentExpression);
                            if (isnan(result)) {
                                strcpy(currentExpression, "Error");
                            } else {
                                snprintf(currentExpression, sizeof(currentExpression), "%g", result);
                            }
                        } else if (strcmp(buttons[i].label, "C") == 0) {
                            currentExpression[0] = '\0';
                        } else if (strcmp(buttons[i].label, "DEL") == 0) {
                            size_t len = strlen(currentExpression);
                            if (len > 0) {
                                currentExpression[len - 1] = '\0';
                            }
                        } else if (strcmp(buttons[i].label, "sin") == 0 ||
                                   strcmp(buttons[i].label, "cos") == 0 ||
                                   strcmp(buttons[i].label, "tan") == 0 ||
                                   strcmp(buttons[i].label, "log") == 0 ||
                                   strcmp(buttons[i].label, "ln") == 0 ||
                                   strcmp(buttons[i].label, "sqrt") == 0) {
                            strcat(currentExpression, buttons[i].label);
                            strcat(currentExpression, "(");
                        } else {
                            if (strlen(currentExpression) + strlen(buttons[i].label) < sizeof(currentExpression)) {
                                strcat(currentExpression, buttons[i].label);
                            }
                        }
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
        SDL_RenderClear(renderer);
        
        for (int i = 0; i < 29; i++) {
            SDL_SetRenderDrawColor(renderer, 
                buttons[i].color.r, 
                buttons[i].color.g, 
                buttons[i].color.b, 
                buttons[i].color.a);
            SDL_RenderFillRect(renderer, &buttons[i].rect);
            
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
            SDL_RenderDrawRect(renderer, &buttons[i].rect);
            
            drawText(renderer, font, buttons[i].label, 
                buttons[i].textColor, &buttons[i].rect);
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 60, 255);
        SDL_RenderFillRect(renderer, &displayRect);
        
        SDL_SetRenderDrawColor(renderer, 100, 100, 200, 255);
        SDL_RenderDrawRect(renderer, &displayRect);
        
        drawText(renderer, font, currentExpression, displayColor, &displayRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}