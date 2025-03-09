#include "raylib.h"

int main() {
  // Window constants
  const int WINDOW_WIDTH = 512;
  const int WINDOW_HEIGHT = 512;
  const char WINDOW_NAME[] = "Dasher Game";

  // Window initialization
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME);
  SetTargetFPS(60);

  // World paramters
  const int GRAVITY = 1000;
  int velocity = 0;
  int scarfy_frame = 0;
  int nebula_frame = 0;
  const float SCARFY_UPDATE_TIME = 1.0 / 12.0;
  const float NEBULA_UPDATE_TIME = 1.0 / 12.0;
  float scarfy_running_time = 0;
  float nebula_running_time = 0;

  // Scarfy parameters
  Texture2D scarfy = LoadTexture("textures/scarfy.png");
  Rectangle scarfyRec{0, 0, static_cast<float>(scarfy.width / 6), static_cast<float>(scarfy.height)};
  Vector2 scarfyPos{(WINDOW_WIDTH / 2) - (scarfyRec.width / 2), WINDOW_HEIGHT - scarfyRec.height};

  // Nebula paramters
  Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
  Rectangle nebulaRec{0, 0, static_cast<float>(nebula.width / 8), static_cast<float>(nebula.height / 8)};
  Vector2 nebulaPos{WINDOW_WIDTH, WINDOW_HEIGHT - (nebulaRec.height)};

  bool is_in_air = false;
  const int JUMP_VAL = -600;
  const int NEB_VEL = -200;

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(WHITE);

    float delta_time = GetFrameTime();

    if (scarfyPos.y >= WINDOW_HEIGHT - scarfyRec.height) {
      velocity = 0;
      is_in_air = false;
    } else {
      velocity += GRAVITY * delta_time;
    }

    if (IsKeyPressed(KEY_SPACE) && !is_in_air) {
      velocity += JUMP_VAL;
      is_in_air = true;
    }

    scarfyPos.y += velocity * delta_time;
    nebulaPos.x += NEB_VEL * delta_time;

    if(!is_in_air) {
      scarfy_running_time += delta_time;

      if(scarfy_running_time >= SCARFY_UPDATE_TIME) {
        scarfy_frame++;

        if (scarfy_frame > 5) {
          scarfy_frame = 0;
        }

        scarfyRec.x = scarfyRec.width * scarfy_frame;
        scarfy_running_time = 0;
      }
    }

    nebula_running_time += delta_time;

    if(nebula_running_time >= NEBULA_UPDATE_TIME) {
      nebula_frame++;

      if (nebula_frame > 7) {
        nebula_frame = 0;
      }

      nebulaRec.x = nebulaRec.width * nebula_frame;
      nebula_running_time = 0;
    }


    DrawTextureRec(nebula, nebulaRec, nebulaPos, WHITE);
    DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);
    
    EndDrawing();
  }

  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  CloseWindow();
};