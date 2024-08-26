#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>



struct Button {
    SDL_Rect rect;
    char label[10];
    SDL_Color color;
    SDL_Color textColor;
};

void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, SDL_Rect* rect) {
    if (strlen(text) == 0) {
        printf("Text is empty, skipping rendering.\n");
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
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

// Function to evaluate the expression (simplified for the example)
double evaluateExpression(const char* expression) {
    double result = 0;
    char op = 0;
    double operand = 0;
    const char* p = expression;

    while (*p) {
        if (isdigit(*p) || (*p == '.' && isdigit(*(p + 1)))) {
            operand = strtod(p, (char**)&p);
            switch (op) {
                case '+': result += operand; break;
                case '-': result -= operand; break;
                case '*': result *= operand; break;
                case '/': result /= operand; break;
                default: result = operand; break;
            }
        } else {
            op = *p;
            p++;
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0) {
        printf("Could not initialize SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Calculator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 360, 460, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("lonely.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Define buttons
    struct Button buttons[20] = {
        {{10, 110, 80, 50}, "7", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{100, 110, 80, 50}, "8", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{190, 110, 80, 50}, "9", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{280, 110, 80, 50}, "/", {255, 100, 100, 255}, {255, 255, 255, 255}},
        {{10, 170, 80, 50}, "4", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{100, 170, 80, 50}, "5", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{190, 170, 80, 50}, "6", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{280, 170, 80, 50}, "*", {255, 100, 100, 255}, {255, 255, 255, 255}},
        {{10, 230, 80, 50}, "1", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{100, 230, 80, 50}, "2", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{190, 230, 80, 50}, "3", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{280, 230, 80, 50}, "-", {255, 100, 100, 255}, {255, 255, 255, 255}},
        {{10, 290, 80, 50}, "0", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{100, 290, 80, 50}, ".", {0, 128, 255, 255}, {255, 255, 255, 255}},
        {{190, 290, 80, 50}, "C", {255, 100, 100, 255}, {255, 255, 255, 255}},
        {{280, 290, 80, 50}, "+", {255, 100, 100, 255}, {255, 255, 255, 255}},
        {{10, 350, 170, 50}, "=", {0, 255, 0, 255}, {255, 255, 255, 255}},
        {{190, 350, 170, 50}, "DEL", {255, 0, 0, 255}, {255, 255, 255, 255}},
    };

    char currentExpression[100] = "";
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

                for (int i = 0; i < 20; i++) {
                    if (x >= buttons[i].rect.x && x <= (buttons[i].rect.x + buttons[i].rect.w) &&
                        y >= buttons[i].rect.y && y <= (buttons[i].rect.y + buttons[i].rect.h)) {
                        if (strcmp(buttons[i].label, "=") == 0) {
                            double result = evaluateExpression(currentExpression);
                            snprintf(currentExpression, sizeof(currentExpression), "%g", result);
                        } else if (strcmp(buttons[i].label, "C") == 0) {
                            memset(currentExpression, 0, sizeof(currentExpression));
                        } else if (strcmp(buttons[i].label, "DEL") == 0) {
                            size_t len = strlen(currentExpression);
                            if (len > 0) {
                                currentExpression[len - 1] = '\0';
                            }
                        } else {
                            if (strlen(currentExpression) + strlen(buttons[i].label) < sizeof(currentExpression)) {
                                strcat(currentExpression, buttons[i].label);
                            }
                        }
                    }
                }
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255); // Dark blue background
        SDL_RenderClear(renderer);

        // Draw buttons
        for (int i = 0; i < 20; i++) {
            SDL_SetRenderDrawColor(renderer, buttons[i].color.r, buttons[i].color.g, buttons[i].color.b, buttons[i].color.a);
            SDL_RenderFillRect(renderer, &buttons[i].rect);
            drawText(renderer, font, buttons[i].label, buttons[i].textColor, &buttons[i].rect);
        }

        // Render the current expression
        drawText(renderer, font, currentExpression, displayColor, &displayRect);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
