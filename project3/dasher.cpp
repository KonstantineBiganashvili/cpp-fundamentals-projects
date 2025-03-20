#include "raylib.h"

struct AnimData
{
  Rectangle rect;
  Vector2 pos;
  int frame;
  float updateTime;
  float runningTime;
};

bool isOnground(AnimData data, float windowHeight)
{
  return data.pos.y >= windowHeight - data.rect.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int nextFrame)
{
  data.runningTime += deltaTime;

  if (data.runningTime >= data.updateTime)
  {
    data.frame++;

    if (data.frame > nextFrame)
    {
      data.frame = 0;
    }

    data.rect.x = data.rect.width * data.frame;
    data.runningTime = 0;
  }

  return data;
}

int main()
{
  // Window constants
  float WINDOW_DIMENSIONS[2];
  WINDOW_DIMENSIONS[0] = 512.0;
  WINDOW_DIMENSIONS[1] = 380.0;
  const char WINDOW_NAME[] = "Dasher Game";

  // Window initialization
  InitWindow(WINDOW_DIMENSIONS[0], WINDOW_DIMENSIONS[1], WINDOW_NAME);
  SetTargetFPS(60);

  // World paramters
  const int GRAVITY = 1000;
  int velocity = 0;

  // Background image
  Texture2D background = LoadTexture("textures/far-buildings.png");
  Texture2D midground = LoadTexture("textures/back-buildings.png");
  Texture2D foreground = LoadTexture("textures/foreground.png");
  float bgX = 0.0;
  float mgX = 0.0;
  float fgX = 0.0;

  // AnimData for scarfy
  Texture2D scarfy = LoadTexture("textures/scarfy.png");
  AnimData scarfyData;
  scarfyData.rect.width = scarfy.width / 6;
  scarfyData.rect.height = scarfy.height;
  scarfyData.rect.x = 0;
  scarfyData.rect.y = 0;
  scarfyData.pos.x = (WINDOW_DIMENSIONS[0] / 2) - (scarfyData.rect.width / 2);
  scarfyData.pos.y = WINDOW_DIMENSIONS[1] - scarfyData.rect.height;
  scarfyData.frame = 0;
  scarfyData.updateTime = 1.0 / 12.0;
  scarfyData.runningTime = 0.0;

  // Animdata for nebula
  Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

  // nebulae initialization
  const int NEBULA_COUNT = 5;
  AnimData nebulae[NEBULA_COUNT]{};

  for (int i = 0; i < NEBULA_COUNT; i++)
  {
    int modulus;
    modulus = i % 2;

    nebulae[i].rect.x = 0.0;
    nebulae[i].rect.y = 0.0;
    nebulae[i].rect.width = static_cast<float>(nebula.width / 8);
    nebulae[i].rect.height = static_cast<float>(nebula.height / 8);
    nebulae[i].pos.x = WINDOW_DIMENSIONS[0] + (i * 300);
    nebulae[i].pos.y = static_cast<float>(WINDOW_DIMENSIONS[1] - nebula.height / 8);
    nebulae[i].frame = 0;
    nebulae[i].runningTime = 0.0;

    if (modulus == 1)
    {
      nebulae[i].updateTime = (1.0 / (12.0 + 4.0));
    }
    else
    {
      nebulae[i].updateTime = (1.0 / 12.0);
    }
  }

  float finishLine{nebulae[NEBULA_COUNT - 1].pos.x};

  bool is_in_air = false;
  const int JUMP_VAL = -600;
  const int NEB_VEL = -200;

  bool collision{};

  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(WHITE);

    float delta_time = GetFrameTime();

    // Draw background
    bgX -= 20 * delta_time;
    mgX -= 40 * delta_time;
    fgX -= 80 * delta_time;

    if (bgX <= -background.width * 2)
    {
      bgX = 0;
    }

    if (mgX <= -midground.width * 2)
    {
      mgX = 0;
    }

    if (fgX <= -foreground.width * 2)
    {
      fgX = 0;
    }

    Vector2 bg1Pos{bgX, 0.0};
    Vector2 bg2Pos{bgX + (background.width * 2), 0.0};
    DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
    DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);
    Vector2 mg1Pos{mgX, 0.0};
    Vector2 mg2Pos{mgX + (midground.width * 2), 0.0};
    DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
    DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);
    Vector2 fg1Pos{fgX, 0.0};
    Vector2 fg2Pos{fgX + (foreground.width * 2), 0.0};
    DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
    DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

    // Scarfy position update logic
    if (isOnground(scarfyData, WINDOW_DIMENSIONS[1]))
    {
      velocity = 0;
      is_in_air = false;
    }
    else
    {
      velocity += GRAVITY * delta_time;
    }

    if (IsKeyPressed(KEY_SPACE) && !is_in_air)
    {
      velocity += JUMP_VAL;
      is_in_air = true;
    }

    // Nebula position update loop
    for (int i = 0; i < NEBULA_COUNT; i++)
    {
      nebulae[i].pos.x += NEB_VEL * delta_time;
    }

    finishLine = nebulae[NEBULA_COUNT - 1].pos.x + 20;

    scarfyData.pos.y += velocity * delta_time;

    if (!is_in_air)
    {
      scarfyData = updateAnimData(scarfyData, delta_time, 5);
    }

    // Nebula frame update loop
    for (int i = 0; i < NEBULA_COUNT; i++)
    {
      nebulae[i] = updateAnimData(nebulae[i], delta_time, 7);
    }

    for (AnimData nebula : nebulae)
    {
      float pad{50};

      Rectangle nebRec{
          nebula.pos.x + pad,
          nebula.pos.y + pad,
          nebula.rect.width - 2 * pad,
          nebula.rect.height - 2 * pad};

      Rectangle scarfyRec{
          scarfyData.pos.x,
          scarfyData.pos.y,
          scarfyData.rect.width,
          scarfyData.rect.height};

      if (CheckCollisionRecs(nebRec, scarfyRec))
      {
        collision = true;
      }
    }

    if (collision)
    {
      DrawText("Game Over!", WINDOW_DIMENSIONS[0] / 2, WINDOW_DIMENSIONS[1] / 2, 32, RED);
    }
    else if (scarfyData.pos.x >= finishLine)
    {
      DrawText("You Win!", WINDOW_DIMENSIONS[0] / 2, WINDOW_DIMENSIONS[1] / 2, 32, GREEN);
    }
    else
    {
      for (int i = 0; i < NEBULA_COUNT; i++)
      {
        DrawTextureRec(nebula, nebulae[i].rect, nebulae[i].pos, WHITE);
      }

      DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);
    }

    EndDrawing();
  }

  UnloadTexture(scarfy);
  UnloadTexture(nebula);
  UnloadTexture(background);
  UnloadTexture(midground);
  UnloadTexture(foreground);
  CloseWindow();
};