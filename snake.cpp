#include <SDL2/SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Snake",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Viewport
    SDL_Rect viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.w = WINDOW_WIDTH;
    viewport.h = WINDOW_HEIGHT;
    SDL_RenderSetViewport(renderer, &viewport);

    // Set the logical size of the renderer
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Set the color for clearing the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color
    SDL_RenderClear(renderer);

    // Set the color for the viewport
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderFillRect(renderer, &viewport);

    // Present the renderer to show the initial viewport
    SDL_RenderPresent(renderer);

    // Initialize the snake
    const int SNAKE_WIDTH = 20;
    const int SNAKE_HEIGHT = 20;

    // Set the initial position of the snake
    SDL_Rect snake;
    snake.x = WINDOW_WIDTH / 2 - SNAKE_WIDTH / 2;
    snake.y = WINDOW_HEIGHT / 2 - SNAKE_HEIGHT / 2;
    snake.w = SNAKE_WIDTH;
    snake.h = SNAKE_HEIGHT;

    // Render the initial snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color for the snake
    SDL_RenderFillRect(renderer, &snake);

    // Render the border around the snake
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for the border
    SDL_RenderDrawRect(renderer, &snake); // Draw a border around the snake

    // Present the renderer to show the initial snake
    SDL_RenderPresent(renderer);

    // Main game loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Handle keyboard input for snake movement
        // Get the current state of the keyboard
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            snake.y -= 10;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            snake.y += 10;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            snake.x -= 10;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            snake.x += 10;
        }

        // Ensure the snake stays within the viewport
        if (snake.x < 0) snake.x = 0;
        if (snake.y < 0) snake.y = 0;
        if (snake.x + snake.w > WINDOW_WIDTH) snake.x = WINDOW_WIDTH - snake.w;
        if (snake.y + snake.h > WINDOW_HEIGHT) snake.y = WINDOW_HEIGHT - snake.h;

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the snake again
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &snake);

        // Render the border around the snake
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for the border
        SDL_RenderDrawRect(renderer, &snake); // Draw border around the snake
        SDL_RenderPresent(renderer);

        // Delay to control the frame rate
        SDL_Delay(100); // Adjust the delay as needed for your game speed
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}