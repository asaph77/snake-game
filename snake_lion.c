#include "raylib.h"
#include <stdlib.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define CELL_SIZE 20
#define MAX_SNAKE_LENGTH 1000

typedef struct {
    int x, y;
} Vector2i;

void ResetGame(Vector2i *snake, int *snakeLength, Vector2i *fruit, Vector2i *dir, bool *gameOver, int *score, float *fruitScale, int *fps) {
    *snakeLength = 3;
    *dir = (Vector2i){1, 0};
    *score = 0;
    *fruitScale = 1.0f;
    *fps = 10;
    *gameOver = false;

    for (int i = 0; i < *snakeLength; i++) {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }

    *fruit = (Vector2i){
        GetRandomValue(0, SCREEN_WIDTH / CELL_SIZE - 1),
        GetRandomValue(0, SCREEN_HEIGHT / CELL_SIZE - 1)
    };
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Lion Game");
    SetTargetFPS(10);

    Vector2i snake[MAX_SNAKE_LENGTH];
    Vector2i direction = {1, 0};
    Vector2i fruit;
    int snakeLength = 3;
    int score = 0;
    float fruitScale = 1.0f;
    int fps = 10;

    bool gameOver = false;
    bool gameStarted = false;

    ResetGame(snake, &snakeLength, &fruit, &direction, &gameOver, &score, &fruitScale, &fps);

    while (!WindowShouldClose()) {
        SetTargetFPS(fps);

        BeginDrawing();
        ClearBackground(DARKGREEN);

        if (!gameStarted) {
            DrawText("SNAKE LION GAME", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 50, 30, GOLD);
            DrawText("Appuyez sur ENTREE pour commencer", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, 20, RAYWHITE);
            if (IsKeyPressed(KEY_ENTER)) gameStarted = true;
            EndDrawing();
            continue;
        }

        if (!gameOver) {
            
            if (IsKeyPressed(KEY_UP) && direction.y == 0) direction = (Vector2i){0, -1};
            if (IsKeyPressed(KEY_DOWN) && direction.y == 0) direction = (Vector2i){0, 1};
            if (IsKeyPressed(KEY_LEFT) && direction.x == 0) direction = (Vector2i){-1, 0};
            if (IsKeyPressed(KEY_RIGHT) && direction.x == 0) direction = (Vector2i){1, 0};

            
            for (int i = snakeLength; i > 0; i--) {
                snake[i] = snake[i - 1];
            }
            snake[0].x += direction.x;
            snake[0].y += direction.y;

            
            if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
                snakeLength++;
                score += 10;
                fps += 1; 
                fruit.x = GetRandomValue(0, SCREEN_WIDTH / CELL_SIZE - 1);
                fruit.y = GetRandomValue(0, SCREEN_HEIGHT / CELL_SIZE - 1);
            }

            
            if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH / CELL_SIZE ||
                snake[0].y < 0 || snake[0].y >= SCREEN_HEIGHT / CELL_SIZE) {
                gameOver = true;
            }

            
            for (int i = 1; i < snakeLength; i++) {
                if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
                    gameOver = true;
                    break;
                }
            }

            
            fruitScale = 1.0f + 0.1f * sinf(GetTime() * 5);
            int fruitSize = CELL_SIZE * fruitScale;
            DrawRectangle(fruit.x * CELL_SIZE + (CELL_SIZE - fruitSize) / 2,
                          fruit.y * CELL_SIZE + (CELL_SIZE - fruitSize) / 2,
                          fruitSize, fruitSize, RED);

           
            for (int i = 0; i < snakeLength; i++) {
                int px = snake[i].x * CELL_SIZE;
                int py = snake[i].y * CELL_SIZE;

                if (i == 0) {
                    DrawRectangle(px, py, CELL_SIZE, CELL_SIZE, GOLD);
                    DrawText("🦁", px + 1, py - 2, CELL_SIZE, BROWN);
                } else {
                    DrawRectangle(px, py, CELL_SIZE, CELL_SIZE, LIME);
                }
            }

            
            DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH / 2 - 50, 5, 24, YELLOW);
        } else {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 - 60, 40, RED);
            DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 20, WHITE);
            DrawText("Appuyez sur R pour rejouer", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 40, 20, LIGHTGRAY);

            if (IsKeyPressed(KEY_R)) {
                ResetGame(snake, &snakeLength, &fruit, &direction, &gameOver, &score, &fruitScale, &fps);
            }
        }

      
        for (int x = 0; x < SCREEN_WIDTH; x += CELL_SIZE) {
            DrawLine(x, 0, x, SCREEN_HEIGHT, Fade(GREEN, 0.1f));
        }
        for (int y = 0; y < SCREEN_HEIGHT; y += CELL_SIZE) {
            DrawLine(0, y, SCREEN_WIDTH, y, Fade(GREEN, 0.1f));
        }
        for (int i = 0; i < snakeLength; i++) {
    int px = snake[i].x * CELL_SIZE + CELL_SIZE / 2;
    int py = snake[i].y * CELL_SIZE + CELL_SIZE / 2;

    if (i == 0) {
        // Tête du lion (cerveau stylisé)
        DrawCircle(px, py, CELL_SIZE / 2, GOLD);
        DrawText("🦁", px - 8, py - 12, CELL_SIZE, BROWN);
    } else {
        // Corps en dégradé jaune/orange
        Color bodyColor = (Color){ 255, 200 - i % 100, 50, 255 };
        DrawCircle(px, py, CELL_SIZE / 2, bodyColor);
    }
}

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
