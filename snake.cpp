#include <SDL2/SDL.h>
#include <iostream>

// This is a simple SDL2 Snake game implementation in C++.
// It initializes SDL, creates a window and renderer, and allows the player to control a snake
// using the arrow keys. The snake is represented as a rectangle that can move around the screen.

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void renderApple(SDL_Renderer* renderer, const SDL_Rect& apple) {
    // Set the color for the apple
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    // Render the apple as a filled rectangle
    SDL_RenderFillRect(renderer, &apple);
    // Set the color for the border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    // Draw a border around the apple
    SDL_RenderDrawRect(renderer, &apple);
}

// Function to render the snake
void renderSnake(SDL_Renderer* renderer, const SDL_Rect& snake) {
    // Set the color for the snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
    // Render the snake as a filled rectangle
    SDL_RenderFillRect(renderer, &snake);
    // Set the color for the border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    // Draw a border around the snake
    SDL_RenderDrawRect(renderer, &snake);
}

// Function to handle keyboard input for snake movement
void handleInput(SDL_Rect& snake) {
    // Get the current state of the keyboard
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_UP]) {
        snake.y -= 10; // Move up
    }
    if (currentKeyStates[SDL_SCANCODE_DOWN]) {
        snake.y += 10; // Move down
    }
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        snake.x -= 10; // Move left
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        snake.x += 10; // Move right
    }
    // Ensure the snake stays within the viewport
    if (snake.x < 0) snake.x = 0;
    if (snake.y < 0) snake.y = 0;
    if (snake.x + snake.w > WINDOW_WIDTH) snake.x = WINDOW_WIDTH - snake.w;
    if (snake.y + snake.h > WINDOW_HEIGHT) snake.y = WINDOW_HEIGHT - snake.h;
}


// Function to initialize SDL and create a window and renderer
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

    // Initialize the apple
    const int APPLE_WIDTH = 20;
    const int APPLE_HEIGHT = 20;
    
    // Set the initial position of the apple
    SDL_Rect apple;
    apple.x = rand() % (WINDOW_WIDTH - APPLE_WIDTH);
    apple.y = rand() % (WINDOW_HEIGHT - APPLE_HEIGHT);
    apple.w = APPLE_WIDTH;
    apple.h = APPLE_HEIGHT;

    // Initialize the snake
    const int SNAKE_WIDTH = 20;
    const int SNAKE_HEIGHT = 20;

    // Set the initial position of the snake
    SDL_Rect snake;
    snake.x = WINDOW_WIDTH / 2 - SNAKE_WIDTH / 2;
    snake.y = WINDOW_HEIGHT / 2 - SNAKE_HEIGHT / 2;
    snake.w = SNAKE_WIDTH;
    snake.h = SNAKE_HEIGHT;

    // Render the initial apple
    renderApple(renderer, apple);

    // Render the initial snake
    renderSnake(renderer, snake);

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
        handleInput(snake);

        // Check for collision with the apple
        if (SDL_HasIntersection(&snake, &apple)) {
            // Move the apple to a new random position
            apple.x = rand() % (WINDOW_WIDTH - apple.w);
            apple.y = rand() % (WINDOW_HEIGHT - apple.h);
        }

        // Clear the renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the apple and snake
        renderApple(renderer, apple);
        renderSnake(renderer, snake);

        // Present the renderer to show the updated state
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