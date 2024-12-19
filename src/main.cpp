#include "raylib.h"
#include "raymath.h"
#include "utils.h"

#include <iostream>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

const float PIXELS_PER_METER = 100.0f;

struct MotionState {
    Vector2 pos;
    Vector2 v;
    Vector2 a;
};

void updateMotionState(MotionState& motionState, float ticks) {
    motionState.pos = Vector2Add(Vector2Scale(motionState.v, ticks), motionState.pos);
    motionState.v = Vector2Add(Vector2Scale(motionState.a, ticks), motionState.v);
}

MotionState initBallMotionState(float ballRadius) {
    return MotionState {
        .pos = Vector2{ballRadius+5, ballRadius+5},
        .v = Vector2{0.5f*PIXELS_PER_METER, 0},
        .a = Vector2{0, 9.8f*PIXELS_PER_METER}
    };
}

const int screenWidth = 1024;
const int screenHeight = 768;

float ballRadius = screenWidth*0.03;

MotionState ballMotionState = initBallMotionState(ballRadius);

Rectangle ground = {.x = 0, .y = screenHeight-10, .width = screenWidth, .height = 10};

float btnWidth = screenWidth * 0.1f;
Rectangle resetBtnRect{
    .width = btnWidth,
    .height = 20,
    .x = screenWidth - btnWidth - 5,
    .y = 5
};

float ball_restitution = 0.8f;

Vector2 mousePoint;
bool btnAction = false;
unsigned char btnState = 0;

void UpdateDrawFrame(void);

int main(void)
{

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    CloseWindow();

    return 0;
}

void UpdateDrawFrame(void) {
    // Update
    btnAction = false;
    mousePoint = GetMousePosition();
    if (CheckCollisionPointRec(mousePoint, resetBtnRect)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
        else btnState = 1;

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
    } else btnState = 0;

    if (btnAction) ballMotionState = initBallMotionState(ballRadius);

    if (CheckCollisionCircleRec(ballMotionState.pos, ballRadius, ground)) {
        ballMotionState.v.y = -ball_restitution*ballMotionState.v.y;
    }
    updateMotionState(ballMotionState, 1.0f/60.0f);
    
    // std::cout << "ballx: " << ballMotionState.pos.x << ", bally: " << ballMotionState.pos.y << '\n';


    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTextBoxed(GetFontDefault(), "reset", resetBtnRect, 12, 1, true, BLACK);

        DrawCircleV(ballMotionState.pos, ballRadius, MAROON);

        DrawRectangleRec(ground, ORANGE);

    EndDrawing();
}